//Relay
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
//int state_05, state_09, state_06, state_08, state_02, state_04, state_10, state_03;
//int send_status_05, send_status_09, send_status_06, send_status_08, send_status_02, send_status_04, send_status_10, send_status_03;
String serialReceived;
String serialReceived1, serialReceived2, serialReceived3, serialReceived4, serialReceived5, serialReceived6, serialReceived7, serialReceived8;
///#define Led_signal 13




void setup() {

   Serial.begin(115200);
   pinMode(Relay1, OUTPUT); //relay1 output
   pinMode(Relay2, OUTPUT); //relay2 output
   pinMode(Relay3, OUTPUT); //relay3 output
   pinMode(Relay4, OUTPUT); //relay4 output
   pinMode(Relay5, OUTPUT); //relay5 output
   pinMode(Relay6, OUTPUT); //relay6 output
   pinMode(Relay7, OUTPUT); //relay7 output
   pinMode(Relay8, OUTPUT); //relay8 output

   pinMode(SWITCH_INPIN1, INPUT); //manual switch 1 input
   pinMode(SWITCH_INPIN2, INPUT); //manual switch 2 input
   pinMode(SWITCH_INPIN3, INPUT); //manual switch 3 input
   pinMode(SWITCH_INPIN4, INPUT); //manual switch 4 input
   pinMode(SWITCH_INPIN5, INPUT); //manual switch 5 input
   pinMode(SWITCH_INPIN6, INPUT); //manual switch 6 input
   pinMode(SWITCH_INPIN7, INPUT); //manual switch 7 input
   pinMode(SWITCH_INPIN8, INPUT); //manual switch 8 input
}

void loop() {

  
  if(Serial.available() > 0) {    // is a character available
   serialReceived = Serial.readStringUntil('\n');

       Serial.println(serialReceived.substring(0,33));
        if(serialReceived.substring(0,33) == "R_1 switched via web request to 1")
                  {
            serialReceived1 = serialReceived;
        Serial.println(serialReceived1.substring(0,33));
                  }  
          if(serialReceived.substring(0,33) == "R_1 switched via web request to 0")
                  {
            serialReceived1 = serialReceived;
       
                  }   
                  
           if(serialReceived.substring(0,33) == "R_2 switched via web request to 1")
                  {
            serialReceived2 = serialReceived;
       
                  }  
             if(serialReceived.substring(0,33) == "R_2 switched via web request to 0")
                  {
            serialReceived2 = serialReceived;
       
                  }  

             if(serialReceived.substring(0,33) == "R_3 switched via web request to 1")
                  {
            serialReceived3 = serialReceived;
       
                  }  
              if(serialReceived.substring(0,33) == "R_3 switched via web request to 0")
                  {
            serialReceived3 = serialReceived;
       
                  }  

                  if(serialReceived.substring(0,33) == "R_4 switched via web request to 1")
                  {
            serialReceived4 = serialReceived;
       
                  }  
                  if(serialReceived.substring(0,33) == "R_4 switched via web request to 0")
                  {
            serialReceived4 = serialReceived;
       
                  }  

                  if(serialReceived.substring(0,33) == "R_5 switched via web request to 1")
                  {
            serialReceived5 = serialReceived;
       
                  }  
                  if(serialReceived.substring(0,33) == "R_5 switched via web request to 0")
                  {
            serialReceived5 = serialReceived;
       
                  }  

                       if(serialReceived.substring(0,33) == "R_6 switched via web request to 1")
                  {
            serialReceived6 = serialReceived;
       
                  }  
                  if(serialReceived.substring(0,33) == "R_6 switched via web request to 0")
                  {
            serialReceived6 = serialReceived;
                  }

         if(serialReceived.substring(0,33) == "R_7 switched via web request to 1")
                  {
            serialReceived7 = serialReceived;
       
                  }  
                  if(serialReceived.substring(0,33) == "R_7 switched via web request to 0")
                  {
             serialReceived7 = serialReceived;
                  }

     if(serialReceived.substring(0,33) == "R_8 switched via web request to 1")
                  {
            serialReceived8 = serialReceived;
       
                  }  
                  if(serialReceived.substring(0,33) == "R_8 switched via web request to 0")
                  {
            serialReceived8 = serialReceived;
                  }

                         
                  }

    
//Relay1-----------------------------
if(((serialReceived1.substring(0,33) == "R_1 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN1))))||((!(serialReceived1.substring(0,33) == "R_1 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN1)))))  //exor logic
      {
        digitalWrite(Relay1, LOW);
       }
      else
      {
        digitalWrite(Relay1, HIGH);
     }

//Relay2-----------------------------
if(((serialReceived2.substring(0,33) == "R_2 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN2))))||((!(serialReceived2.substring(0,33) == "R_2 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN2)))))  //exor logic
      {
       digitalWrite(Relay2, LOW);
       }
      else
      {
        digitalWrite(Relay2, HIGH);
           
      }

      
//Relay3-----------------------------

       if(((serialReceived3.substring(0,33) == "R_3 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN3))))||((!(serialReceived3.substring(0,33) == "R_3 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN3)))))  //exor logic
      {
       digitalWrite(Relay3, LOW);
   
      }
      else
      {
        digitalWrite(Relay3, HIGH);
      }
      
//Relay4-----------------------------
       if(((serialReceived4.substring(0,33) == "R_4 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN4))))||((!(serialReceived4.substring(0,33) == "R_4 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN4)))))  //exor logic
      {
       digitalWrite(Relay4, LOW);
       }
      else
      {
        digitalWrite(Relay4, HIGH);
       }
       
//Relay5-----------------------------
       if(((serialReceived5.substring(0,33) == "R_5 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN5))))||((!(serialReceived5.substring(0,33) == "R_5 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN5)))))  //exor logic
      {
       digitalWrite(Relay5, LOW);   
      }
      else
      {
        digitalWrite(Relay5, HIGH);
       }
       
//Relay6-----------------------------
       if(((serialReceived6.substring(0,33) == "R_6 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN6))))||((!(serialReceived6.substring(0,33) == "R_6 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN6)))))  //exor logic
      {
       digitalWrite(Relay6, LOW);
       }
      else
      {
        digitalWrite(Relay6, HIGH);
       }
       
//Relay7-----------------------------
       if(((serialReceived7.substring(0,33) == "R_7 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN7))))||((!(serialReceived7.substring(0,33) == "R_7 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN7)))))  //exor logic
      {
      
      digitalWrite(Relay7, LOW);
       }
      else
      {
        digitalWrite(Relay7, HIGH);
      }
      
//Relay8-----------------------------
       if(((serialReceived8.substring(0,33) == "R_8 switched via web request to 1")&&(!(digitalRead(SWITCH_INPIN8))))||((!(serialReceived8.substring(0,33) == "R_8 switched via web request to 1"))&&((digitalRead(SWITCH_INPIN8)))))  //exor logic
      {
       digitalWrite(Relay8, LOW);
       }
      else
      {
        digitalWrite(Relay8, HIGH);
       }
     
            
   } 


