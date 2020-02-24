/*
  xsns_01_counter.ino - Counter sensors (water meters, electricity meters etc.) sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Maarten Damen and Theo Arends

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

#ifdef USE_COUNTER
/*********************************************************************************************\
 * Counter sensors (water meters, electricity meters etc.)
\*********************************************************************************************/

#define XSNS_01             1

unsigned long last_counter_timer[MAX_COUNTERS]; // Last counter time in micro seconds

#ifndef ARDUINO_ESP8266_RELEASE_2_3_0       // Fix core 2.5.x ISR not in IRAM Exception
void CounterUpdate(uint8_t index) ICACHE_RAM_ATTR;
void CounterUpdate1(void) ICACHE_RAM_ATTR;
void CounterUpdate2(void) ICACHE_RAM_ATTR;
void CounterUpdate3(void) ICACHE_RAM_ATTR;
void CounterUpdate4(void) ICACHE_RAM_ATTR;
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

void CounterUpdate(uint8_t index)
{
  unsigned long counter_debounce_time = micros() - last_counter_timer[index -1];
  if (counter_debounce_time > Settings.pulse_counter_debounce * 1000) {
    last_counter_timer[index -1] = micros();
    if (bitRead(Settings.pulse_counter_type, index -1)) {
      RtcSettings.pulse_counter[index -1] = counter_debounce_time;
    } else {
      RtcSettings.pulse_counter[index -1]++;
    }

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("CNTR: Interrupt %d"), index);
  }
}

void CounterUpdate1(void)
{
  CounterUpdate(1);
}

void CounterUpdate2(void)
{
  CounterUpdate(2);
}

void CounterUpdate3(void)
{
  CounterUpdate(3);
}

void CounterUpdate4(void)
{
  CounterUpdate(4);
}

/********************************************************************************************/

void CounterSaveState(void)
{
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      Settings.pulse_counter[i] = RtcSettings.pulse_counter[i];
    }
  }
}

void CounterInit(void)
{
  typedef void (*function) () ;
  function counter_callbacks[] = { CounterUpdate1, CounterUpdate2, CounterUpdate3, CounterUpdate4 };

  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      pinMode(pin[GPIO_CNTR1 +i], bitRead(counter_no_pullup, i) ? INPUT : INPUT_PULLUP);
      attachInterrupt(pin[GPIO_CNTR1 +i], counter_callbacks[i], FALLING);
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_COUNTER[] PROGMEM =
  "{s}" D_COUNTER "%d{m}%s%s{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void CounterShow(bool json)
{
  char stemp[10];

  uint8_t dsxflg = 0;
  uint8_t header = 0;
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      char counter[33];
      if (bitRead(Settings.pulse_counter_type, i)) {
        dtostrfd((double)RtcSettings.pulse_counter[i] / 1000000, 6, counter);
      } else {
        dsxflg++;
        snprintf_P(counter, sizeof(counter), PSTR("%lu"), RtcSettings.pulse_counter[i]);
      }

      if (json) {
        if (!header) {
          ResponseAppend_P(PSTR(",\"COUNTER\":{"));
          stemp[0] = '\0';
        }
        header++;
        ResponseAppend_P(PSTR("%s\"C%d\":%s"), stemp, i +1, counter);
        strlcpy(stemp, ",", sizeof(stemp));
#ifdef USE_DOMOTICZ
        if ((0 == tele_period) && (1 == dsxflg)) {
          DomoticzSensor(DZ_COUNT, RtcSettings.pulse_counter[i]);
          dsxflg++;
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_COUNTER, i +1, counter, (bitRead(Settings.pulse_counter_type, i)) ? " " D_UNIT_SECOND : "");
#endif  // USE_WEBSERVER
      }
    }
    if (bitRead(Settings.pulse_counter_type, i)) {
      RtcSettings.pulse_counter[i] = 0xFFFFFFFF;  // Set Timer to max in case of no more interrupts due to stall of measured device
    }
  }
  if (json) {
    if (header) {
      ResponseJsonEnd();
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns01(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      CounterInit();
      break;
    case FUNC_JSON_APPEND:
      CounterShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      CounterShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
    case FUNC_SAVE_AT_MIDNIGHT:
      CounterSaveState();
      break;
  }
  return result;
}

#endif  // USE_COUNTER
