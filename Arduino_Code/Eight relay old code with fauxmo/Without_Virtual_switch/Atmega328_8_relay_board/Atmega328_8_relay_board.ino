
#define Relay1 4 //Gpio 4
#define Relay2 5 //Gpio 5
#define Relay3 6 //Gpio 6
#define Relay4 8 //Gpio 8
#define Relay5 9 //Gpio 9
#define Relay6 10//Gpio 10
#define Relay7 2 //Gpio 2
#define Relay8 3 //Gpio 3

//manual switch
#define SWITCH_INPIN1 7 //switch 1
#define SWITCH_INPIN2 11//switch 2
#define SWITCH_INPIN3 12//switch 3
#define SWITCH_INPIN4 13//switch 4
#define SWITCH_INPIN5 A0//switch 5
#define SWITCH_INPIN6 A1//switch 6
#define SWITCH_INPIN7 A2//switch 7
#define SWITCH_INPIN8 A3//switch 8

int switch_status1, switch_status2, switch_status3, switch_status4, switch_status5, switch_status6, switch_status7, switch_status8;
int state_05, state_09, state_06, state_08, state_02, state_04, state_10, state_03;
int send_status_05, send_status_09, send_status_06, send_status_08, send_status_02, send_status_04, send_status_10, send_status_03;
String serialReceived;

#define Led_signal 13




void setup() {

   Serial.begin(115200);
   pinMode(Led_signal, OUTPUT);  // LED on pin 13 of UNO
   pinMode(Relay1, OUTPUT);
   pinMode(Relay2, OUTPUT);
   pinMode(Relay3, OUTPUT);
   pinMode(Relay4, OUTPUT);
   pinMode(Relay5, OUTPUT);
   pinMode(Relay6, OUTPUT);
   pinMode(Relay7, OUTPUT);
   pinMode(Relay8, OUTPUT);

   pinMode(SWITCH_INPIN1, INPUT);
   pinMode(SWITCH_INPIN2, INPUT);
   pinMode(SWITCH_INPIN3, INPUT);
   pinMode(SWITCH_INPIN4, INPUT);
   pinMode(SWITCH_INPIN5, INPUT);
   pinMode(SWITCH_INPIN6, INPUT);
   pinMode(SWITCH_INPIN7, INPUT);
   pinMode(SWITCH_INPIN8, INPUT);
}

void loop() {

//String serialReceived = " ";
//
//         if(switch_status1==(digitalRead(SWITCH_INPIN1)))// to read the status of physical switch
//   {
//        // send_status=0;
//       // switch_status1=(digitalRead(SWITCH_INPIN1));
//   }
//   else
//  {
//    switch_status1=(digitalRead(SWITCH_INPIN1));
//     send_status_05=1;
//   }
//if(send_status_05==1)
//  {
//     send_status_05=0;
//     //toPub = 1;   
//  }
//  else
//  {   
//     //toPub = 0;
//  }
     

 
  
  if(Serial.available() > 0) {    // is a character available?
   serialReceived = Serial.readStringUntil('\n');

       Serial.println(serialReceived.substring(0,33));
                       }

    

if(((serialReceived.substring(0,33) == "R_1 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN1))))||((!(serialReceived.substring(0,33) == "R_1 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN1)))))  //exor logic
      {
      //digitalWrite(OUTLED, HIGH);
      digitalWrite(Relay1, HIGH);
     // digitalWrite(OUTPIN_04, HIGH);
     // toPub = 1;
       //state="Light is ON";
      //Serial.print("Light switched via web request to  ");      
      //Serial.println(digitalWrite(OUTPIN, HIGH));      
      }
      else
      {
        digitalWrite(Relay1, LOW);
      //digitalWrite(OUTPIN_04, LOW);
      //toPub = 1;
      // state="Light is OFF";
      //Serial.print("Light switched via web request to  ");      
      //Serial.println(digitalWrite(OUTPIN, LOW)); 
      }

//      if (serialReceived.substring(0,33) == "R_1 switched via web request to 1")   //Relay1 high
//       {
//          digitalWrite(Relay1, HIGH);
//          digitalWrite(Led_signal, HIGH);
//          delay(1000);
//          digitalWrite(Led_signal, LOW);
//          Serial.println("relay1 on");
//          delay(100);
//          Serial.println("relay1+1+on");
//
//       }
//
//       if (serialReceived.substring(0,33) == "R_1 switched via web request to 0")   //Relay1 low
//       {
//          digitalWrite(Relay1, LOW);
//          digitalWrite(Led_signal, HIGH);
//          delay(1000);
//          digitalWrite(Led_signal, LOW);
//          Serial.println("relay1 off");
//          delay(100);
//          Serial.println("relay1+0+off");
//
//       }

       if (serialReceived.substring(0,33) == "R_2 switched via web request to 1")   //Relay2 high
       {
          digitalWrite(Relay2, HIGH);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay2 on");
          delay(100);
          Serial.println("relay2+1+on");

       }

       if (serialReceived.substring(0,33) == "R_2 switched via web request to 0")   //Relay2 low
       {
          digitalWrite(Relay2, LOW);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay2 off");
          delay(100);
          Serial.println("relay2+0+off");

       }

       if (serialReceived.substring(0,33) == "R_3 switched via web request to 1")   //Relay3 high
       {
          digitalWrite(Relay3, HIGH);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay3 on");
          delay(100);
          Serial.println("relay3+1+on");

       }

       if (serialReceived.substring(0,33) == "R_3 switched via web request to 0")   //Relay3 low
       {
          digitalWrite(Relay3, LOW);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay3 off");
          delay(100);
          Serial.println("relay3+0+off");

       }

       if (serialReceived.substring(0,33) == "R_4 switched via web request to 1")   //Relay4 high
       {
          digitalWrite(Relay4, HIGH);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay4 on");
          delay(100);
          Serial.println("relay4+1+on");

       }

       if (serialReceived.substring(0,33) == "R_4 switched via web request to 0")   //Relay4 low
       {
          digitalWrite(Relay4, LOW);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay4 off");
          delay(100);
          Serial.println("relay4+0+off");

       }

       if (serialReceived.substring(0,33) == "R_5 switched via web request to 1")   //Relay5 high
       {
          digitalWrite(Relay5, HIGH);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay5 on");
          delay(100);
          Serial.println("relay5+1+on");

       }

       if (serialReceived.substring(0,33) == "R_5 switched via web request to 0")   //Relay5 low
       {
          digitalWrite(Relay5, LOW);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay5 off");
          delay(100);
          Serial.println("relay5+0+off");

       }

       if (serialReceived.substring(0,33) == "R_6 switched via web request to 1")   //Relay6 high
       {
          digitalWrite(Relay6, HIGH);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay6 on");
          delay(100);
          Serial.println("relay6+1+on");

       }

       if (serialReceived.substring(0,33) == "R_6 switched via web request to 0")   //Relay6 low
       {
          digitalWrite(Relay6, LOW);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay6 off");
          delay(100);
          Serial.println("relay6+0+off");

       }

       if (serialReceived.substring(0,33) == "R_7 switched via web request to 1")    //Relay7 high
       {
          digitalWrite(Relay7, HIGH);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay7 on");
          delay(100);
          Serial.println("relay7+1+on");

       }

       if (serialReceived.substring(0,33) == "R_7 switched via web request to 0")    //Relay7 low
       {
          digitalWrite(Relay7, LOW);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay7 off");
          delay(100);
          Serial.println("relay7+0+off");

       }

       if (serialReceived.substring(0,33) == "R_8 switched via web request to 1")    //Relay8 high
       {
          digitalWrite(Relay8, HIGH);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay8 on");
          delay(100);
          Serial.println("relay8+1+on");

       }

       if (serialReceived.substring(0,33) == "R_8 switched via web request to 0")     //Relay8 low
       {
          digitalWrite(Relay8, LOW);
          digitalWrite(Led_signal, HIGH);
          delay(1000);
          digitalWrite(Led_signal, LOW);
          Serial.println("relay8 off");
          delay(100);
          Serial.println("relay8+0+off");

       }

       

       
 
  
        
   } 


