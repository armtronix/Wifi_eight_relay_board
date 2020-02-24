/*
  xsns_06_dht.ino - DHTxx, AM23xx and SI7021 temperature and humidity sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_DHT
/*********************************************************************************************\
 * DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321), SI7021 - Temperature and Humidy
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 * Source: Adafruit Industries https://github.com/adafruit/DHT-sensor-library
\*********************************************************************************************/

#define XSNS_06          6

#define DHT_MAX_SENSORS  3
#define DHT_MAX_RETRY    8

uint32_t dht_max_cycles;
uint8_t dht_data[5];
uint8_t dht_sensors = 0;

struct DHTSTRUCT {
  uint8_t     pin;
  uint8_t     type;
  char     stype[12];
  uint32_t lastreadtime;
  uint8_t  lastresult;
  float    t = NAN;
  float    h = NAN;
} Dht[DHT_MAX_SENSORS];

void DhtReadPrep(void)
{
  for (uint32_t i = 0; i < dht_sensors; i++) {
    digitalWrite(Dht[i].pin, HIGH);
  }
}

int32_t DhtExpectPulse(uint8_t sensor, bool level)
{
  int32_t count = 0;

  while (digitalRead(Dht[sensor].pin) == level) {
    if (count++ >= (int32_t)dht_max_cycles) {
      return -1;  // Timeout
    }
  }
  return count;
}

bool DhtRead(uint8_t sensor)
{
  int32_t cycles[80];
  uint8_t error = 0;

  dht_data[0] = dht_data[1] = dht_data[2] = dht_data[3] = dht_data[4] = 0;

//  digitalWrite(Dht[sensor].pin, HIGH);
//  delay(250);

  if (Dht[sensor].lastresult > DHT_MAX_RETRY) {
    Dht[sensor].lastresult = 0;
    digitalWrite(Dht[sensor].pin, HIGH);  // Retry read prep
    delay(250);
  }
  pinMode(Dht[sensor].pin, OUTPUT);
  digitalWrite(Dht[sensor].pin, LOW);

  if (GPIO_SI7021 == Dht[sensor].type) {
    delayMicroseconds(500);
  } else {
    delay(20);
  }

  noInterrupts();
  digitalWrite(Dht[sensor].pin, HIGH);
  delayMicroseconds(40);
  pinMode(Dht[sensor].pin, INPUT_PULLUP);
  delayMicroseconds(10);
  if (-1 == DhtExpectPulse(sensor, LOW)) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " " D_START_SIGNAL_LOW " " D_PULSE));
    error = 1;
  }
  else if (-1 == DhtExpectPulse(sensor, HIGH)) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " " D_START_SIGNAL_HIGH " " D_PULSE));
    error = 1;
  }
  else {
    for (uint32_t i = 0; i < 80; i += 2) {
      cycles[i]   = DhtExpectPulse(sensor, LOW);
      cycles[i+1] = DhtExpectPulse(sensor, HIGH);
    }
  }
  interrupts();
  if (error) { return false; }

  for (uint32_t i = 0; i < 40; ++i) {
    int32_t lowCycles  = cycles[2*i];
    int32_t highCycles = cycles[2*i+1];
    if ((-1 == lowCycles) || (-1 == highCycles)) {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " " D_PULSE));
      return false;
    }
    dht_data[i/8] <<= 1;
    if (highCycles > lowCycles) {
      dht_data[i / 8] |= 1;
    }
  }

  uint8_t checksum = (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF;
  if (dht_data[4] != checksum) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_CHECKSUM_FAILURE " %02X, %02X, %02X, %02X, %02X =? %02X"),
      dht_data[0], dht_data[1], dht_data[2], dht_data[3], dht_data[4], checksum);
    return false;
  }

  return true;
}

void DhtReadTempHum(uint8_t sensor)
{
  if ((NAN == Dht[sensor].h) || (Dht[sensor].lastresult > DHT_MAX_RETRY)) {  // Reset after 8 misses
    Dht[sensor].t = NAN;
    Dht[sensor].h = NAN;
  }
  if (DhtRead(sensor)) {
    switch (Dht[sensor].type) {
    case GPIO_DHT11:
      Dht[sensor].h = dht_data[0];
      Dht[sensor].t = dht_data[2] + ((float)dht_data[3] * 0.1f);  // Issue #3164
      break;
    case GPIO_DHT22:
    case GPIO_SI7021:
      Dht[sensor].h = ((dht_data[0] << 8) | dht_data[1]) * 0.1;
      Dht[sensor].t = (((dht_data[2] & 0x7F) << 8 ) | dht_data[3]) * 0.1;
      if (dht_data[2] & 0x80) {
        Dht[sensor].t *= -1;
      }
      break;
    }
    Dht[sensor].t = ConvertTemp(Dht[sensor].t);
    Dht[sensor].h = ConvertHumidity(Dht[sensor].h);
    Dht[sensor].lastresult = 0;
  } else {
    Dht[sensor].lastresult++;
  }
}

bool DhtSetup(uint8_t pin, uint8_t type)
{
  bool success = false;

  if (dht_sensors < DHT_MAX_SENSORS) {
    Dht[dht_sensors].pin = pin;
    Dht[dht_sensors].type = type;
    dht_sensors++;
    success = true;
  }
  return success;
}

/********************************************************************************************/

void DhtInit(void)
{
  dht_max_cycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for reading pulses from DHT sensor.

  for (uint32_t i = 0; i < dht_sensors; i++) {
    pinMode(Dht[i].pin, INPUT_PULLUP);
    Dht[i].lastreadtime = 0;
    Dht[i].lastresult = 0;
    GetTextIndexed(Dht[i].stype, sizeof(Dht[i].stype), Dht[i].type, kSensorNames);
    if (dht_sensors > 1) {
      snprintf_P(Dht[i].stype, sizeof(Dht[i].stype), PSTR("%s%c%02d"), Dht[i].stype, IndexSeparator(), Dht[i].pin);
    }
  }
}

void DhtEverySecond(void)
{
  if (uptime &1) {
    // <1mS
    DhtReadPrep();
  } else {
    for (uint32_t i = 0; i < dht_sensors; i++) {
      // DHT11 and AM2301 25mS per sensor, SI7021 5mS per sensor
      DhtReadTempHum(i);
    }
  }
}

void DhtShow(bool json)
{
  for (uint32_t i = 0; i < dht_sensors; i++) {
    char temperature[33];
    dtostrfd(Dht[i].t, Settings.flag2.temperature_resolution, temperature);
    char humidity[33];
    dtostrfd(Dht[i].h, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      ResponseAppend_P(JSON_SNS_TEMPHUM, Dht[i].stype, temperature, humidity);
#ifdef USE_DOMOTICZ
      if ((0 == tele_period) && (0 == i)) {
        DomoticzTempHumSensor(temperature, humidity);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      if ((0 == tele_period) && (0 == i)) {
        KnxSensor(KNX_TEMPERATURE, Dht[i].t);
        KnxSensor(KNX_HUMIDITY, Dht[i].h);
      }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TEMP, Dht[i].stype, temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, Dht[i].stype, humidity);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns06(uint8_t function)
{
  bool result = false;

  if (dht_flg) {
    switch (function) {
      case FUNC_INIT:
        DhtInit();
        break;
      case FUNC_EVERY_SECOND:
        DhtEverySecond();
        break;
      case FUNC_JSON_APPEND:
        DhtShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        DhtShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DHT
