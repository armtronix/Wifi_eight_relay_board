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

String serialReceived;
String serialReceived1, serialReceived2, serialReceived3, serialReceived4, serialReceived5, serialReceived6, serialReceived7, serialReceived8;
String binarydata;
int setflag1 = 0;
int setflag2 = 0;
int setflag3 = 0;
int setflag4 = 0;
int setflag5 = 0;
int setflag6 = 0;
int setflag7 = 0;
int setflag8 = 0;
//unsigned long currentmillis;
//unsigned long lastmillis = 0;
//long ontime = 2000;
int relayflag1 = 0 ;
int relayflag2 = 0 ;
int relayflag3 = 0 ;
int relayflag4 = 0 ;
int relayflag5 = 0 ;
int relayflag6 = 0 ;
int relayflag7 = 0 ;
int relayflag8 = 0 ;

void setup() {

  Serial.begin(19200);
  Serial.println("Wifi Eight Relay Board Ver0.2"); //added on 1-03-18
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
void loop() 
{
 
  if (Serial.available() > 0) 
    {   // is a character available
       serialReceived = Serial.readStringUntil('\n');
     if (serialReceived.substring(0, 7) == "Relays:")
      {
        String tempbinary = String((serialReceived.substring(7).toInt()), BIN);
            if (tempbinary.length() == 1)
              {
                binarydata = "0000000" + tempbinary;
              }
            else if (tempbinary.length() == 2)
              {
                binarydata = "000000" + tempbinary;
              }
            else if (tempbinary.length() == 3)
              {
                binarydata = "00000" + tempbinary;
              }
            else if (tempbinary.length() == 4)
              {
                binarydata = "0000" + tempbinary;
              }
            else if (tempbinary.length() == 5)
              {
                binarydata = "000" + tempbinary;
              }
            else if (tempbinary.length() == 6)
              {
                binarydata = "00" + tempbinary;
              }
            else if (tempbinary.length() == 7)
              {
                binarydata = "0" + tempbinary;
              }
            else if (tempbinary.length() == 8)
              {
                binarydata = tempbinary;
              }
           
       }

     if ((binarydata.substring(7, 8) == "1") && (digitalRead(Relay1) == LOW))
       {
           digitalWrite(Relay1, HIGH);
          // Serial.println("HIGH via serial1");
       } 
     else  if ((binarydata.substring(7, 8) == "0") && (digitalRead(Relay1) == HIGH))
       {
         digitalWrite(Relay1, LOW);
        // Serial.println("low via serial1"); 
       }
     if (binarydata.substring(6, 7) == "1" && (digitalRead(Relay2) == LOW))
       {
         digitalWrite(Relay2, HIGH);
       //  Serial.println("HIGH via serial2");
       } 
     else if (binarydata.substring(6, 7) == "0" && (digitalRead(Relay2) == HIGH))
       {
         digitalWrite(Relay2, LOW);
       //  Serial.println("low via serial2"); 
       }
     if (binarydata.substring(5, 6) == "1" && (digitalRead(Relay3) == LOW))
       {
         digitalWrite(Relay3, HIGH);
     //    Serial.println("HIGH via serial3");
       } 
     else if (binarydata.substring(5, 6) == "0" && (digitalRead(Relay3) == HIGH))
       {
         digitalWrite(Relay3, LOW);
    //    Serial.println("low via serial3"); 
       }
     if (binarydata.substring(4, 5) == "1" && (digitalRead(Relay4) == LOW))
      {
         digitalWrite(Relay4, HIGH);
     //    Serial.println("HIGH via serial4");
       } 
     else if (binarydata.substring(4, 5) == "0" && (digitalRead(Relay4) == HIGH))
       {
       digitalWrite(Relay4, LOW);
    //    Serial.println("low via serial4"); 
       }

     if (binarydata.substring(3, 4) == "1" && (digitalRead(Relay5) == LOW))
      {
         digitalWrite(Relay5, HIGH);
    //     Serial.println("HIGH via serial5");
       } 
     else if (binarydata.substring(3, 4) == "0" && (digitalRead(Relay5) == HIGH))
       {
       digitalWrite(Relay5, LOW);
    //    Serial.println("low via serial5"); 
       }

     if (binarydata.substring(2, 3) == "1" && (digitalRead(Relay6) == LOW))
       {
         digitalWrite(Relay6, HIGH);
     //    Serial.println("HIGH via serial6");
       } 
     else if (binarydata.substring(2, 3) == "0" && (digitalRead(Relay6) == HIGH))
       {
         digitalWrite(Relay6, LOW);
    //    Serial.println("low via serial6"); 
       }

     if (binarydata.substring(1, 2) == "1" && (digitalRead(Relay7) == LOW))
      {
         digitalWrite(Relay7, HIGH);
    //     Serial.println("HIGH via serial7");
       } 
     else if (binarydata.substring(1, 2) == "0" && (digitalRead(Relay7) == HIGH))
       {
         digitalWrite(Relay7, LOW);
    //    Serial.println("low via serial7"); 
       }

     if (binarydata.substring(0,1) == "1" && (digitalRead(Relay8) == LOW))
       {
         digitalWrite(Relay8, HIGH);
    //     Serial.println("HIGH via serial8");
       } 
     else if (binarydata.substring(0,1) == "0" && (digitalRead(Relay8) ==HIGH))
       {
         digitalWrite(Relay8, LOW);
    //    Serial.println("low via serial8"); 
       }
    }
  
 ///////////////////////////////////////////////////////////////////////////////////////
  if((digitalRead(SWITCH_INPIN1) == LOW) && (setflag1 == 0))
   {   
      if(digitalRead(Relay1) == HIGH)
        {
         relayflag1 = 1;
        }
      else
        {
          relayflag1 = 0;
        }
        setflag1 = 1;
   }
  else if((digitalRead(SWITCH_INPIN1) == HIGH) && (setflag1 == 1))
   {    
      if(digitalRead(Relay1) == HIGH)
        {
         relayflag1 = 1;
        }
      else
        {
          relayflag1 = 0;
        }
        setflag1 = 0;
   }
////////////////////////////////////////////////////////////////////////////////
  if((digitalRead(SWITCH_INPIN2) == LOW) && (setflag2 == 0))
   {  
     if(digitalRead(Relay2) == HIGH)
        {
         relayflag2 = 1;
        }
      else
        {
          relayflag2 = 0;
        }
        setflag2 = 1;
   }
  else if((digitalRead(SWITCH_INPIN2) == HIGH) && (setflag2 == 1))
   {  
     if(digitalRead(Relay2) == HIGH)
      {
       relayflag2 = 1;
      }
     else
      {
        relayflag2 = 0;
      }
        setflag2 = 0;
   }
//////////////////////////////////////////////////////////////////////////////////

  if((digitalRead(SWITCH_INPIN3) == LOW) && (setflag3 == 0))
   {
  
     if(digitalRead(Relay3) == HIGH)
      {
       relayflag3 = 1;
      }
     else
      {
        relayflag3 = 0;
      }
        setflag3 = 1;
   }
  else if((digitalRead(SWITCH_INPIN3) == HIGH) && (setflag3 == 1))
   {  
     if(digitalRead(Relay3) == HIGH)
      {
       relayflag3 = 1;
      }
     else
      {
        relayflag3 = 0;
      }
     setflag3 = 0;
   }
//////////////////////////////////////////////////////////////////////////////////
  if((digitalRead(SWITCH_INPIN4) == LOW) && (setflag4 == 0))
   {   
     if(digitalRead(Relay4) == HIGH)
      {
       relayflag4 = 1;
      }
     else
      {
        relayflag4 = 0;
      }
        setflag4 = 1;
   }
  else if((digitalRead(SWITCH_INPIN4) == HIGH) && (setflag4 == 1))
   { 
     if(digitalRead(Relay4) == HIGH)
      {
       relayflag4 = 1;
      }
     else
      {
        relayflag4 = 0;
      }
        setflag4 = 0;
   }
//////////////////////////////////////////////////////////////////////////////////
  if((digitalRead(SWITCH_INPIN5) == LOW) && (setflag5 == 0))
   {   
     if(digitalRead(Relay5) == HIGH)
      {
       relayflag5 = 1;
      }
     else
      {
        relayflag5 = 0;
      }
        setflag5 = 1;
   }
  else if((digitalRead(SWITCH_INPIN5) == HIGH) && (setflag5 == 1))
   {  
     if(digitalRead(Relay5) == HIGH)
      {
       relayflag5 = 1;
      }
     else
      {
        relayflag5 = 0;
      }
        setflag5 = 0;
   }
//////////////////////////////////////////////////////////////////////////////////
  if((digitalRead(SWITCH_INPIN6) == LOW) && (setflag6 == 0))
   {   
     if(digitalRead(Relay6) == HIGH)
      {
       relayflag6 = 1;
      }
     else
      {
        relayflag6 = 0;
      }
        setflag6 = 1;
    }
  else if((digitalRead(SWITCH_INPIN6) == HIGH) && (setflag6 == 1))
   {  
     if(digitalRead(Relay6) == HIGH)
      {
       relayflag6 = 1;
      }
     else
      {
        relayflag6 = 0;
      }
        setflag6 = 0;
   }
//////////////////////////////////////////////////////////////////////////////////
  if((digitalRead(SWITCH_INPIN7) == LOW) && (setflag7 == 0))
   {   
     if(digitalRead(Relay7) == HIGH)
      {
       relayflag7 = 1;
      }
     else
      {
        relayflag7 = 0;
      }
        setflag7 = 1;
   }
  else if((digitalRead(SWITCH_INPIN7) == HIGH) && (setflag7 == 1))
   {  
     if(digitalRead(Relay7) == HIGH)
      {
       relayflag7 = 1;
      }
      else
      {
        relayflag7 = 0;
      }
        setflag7 = 0;
   }
//////////////////////////////////////////////////////////////////////////////////
  if((digitalRead(SWITCH_INPIN8) == LOW) && (setflag8 == 0))
   {   
     if(digitalRead(Relay8) == HIGH)
      {
       relayflag8 = 1;
      }
     else
      {
        relayflag8 = 0;
      }
        setflag8 = 1;
    }
  else if((digitalRead(SWITCH_INPIN8) == HIGH) && (setflag8 == 1))
   {  
     if(digitalRead(Relay8) == HIGH)
      {
       relayflag8 = 1;
      }
      else
      {
        relayflag8 = 0;
      }
        setflag8 = 0;
   }
//////////////////////////////////////////////////////////////////////////////////
 
 if (relayflag1 == 0)
    {
     Serial.println("POWER1 ON");   
     relayflag1 =2;
    }
  else if (relayflag1 == 1)
    {
      Serial.println("POWER1 OFF");     
      relayflag1 =2;
    }
   
 if (relayflag2 == 0)
    {
     Serial.println("POWER2 ON");     
     relayflag2 = 2;
    }
  else if (relayflag2 == 1)
    {
      Serial.println("POWER2 OFF");      
      relayflag2 = 2;
    }

 if (relayflag3 == 0)
    {
     Serial.println("POWER3 ON");     
     relayflag3 = 2;
    }
  else if (relayflag3 == 1)
    {
      Serial.println("POWER3 OFF");      
      relayflag3 = 2;
    }

 if (relayflag4 == 0)
    {
     Serial.println("POWER4 ON");     
     relayflag4 = 2;
    }
  else if (relayflag4 == 1)
    {
      Serial.println("POWER4 OFF");      
      relayflag4 = 2;
    }

 if (relayflag5 == 0)
    {
     Serial.println("POWER5 ON");    
     relayflag5 = 2;
    }
  else if (relayflag5 == 1)
    {
      Serial.println("POWER5 OFF");      
      relayflag5 = 2;
    }

 if (relayflag6 == 0)
    {
     Serial.println("POWER6 ON");     
     relayflag6 = 2;
    }
  else if (relayflag6 == 1)
    {
      Serial.println("POWER6 OFF");      
      relayflag6 = 2;
    }

 if (relayflag7 == 0)
    {
     Serial.println("POWER7 ON");     
     relayflag7 = 2;
    }
  else if (relayflag7 == 1)
    {
      Serial.println("POWER7 OFF");      
      relayflag7 = 2;
    }

 if (relayflag8 == 0)
    {
     Serial.println("POWER8 ON");   
     relayflag8 = 2;
    }
  else if (relayflag8 == 1)
    {
      Serial.println("POWER8 OFF");      
      relayflag8 = 2;
    }

}
