boolean connectMQTT(){
  if (mqttClient.connected()){
    return true;
  }  
  
  Serial.print("Connecting to MQTT server ");
  Serial.print(mqttServer);
  Serial.print(" as ");
  Serial.println(host);
  
  if (mqttClient.connect(host)) {
    Serial.println("Connected to MQTT broker");
    if(mqttClient.subscribe((char*)subTopic.c_str())){
      Serial.println("Subsribed to topic.");
    } else {
      Serial.println("NOT subsribed to topic!");      
    }
    return true;
  }
  else {
    Serial.println("MQTT connect failed! ");
    return false;
  }
}

void disconnectMQTT(){
  mqttClient.disconnect();
}

void mqtt_handler(){
  if (toPub==1){
    Debugln("DEBUG: Publishing state via MWTT");
    if(pubState()){
     toPub=0; 
    }
  }
  mqttClient.loop();
  delay(100); //let things happen in background
}

void mqtt_arrived(char* subTopic, byte* payload, unsigned int length) { // handle messages arrived 
  int i = 0;
  //Serial.print("MQTT message arrived:  topic: " + String(subTopic));
    // create character buffer with ending null terminator (string)
    
  for(i=0; i<length; i++) {    
    buf[i] = payload[i];
  }
  buf[i] = '\0';
  String msgString = String(buf);
  //Serial.println(" message: " + msgString);
  if (msgString == "R1_ON") //Relay1
  {
      Serial.println("R_1 switched via web request to 1");

  }
    else if (msgString == "R1_OFF")
    {
      Serial.println("R_1 switched via web request to 0");
    }
    
  if (msgString == "R2_ON")//Relay2
  {
      Serial.println("R_2 switched via web request to 1");
  } 
  else if (msgString == "R2_OFF")
  {
     Serial.println("R_2 switched via web request to 0"); 
  }   
       
  if (msgString == "R3_ON")//Relay3
  {
  Serial.println("R_3 switched via web request to 1");
  } 
  else if (msgString == "R3_OFF")
  {
      Serial.println("R_3 switched via web request to 0");
  }  
  
    if (msgString == "R4_ON")//Relay4
    {
      Serial.println("R_4 switched via web request to 1");
  } 
  else if (msgString == "R4_OFF")
  {
      Serial.println("R_4 switched via web request to 0");
  }
   
     if (msgString == "R5_ON") //Relay5
  {
      Serial.println("R_5 switched via web request to 1");

  }
      
     else if (msgString == "R5_OFF")
    {
     Serial.println("R_5 switched via web request to 0");

    }
    
  if (msgString == "R6_ON")//Relay6
  {
       Serial.println("R_6 switched via web request to 1");
 
  } 
  else if (msgString == "R6_OFF")
  {
       Serial.println("R_6 switched via web request to 0");
  }

  if (msgString == "R7_ON") //Relay7
  {
       Serial.println("R_7 switched via web request to 1");

  } 
  else if (msgString == "R7_OFF")
  {
     Serial.println("R_7 switched via web request to 0");
 
  } 
   
    if (msgString == "R8_ON")//Relay8
    {
       Serial.println("R_8 switched via web request to 1");

  } 
  else if (msgString == "R8_OFF")
  {
       Serial.println("R_8 switched via web request to 0");

  }       
}

boolean pubState(){ //Publish the current state of the light    
  if (!connectMQTT()){
      delay(100);
      if (!connectMQTT){                            
        Serial.println("Could not connect MQTT.");
        Serial.println("Publish state NOK");
        return false;
      }
    }
    if (mqttClient.connected()){      
        //String state = (digitalRead(OUTPIN))?"1":"0";
        Serial.println("To publish state " + state );  
      if (mqttClient.publish((char*)pubTopic.c_str(), (char*) state.c_str())) {
        Serial.println("Publish state OK");        
        return true;
      } else {
        Serial.println("Publish state NOK");        
        return false;
      }
     } else {
         Serial.println("Publish state NOK");
         Serial.println("No MQTT connection.");        
     }    
}
