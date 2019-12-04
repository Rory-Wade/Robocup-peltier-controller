#include <EEPROM.h>
#define Kp 5
#define Ki 0.001
#define Kd 0.01
#define Dt 500; //loop delay

#define MAXONTIME 35000
#define LOWPWMVALUE 90

#define TEMPDIFF 12

#define MAX_PWM 100
#define MIN_PWM 0

class Peltier {
    const byte mosfetPin;
    const byte tempPin;
    const byte switchPin;
    byte ID;
    
    double temperature;
    double setPoint;
    double outputVal;
    int tempOffset = 3;
    
    double pre_error;
    double integral;

    unsigned long lastLowTime;
    
  double thermistor(int RawADC) {
    double temp;
    temp = log(10000.0*((1024.0/RawADC-1))); 
    temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    temp = temp - 273.15;
    return temp;
  }

  bool enabled(){
    return !digitalRead(switchPin);
  }

  public:
    Peltier(byte mosfetPinAttach, byte tempPinAttach, byte switchPinAttach) :
        mosfetPin(mosfetPinAttach),
        tempPin(tempPinAttach),
        switchPin(switchPinAttach)
      {
      }
    
    void setup(int setID) {
      pinMode(mosfetPin, OUTPUT);
      pinMode(tempPin, INPUT);
      pinMode(switchPin, INPUT_PULLUP);

      ID = setID; 
      tempOffset = getOffset();
    }

    void off() {
      if(!enabled()){
        lastLowTime = millis();
      }
      
      analogWrite(mosfetPin, 0);
    }

    bool goodStatus(){
      if(!enabled()){
        return true;
      }

      temperature =  thermistor(analogRead(tempPin));
      
      if(temperature > 55 + tempOffset){
        return false;
      }

      if(temperature < 0){
        return false;
      }
      
      if(lastLowTime + MAXONTIME < millis()){
        return false;
      }
      
      return true;
    }
    
    void loop(double ambientTemp) {

      setPoint = ambientTemp + TEMPDIFF + tempOffset;
//      setPoint = 40 + tempOffset;
      
      temperature =  thermistor(analogRead(tempPin));
      
      if(!enabled()){
        off();
        return;
      }
      
      double error = setPoint - temperature; // Proportion
      integral += error * Dt;  // Integral
      double derivative = (error - pre_error) / Dt; // Derivative 
      
      outputVal = Kp * error + Ki * integral + Kd * derivative; // Calculate total output
      
      // Restrict to max/min
      if( outputVal > MAX_PWM )
        outputVal = MAX_PWM;
      else if( outputVal < MIN_PWM )
        outputVal = MIN_PWM;

      pre_error = error;

      if(outputVal < LOWPWMVALUE){
        lastLowTime = millis();
      }
  
      analogWrite(mosfetPin, outputVal);
    }

    double getTemp(){
      return temperature - tempOffset;
    }
    
    double getOffset(){

      if(EEPROM.read((ID)) == 255){
        Serial.print("No Value Set!");
        EEPROM.write((5*ID), tempOffset);
      }
    
      int offset = EEPROM.read((ID)); //Doubles are 4 bytes one NUll inbetween.

      if(offset < 100 && offset > -100){
        return offset;
      }

      return 0;
    }
    
    bool setOffset(double offset){
      EEPROM.write((ID), offset);
      tempOffset = offset;
    }
};

