/*WIFI EIGHT RELAY BOARD VIRTUAL SWITCH CODE
 * 
 * Physial Switch                     Relay           MQTT ON      MQTT OFF            HTTP ON                         HTTP OFF 
 *   switch 1(GPIO7)                  Relay1(GPIO4)   R1_ON         R1_OFF       http://ip/gpio?state_05=1         http://ip/gpio?state_05=0
 *   switch 2( GPIO11)                Relay2(GPIO5)   R2_ON         R2_OFF       http://ip/gpio?state_09=1         http://ip/gpio?state_09=0
 *   switch 3(GPIO12)                 Relay3(GPIO6)   R3_ON         R3_OFF       http://ip/gpio?state_06=1         http://ip/gpio?state_06=0
 *   switch 4(GPIO13)                 Relay4(GPIO8)   R4_ON         R4_OFF       http://ip/gpio?state_08=1         http://ip/gpio?state_08=0
 *   switch 5(GPIOA0)                 Relay5(GPIO9)   R5_ON         R5_OFF       http://ip/gpio?state_02=1         http://ip/gpio?state_02=0
 *   switch 6(GPIOA1)                 Relay6(GPIO10)  R6_ON         R6_OFF       http://ip/gpio?state_04=1         http://ip/gpio?state_04=0
 *   switch 7(GPIOA2)                 Relay7(GPIO2)   R7_ON         R7_OFF       http://ip/gpio?state_10=1         http://ip/gpio?state_10=0
 *   switch 8(GPIOA3)                 Relay8(GPIO3)   R8_ON         R8_OFF       http://ip/gpio?state_03=1         http://ip/gpio?state_03=0
 * 
 *  This sketch is running a web server for configuring WiFI if can't connect or for controlling of one GPIO to switch a light/LED
 *  Also it supports to change the state of the light via MQTT message and gives back the state after change.
 *  The push button has to switch to ground. It has following functions: Normal press less than 1 sec but more than 50ms-> Switch light. Restart press: 3 sec -> Restart the module. Reset press: 20 sec -> Clear the settings in EEPROM
 *  While a WiFi config is not set or can't connect:
 *    http://server_ip/gpio ->will give a config page with 
 *    http://server_ip/gpio?state_05=0 -> Will change the GPIO4 pin to low 
 *    http://server_ip/gpio?state_05=1 -> Will change the GPIO4 pin to high
 *    http://server_ip/gpio?state_09=0 -> Will change the GPIO5 pin to low
 *    http://server_ip/gpio?state_09=1 -> Will change the GPIO5 pin to high
 *    http://server_ip/gpio?state_06=0 -> Will change the GPIO6 pin to low
 *    http://server_ip/gpio?state_06=1 -> Will change the GPIO6 pin to high
 *    http://server_ip/gpio?state_08=0 -> Will change the GPIO8 pin to low
 *    http://server_ip/gpio?state_08=1 -> Will change the GPIO8 pin to high
 *    http://server_ip/gpio?state_02=0 -> Will change the GPIO9 pin to low
 *    http://server_ip/gpio?state_02=1 -> Will change the GPIO9 pin to high
 *    http://server_ip/gpio?state_04=0 -> Will change the GPIO10 pin to low
 *    http://server_ip/gpio?state_04=1 -> Will change the GPIO10 pin to high
 *    http://server_ip/gpio?state_10=0 -> Will change the GPIO2 pin to low
 *    http://server_ip/gpio?state_10=1 -> Will change the GPIO2 pin to high
 *    http://server_ip/gpio?state_3=0 -> Will change the GPIO3 pin to low
 *    http://server_ip/gpio?state_3=0 -> Will change the GPIO3 pin to high
 *   http://server_ip/cleareeprom -> Will reset the WiFi setting and rest to configure mode as AP
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected. (most likly it will be 192.168.4.1)
 * To force AP config mode, press button 20 Secs!
 *  For several snippets used, the credit goes to:
 *  - https://github.com/esp8266
 *  - https://github.com/chriscook8/esp-arduino-apboot
 *  - https://github.com/knolleary/pubsubclient
 *  - https://github.com/vicatcu/pubsubclient <- Currently this needs to be used instead of the origin
 *  - https://gist.github.com/igrr/7f7e7973366fc01d6393
 *  - http://www.esp8266.com/viewforum.php?f=25
 *  - http://www.esp8266.com/viewtopic.php?f=29&t=2745
 *  - And the whole Arduino and ESP8266 comunity
 */
#define TOKEN "5gOhO74wjWzJzJXtBOs5"
#define DEBUG
//#define WEBOTA
//debug added for information, change this according your needs

#ifdef DEBUG
  #define Debug(x)    Serial.print(x)
  #define Debugln(x)  Serial.println(x)
  #define Debugf(...) Serial.printf(__VA_ARGS__)
  #define Debugflush  Serial.flush
#else
  #define Debug(x)    {}
  #define Debugln(x)  {}
  #define Debugf(...) {}
  #define Debugflush  {}
#endif


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <EEPROM.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <Espalexa.h>
//#include "fauxmoESP.h"
//fauxmoESP fauxmo;
extern "C" {
  #include "user_interface.h" //Needed for the reset command
}


void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void fifthLightChanged(uint8_t brightness);
void sixthLightChanged(uint8_t brightness);
void seventhLightChanged(uint8_t brightness);
void eightLightChanged(uint8_t brightness);

//***** Settings declare ********************************************************************************************************* 
String hostName ="Armtronix"; //The MQTT ID -> MAC adress will be added to make it kind of unique
int iotMode=0; //IOT mode: 0 = Web control, 1 = MQTT (No const since it can change during runtime)
//select GPIO's
//#define OUTPIN_04 4 //output pin
//#define OUTPIN_12 12 //output pin
//#define OUTPIN_13 13//output pin
//#define OUTPIN_14 14 //output pin
//#define INPIN_0 0
//#define INPIN_16 16// input pin (push button)
//#define INPIN_2 2
//#define INPIN_5 5
//#define INPIN_15 15
//#define OUTLED 5 
#define RESTARTDELAY 3 //minimal time in sec for button press to reset
#define HUMANPRESSDELAY 50 // the delay in ms untill the press should be handled as a normal push by human. Button debounce. !!! Needs to be less than RESTARTDELAY & RESETDELAY!!!
#define RESETDELAY 20 //Minimal time in sec for button press to reset all settings and boot to config mode
#define RESET_PIN 16
//##### Object instances ##### 
MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient;
Ticker btn_timer;
Ticker otaTickLoop;
Espalexa espalexa;

//##### Flags ##### They are needed because the loop needs to continue and cant wait for long tasks!
int rstNeed=0;   // Restart needed to apply new settings
int toPub=0; // determine if state should be published.
int configToClear=0; // determine if config should be cleared.
int otaFlag=0;
boolean inApMode=0;
//##### Global vars ##### 
int webtypeGlob;
int otaCount=300; //imeout in sec for OTA mode
int current; //Current state of the button
unsigned long count = 0; //Button press time counter
String st; //WiFi Stations HTML list
String state; //State of light
char buf[40]; //For MQTT data recieve
char* host; //The DNS hostname
//To be read from Config file
String esid="";
String epass = "";
String pubTopic;
String subTopic;
String mqttServer = "";
String mqtt_user ="";      //added on 28/07/2018
String mqtt_passwd ="";    //added on 28/07/2018
String mqtt_will_msg =" disconnected";
const char* otaServerIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

/*Alexa event names */
boolean wifiConnected = false;
String firstName;
String secondName;
String thirdName;
String fourthName;
String fifthName;
String sixthName;
String seventhName;
String eighthName;

int output_pin_status_05, output_pin_status_09, output_pin_status_06, output_pin_status_08, output_pin_status_02, output_pin_status_04, output_pin_status_10, output_pin_status_03;

//-------------- void's -------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(RESET_PIN, OUTPUT); 
  digitalWrite(RESET_PIN, HIGH);
  // prepare GPIO2
  //pinMode(OUTPIN_04, OUTPUT);
  //pinMode(OUTPIN_12, OUTPUT);
  //pinMode(OUTPIN_13, OUTPUT);
  //pinMode(OUTPIN_14, OUTPUT);
  //pinMode(OUTLED, OUTPUT);

   //pinMode(INPIN_5, INPUT);
  //  pinMode(INPIN_2, INPUT);
  //  pinMode(INPIN_0, INPUT);
  //  pinMode(INPIN_15, INPUT);
  //pinMode(INPIN, INPUT_PULLUP);
  //digitalWrite(OUTLED, HIGH);
 // btn_timer.attach(0.05, btn_handle);
  Debugln("DEBUG: Entering loadConfig()");
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  }
  
  uint8_t mac[6];
  WiFi.macAddress(mac);
  hostName += "-";
  hostName += macToStr(mac);
  String hostTemp=hostName;
  hostTemp.replace(":","-");
  host = (char*) hostTemp.c_str();
  loadConfig();
  //loadConfigOld();
  Debugln("DEBUG: loadConfig() passed");
  
  // Connect to WiFi network
  Debugln("DEBUG: Entering initWiFi()");
  initWiFi();
  Debugln("DEBUG: initWiFi() passed");
  Debug("iotMode:");
  Debugln(iotMode);
  Debug("webtypeGlob:");
  Debugln(webtypeGlob);
  Debug("otaFlag:");
  Debugln(otaFlag);
  Debugln("DEBUG: Starting the main loop");

 //Alexa Part
   if(wifiConnected){
    server.on("/", HTTP_GET, [](){
    server.send(200, "text/plain", "This is an example index page your server may send.");
    });
    server.on("/test", HTTP_GET, [](){
    server.send(200, "text/plain", "This is a second subpage you may have.");
    });
    server.onNotFound([](){
      if (!espalexa.handleAlexaApiCall(server.uri(),server.arg(0))) //if you don't know the URI, ask espalexa whether it is an Alexa control request
      {
        //whatever you want to do with 404s
        server.send(404, "text/plain", "Not found");
      }
    });
  }
  // Define your devices here.
  espalexa.addDevice((char*)firstName.c_str(), firstLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)secondName.c_str(), secondLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)thirdName.c_str(), thirdLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)fourthName.c_str(), fourthLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)fifthName.c_str(), fifthLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)sixthName.c_str(), sixthLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)seventhName.c_str(), seventhLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)eighthName.c_str(), eighthLightChanged); //simplest definition, default state off
  espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own

}

//    // Device Names for Simulated Wemo switches
//    fauxmo.addDevice((char*)firstName.c_str());
//    fauxmo.addDevice((char*)secondName.c_str());
//    fauxmo.addDevice((char*)thirdName.c_str());
//    fauxmo.addDevice((char*)fourthName.c_str());
//    fauxmo.addDevice((char*)fifthName.c_str());
//    fauxmo.addDevice((char*)sixthName.c_str());
//    fauxmo.addDevice((char*)seventhName.c_str());
//    fauxmo.addDevice((char*)eighthName.c_str());
//    fauxmo.onMessage(callback);
//}


//void btn_handle()
//{
//  if(!digitalRead(INPIN)){
//    ++count; // one count is 50ms
//  } else {
//    if (count > 1 && count < HUMANPRESSDELAY/5) { //push between 50 ms and 1 sec      
//      Serial.print("button pressed "); 
//      Serial.print(count*0.05); 
//      Serial.println(" Sec."); 
//    
//      Serial.print("Light is ");
//      Serial.println(digitalRead(OUTPIN_04));
//      
//      Serial.print("Switching light to "); 
//      Serial.println(!digitalRead(OUTPIN_04));
//      digitalWrite(OUTPIN_04, !digitalRead(OUTPIN_04)); 
//      state = digitalRead(OUTPIN_04);
//      if(iotMode==1 && mqttClient.connected()){
//        toPub=1;        
//        Debugln("DEBUG: toPub set to 1");
//      }
//    } else if (count > (RESTARTDELAY/0.05) && count <= (RESETDELAY/0.05)){ //pressed 3 secs (60*0.05s)
//      Serial.print("button pressed "); 
//      Serial.print(count*0.05); 
//      Serial.println(" Sec. Restarting!"); 
//      setOtaFlag(!otaFlag);      
//      ESP.reset();
//    } else if (count > (RESETDELAY/0.05)){ //pressed 20 secs
//      Serial.print("button pressed "); 
//      Serial.print(count*0.05); 
//      Serial.println(" Sec."); 
//      Serial.println(" Clear settings and resetting!");       
//      configToClear=1;
//      }
//    count=0; //reset since we are at high
//  }
//}



//-------------------------------- Main loop ---------------------------
void loop() {

  String serialReceived = "";

            if(Serial.available() > 0) {

            serialReceived = Serial.readStringUntil('\n');
           // serialReceived.substring(0,1).toCharArray(commandChar, 1);

             //Serial.println(serialReceived);
              
              }

           if (serialReceived.substring(0,11) == "relay1+1+on") // relay1 status
            {
            String relay1_status = serialReceived.substring(7,8);
             output_pin_status_05 = relay1_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay1+0+off")
            {
            String relay1_status = serialReceived.substring(7,8);
             output_pin_status_05 = relay1_status.toInt();
             }
//----------------------------------------------------------------------------------------------

if (serialReceived.substring(0,11) == "relay2+1+on") // relay2 status
            {
            String relay2_status = serialReceived.substring(7,8);
             output_pin_status_09 = relay2_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay2+0+off")
            {
            String relay2_status = serialReceived.substring(7,8);
             output_pin_status_09 = relay2_status.toInt();
             }
//----------------------------------------------------------------------------------------------

if (serialReceived.substring(0,11) == "relay3+1+on") // relay3 status
            {
            String relay3_status = serialReceived.substring(7,8);
             output_pin_status_06 = relay3_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay3+0+off")
            {
            String relay3_status = serialReceived.substring(7,8);
             output_pin_status_06 = relay3_status.toInt();
             }
//----------------------------------------------------------------------------------------------
if (serialReceived.substring(0,11) == "relay4+1+on") // relay4 status
            {
            String relay4_status = serialReceived.substring(7,8);
             output_pin_status_08 = relay4_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay1+0+off")
            {
            String relay4_status = serialReceived.substring(7,8);
             output_pin_status_08 = relay4_status.toInt();
             }
//----------------------------------------------------------------------------------------------
  if (serialReceived.substring(0,11) == "relay5+1+on") // relay5 status
            {
            String relay5_status = serialReceived.substring(7,8);
             output_pin_status_02 = relay5_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay5+0+off")
            {
            String relay5_status = serialReceived.substring(7,8);
             output_pin_status_02 = relay5_status.toInt();
             }
//----------------------------------------------------------------------------------------------
if (serialReceived.substring(0,11) == "relay6+1+on") // relay6 status
            {
            String relay6_status = serialReceived.substring(7,8);
             output_pin_status_04 = relay6_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay6+0+off")
            {
            String relay6_status = serialReceived.substring(7,8);
             output_pin_status_04 = relay6_status.toInt();
             }
//----------------------------------------------------------------------------------------------
if (serialReceived.substring(0,11) == "relay7+1+on") // relay7 status
            {
            String relay7_status = serialReceived.substring(7,8);
             output_pin_status_10 = relay7_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay7+0+off")
            {
            String relay7_status = serialReceived.substring(7,8);
             output_pin_status_10 = relay7_status.toInt();
             }
//----------------------------------------------------------------------------------------------
if (serialReceived.substring(0,11) == "relay8+1+on") // relay8 status
            {
            String relay8_status = serialReceived.substring(7,8);
             output_pin_status_03 = relay8_status.toInt();
             }
         if (serialReceived.substring(0,12) == "relay8+0+off")
            {
            String relay8_status = serialReceived.substring(7,8);
             output_pin_status_03 = relay8_status.toInt();
             }
//----------------------------------------------------------------------------------------------
  //Debugln("DEBUG: loop() begin");
  if(configToClear==1){
    //Debugln("DEBUG: loop() clear config flag set!");
    clearConfig()? Serial.println("Config cleared!") : Serial.println("Config could not be cleared");
    delay(1000);
    ESP.reset();
  }
  //Debugln("DEBUG: config reset check passed");  
  if (WiFi.status() == WL_CONNECTED && otaFlag){
    if(otaCount<=1) {
      Serial.println("OTA mode time out. Reset!"); 
      setOtaFlag(0);
      ESP.reset();
      delay(100);
    }
    server.handleClient();
    delay(1);

  } else if (WiFi.status() == WL_CONNECTED || webtypeGlob == 1){
    //Debugln("DEBUG: loop() wifi connected & webServer ");
    if (iotMode==0 || webtypeGlob == 1){
      //Debugln("DEBUG: loop() Web mode requesthandling ");
      server.handleClient();
      delay(1);
      if(esid != "" && WiFi.status() != WL_CONNECTED) //wifi reconnect part
      {
        Scan_Wifi_Networks();
      }
    } else if (iotMode==1 && webtypeGlob != 1 && otaFlag !=1){
          //Debugln("DEBUG: loop() MQTT mode requesthandling ");
          if (!connectMQTT()){
              delay(200);          
          }                    
          if (mqttClient.connected()){            
              //Debugln("mqtt handler");
              mqtt_handler();
          } else {
              Debugln("mqtt Not connected!");
          }
    }
  } else{
    Debugln("DEBUG: loop - WiFi not connected");  
    delay(1000);
    initWiFi(); //Try to connect again
  }
 
    //Debugln("DEBUG: loop() end");
//    if(digitalRead(0)== LOW)
//  {
//  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 1 ON");
//  }
//  else
//  {
//  digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 1 OFF");
// }
//
//  if(digitalRead(2)== LOW)
//  {
//  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 2 ON");
//  }
//  else
//  {
//  digitalWrite(12, LOW);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 2 OFF");
//  }
//
//  if(digitalRead(15)== HIGH)
//  {
//  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 3 ON");
//  }
//  else
//  {
//  digitalWrite(13, LOW);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 3 OFF");
//  }
//
//  if(digitalRead(5)== HIGH)
//  {
//  digitalWrite(14, HIGH);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 4 ON");
//  }
//  else
//  {
//  digitalWrite(14, LOW);   // turn the LED on (HIGH is the voltage level)
//  Serial.println("Relay 4 OFF");
//  }
  
//  fauxmo.handle();
  espalexa.loop();
   delay(1);
}

// -----------------------------------------------------------------------------
// Device Callback
// -----------------------------------------------------------------------------
//void callback(uint8_t device_id, const char * device_name, bool state) {
//  Serial.print("Device "); Serial.print(device_name); 
//  Serial.print(" state: ");
//  if (state) {
//    Serial.println("ON");
//  } else {
//    Serial.println("OFF");
//  }
//  //Switching action on detection of device name
//  if ( (strcmp(device_name, (char*)firstName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_1 switched via web request to 1"); 
//    } else {
//      Serial.println("R_1 switched via web request to 0"); 
//    }
//  }
//  if ( (strcmp(device_name, (char*)secondName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_2 switched via web request to 1");
//    } else {
//      Serial.println("R_2 switched via web request to 0");
//    }
//  }
//  if ( (strcmp(device_name, (char*)thirdName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_3 switched via web request to 1");
//    } else {
//      Serial.println("R_3 switched via web request to 0");
//    }
//  }
//  if ( (strcmp(device_name, (char*)fourthName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_4 switched via web request to 1");
//    } else {
//      Serial.println("R_4 switched via web request to 0");
//    }
//  }
//    if ( (strcmp(device_name, (char*)fifthName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_5 switched via web request to 1");
//    } else {
//      Serial.println("R_5 switched via web request to 0");
//    }
//  }
//    if ( (strcmp(device_name, (char*)sixthName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_6 switched via web request to 1");
//    } else {
//      Serial.println("R_6 switched via web request to 0");
//    }
//    }
//      if ( (strcmp(device_name, (char*)seventhName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_7 switched via web request to 1");
//    } else {
//      Serial.println("R_7 switched via web request to 0");
//    }
//  }
//    if ( (strcmp(device_name, (char*)eighthName.c_str()) == 0) ) {
//    // adjust the relay immediately!
//    if (state) {
//      Serial.println("R_8 switched via web request to 1");
//    } else {
//      Serial.println("R_8 switched via web request to 0");
//    }
//  }
//  
//  
//}

void firstLightChanged(uint8_t brightness) {
    Serial.print("R_1 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    //  Serial.println("on");
    }
    else if (brightness == 0) {
      Serial.println("0");
    // Serial.println("off");
    }
}

void secondLightChanged(uint8_t brightness) {
    Serial.print("R_2 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    }
    else if (brightness == 0) {
      Serial.println("0");
    }
}

void thirdLightChanged(uint8_t brightness) {
    Serial.print("R_3 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    }
    else if (brightness == 0) {
      Serial.println("0");
    }
}

void fourthLightChanged(uint8_t brightness) {
    Serial.print("R_4 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    }
    else if (brightness == 0) {
      Serial.println("0");
    }
}

void fifthLightChanged(uint8_t brightness) {
    Serial.print("R_5 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    }
    else if (brightness == 0) {
      Serial.println("0");
    }
}

void sixthLightChanged(uint8_t brightness) {
    Serial.print("R_6 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    }
    else if (brightness == 0) {
      Serial.println("0");
    }
}

void seventhLightChanged(uint8_t brightness) {
    Serial.print("R_7 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    }
    else if (brightness == 0) {
      Serial.println("0");
    }
}

void eighthLightChanged(uint8_t brightness) {
    Serial.print("R_8 switched via web request to ");
    
    if (brightness == 255) {
      Serial.println("1");
    }
    else if (brightness == 0) {
      Serial.println("0");
    }
}
