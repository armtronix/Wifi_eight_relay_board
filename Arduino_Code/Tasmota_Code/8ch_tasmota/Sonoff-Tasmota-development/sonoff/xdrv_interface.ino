/*
  xdrv_interface.ino - Driver interface support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends inspired by ESPEasy

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

#ifdef XFUNC_PTR_IN_ROM
bool (* const xdrv_func_ptr[])(uint8_t) PROGMEM = {   // Driver Function Pointers
#else
bool (* const xdrv_func_ptr[])(uint8_t) = {   // Driver Function Pointers
#endif

#ifdef XDRV_01
  &Xdrv01,
#endif

#ifdef XDRV_02
  &Xdrv02,
#endif

#ifdef XDRV_03
  &Xdrv03,
#endif

#ifdef XDRV_04
  &Xdrv04,
#endif

#ifdef XDRV_05
  &Xdrv05,
#endif

#ifdef XDRV_06
  &Xdrv06,
#endif

#ifdef XDRV_07
  &Xdrv07,
#endif

#ifdef XDRV_08
  &Xdrv08,
#endif

#ifdef XDRV_09
  &Xdrv09,
#endif

#ifdef XDRV_10
  &Xdrv10,
#endif

#ifdef XDRV_11
  &Xdrv11,
#endif

#ifdef XDRV_12
  &Xdrv12,
#endif

#ifdef XDRV_13
  &Xdrv13,
#endif

#ifdef XDRV_14
  &Xdrv14,
#endif

#ifdef XDRV_15
  &Xdrv15,
#endif

#ifdef XDRV_16
  &Xdrv16,
#endif

#ifdef XDRV_17
  &Xdrv17,
#endif

#ifdef XDRV_18
  &Xdrv18,
#endif

#ifdef XDRV_19
  &Xdrv19,
#endif

#ifdef XDRV_20
  &Xdrv20,
#endif

#ifdef XDRV_21
  &Xdrv21,
#endif

#ifdef XDRV_22
  &Xdrv22,
#endif

#ifdef XDRV_23
  &Xdrv23,
#endif

#ifdef XDRV_24
  &Xdrv24,
#endif

#ifdef XDRV_25
  &Xdrv25,
#endif

#ifdef XDRV_26
  &Xdrv26,
#endif

#ifdef XDRV_27
  &Xdrv27,
#endif

#ifdef XDRV_28
  &Xdrv28,
#endif

#ifdef XDRV_29
  &Xdrv29,
#endif

#ifdef XDRV_30
  &Xdrv30,
#endif

#ifdef XDRV_31
  &Xdrv31,
#endif

#ifdef XDRV_32
  &Xdrv32,
#endif

// Optional user defined drivers in range 91 - 99

#ifdef XDRV_91
  &Xdrv91,
#endif

#ifdef XDRV_92
  &Xdrv92,
#endif

#ifdef XDRV_93
  &Xdrv93,
#endif

#ifdef XDRV_94
  &Xdrv94,
#endif

#ifdef XDRV_95
  &Xdrv95,
#endif

#ifdef XDRV_96
  &Xdrv96,
#endif

#ifdef XDRV_97
  &Xdrv97,
#endif

#ifdef XDRV_98
  &Xdrv98,
#endif

#ifdef XDRV_99
  &Xdrv99
#endif
};

const uint8_t xdrv_present = sizeof(xdrv_func_ptr) / sizeof(xdrv_func_ptr[0]);  // Number of drivers found

bool XdrvMqttData(char *topicBuf, uint16_t stopicBuf, char *dataBuf, uint16_t sdataBuf)
{
  XdrvMailbox.index = stopicBuf;
  XdrvMailbox.data_len = sdataBuf;
  XdrvMailbox.topic = topicBuf;
  XdrvMailbox.data = dataBuf;

  return XdrvCall(FUNC_MQTT_DATA);
}

bool XdrvRulesProcess(void)
{
  return XdrvCall(FUNC_RULES_PROCESS);
}

#ifdef USE_DEBUG_DRIVER
void ShowFreeMem(const char *where)
{
  char stemp[30];
  snprintf_P(stemp, sizeof(stemp), where);
  XdrvMailbox.data = stemp;
  XdrvCall(FUNC_FREE_MEM);
}
#endif

/*********************************************************************************************\
 * Function call to all xdrv
\*********************************************************************************************/

bool XdrvCall(uint8_t Function)
{
  bool result = false;

  for (uint32_t x = 0; x < xdrv_present; x++) {
//    WifiAddDelayWhenDisconnected();
    result = xdrv_func_ptr[x](Function);

    if (result && ((FUNC_COMMAND == Function) ||
                   (FUNC_COMMAND_DRIVER == Function) ||
                   (FUNC_MQTT_DATA == Function) ||
                   (FUNC_RULES_PROCESS == Function) ||
                   (FUNC_BUTTON_PRESSED == Function) ||
                   (FUNC_SERIAL == Function) ||
                   (FUNC_MODULE_INIT == Function) ||
                   (FUNC_SET_CHANNELS == Function) ||
                   (FUNC_SET_DEVICE_POWER == Function)
                  )) {
      break;
    }
  }

  return result;
}
