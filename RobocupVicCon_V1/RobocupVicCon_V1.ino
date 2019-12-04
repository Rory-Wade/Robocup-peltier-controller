#include <avr/wdt.h>
#include "peltier.h"

#define channelL 5
#define ambientTher A0

//9:51 new batch

Peltier channels[channelL] = {
 Peltier(5,A5,12),
 Peltier(6,A4,8),
 Peltier(9,A3,7),
 Peltier(10,A2,4),
 Peltier(11,A1,2)
};

//Peltier(5,A5,12) //1
//Peltier(6,A4,8) //2
//Peltier(9,A3,7) //3
//Peltier(10,A2,4) //4
//Peltier(11,A1,2) //5

bool flashLoop = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for(int i = 0; i < channelL; i++){
    channels[i].setup(i);
  }

  pinMode(13,OUTPUT);
  
  Serial.println(("\n\n\t\t------------- PELTIER CONTROLLER -------------\n"));
  Serial.println(("\t\t\t\t Version 0.0.6"));
  Serial.println(("\t\t\t\tSend '?' For Help\n"));
  Serial.println(("\t\t-------------- Rory & Phil Wade --------------\n\n"));
}

void loop() {
  // put your main code here, to run repeatedly:
  bool allGood = true;
  double ambientTemperature = thermistor(analogRead(ambientTher));

  
  Serial.print("GTV > {\tA:");
  Serial.print(ambientTemperature);
  
  for(int i = 0; i < channelL; i++){
    bool curGoodStat = channels[i].goodStatus();
    
    Serial.print(";\t");
    
    if(curGoodStat){
      channels[i].loop(ambientTemperature);
    }else{
      channels[i].off();
      digitalWrite(13, HIGH);
    }
   
    Serial.print(i);
    Serial.print(":");
    Serial.print(channels[i].getTemp());
    
    if(!curGoodStat){
      Serial.print("-E");
    }
    
    allGood = allGood && curGoodStat;
    delay(98);
  }

  Serial.println("\t}");

  if(flashLoop)
    digitalWrite(13, HIGH);
    
  flashLoop = !flashLoop;
  delay(10);
  
  if(allGood){
    digitalWrite(13, LOW);
  }else{
    Serial.println("Device ERROR!");
  }
  
  serialHandle();
}

void allOff(){
  for(int i = 0; i < channelL; i++){
    channels[i].off();
  }
}

double thermistor(int RawADC) {
    double temp;
    temp = log(10000.0*((1024.0/RawADC-1))); 
    temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    temp = temp - 273.15;
    return temp;
}

void serialHandle(){
  if(Serial.available() > 3){
    Serial.print("\n\n\n");
    
    char a = Serial.read();
    int id = (int)a - 48;
        
    bool validID = (id >= 0 && id <= channelL);
    
    char method = Serial.read();

    a = Serial.read();
    int quantity = (int)a - 48;
    
    bool validQuantity = (quantity >= 0 && quantity < 20);

    Serial.print(id);
    Serial.print(method);
    Serial.print(quantity);
      
    if(validID && validQuantity){
      switch(method){
        case '+':
          if(id == channelL){
            for(int i = 0; i < channelL; i++){
              channels[i].setOffset(channels[i].getOffset() + quantity);
            }
          }else{
            channels[id].setOffset(channels[id].getOffset() + quantity);
          }
          
        break;
        case '-':
        if(id == channelL){
            for(int i = 0; i < channelL; i++){
              channels[i].setOffset(channels[i].getOffset() - quantity);
            }
          }else{
            channels[id].setOffset(channels[id].getOffset() - quantity);
          }
        break;
        case '=':
        if(id == channelL){
            for(int i = 0; i < channelL; i++){
              channels[i].setOffset(quantity);
            }
          }else{
            channels[id].setOffset(quantity);
          }
        break;
        default:
          Serial.println("\nINVALID METHOD\n");
        break;
      }
      
    }else{
      
      Serial.println("INVALID");
    }
    
  Serial.print("\n\n\n");
  }else if(Serial.available() > 0){
     char cmd = Serial.read();
     
       switch(cmd){
        case '?':
          Serial.println(("\n\t----------- HELP ------------"));
          Serial.println(("\t?\tHELP"));
          Serial.println(("\tG\tGet All Offsets"));
          Serial.println(("\tR\tReset Offsets"));
          Serial.println(("\t0+1\t[Set peltier 0 to 4 offset or use 5 to set all] (+/-/=) [Quantity to add, subtract or equal]\n\n"));
        break;
        case 'G':
          Serial.print("\n\noffsets");
          
          for(int i = 0; i < channelL; i++){
            Serial.print(" > ");
            Serial.print(i);
            Serial.print(":");
            Serial.print(channels[i].getOffset());
          }
          Serial.println("\n");
          
        break;
        case 'R':
        
          
          Serial.println("\n\nALL OFFSETS RESET\n\n");
          allOff();

          for(int i = 0; i < channelL; i++){
            channels[i].setOffset(0);
          }
          
          break;
        
        
        default:
          Serial.print("INVALID METHOD");
        break;
      }
  }else{
    
  }
  
  serialFlush();
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}  
 
void software_reset()
{
  wdt_reset(); 
  wdt_enable(WDTO_250MS);
  exit (1);  // loop forever
}


