#include "Thermistors.h"
#include "PIDController.h"

#define DRIVE_MOTOR_VP_PIN 8
#define DRIVE_MOTOR_RPM 45 //rpm


#define SCREW_PITCH 50 //mm
#define SCREW_CROSSSECTIONAL_AREA 380.1327111

#define FAN_PIN 60

void initExtruder() {
  pinMode(HEATER_0_PIN, OUTPUT);
  pinMode(HEATER_1_PIN, OUTPUT);
  pinMode(HEATER_2_PIN, OUTPUT);
  pinMode(HEATER_3_PIN, OUTPUT);

}

void killHeaters() {
  analogWrite(HEATER_0_PIN, 0);
  analogWrite(HEATER_1_PIN, 0);
  analogWrite(HEATER_2_PIN, 0);
  analogWrite(HEATER_3_PIN, 0);
}

void spinDriveMotor(double rpm) {
  float driveOutput = rpm / DRIVE_MOTOR_RPM; //calculate reqd duty cycle of motor based off max rpm
  analogWrite(DRIVE_MOTOR_VP_PIN, driveOutput);
}

bool setHeater(int zone, float setTemp) {
  PIDController0 m_pid0;
  PIDController1 m_pid1;
  PIDController2 m_pid2;
  PIDController3 m_pid3;
  
  switch(zone) {
    case 0:
      double heater0Output = m_pid0.callPIDController0(
          setTemp, 
          getThermistorValue(0)
      );
      
      if(checkThermalSaftey(heater0Output, getThermistorValue(0), 0)) {
        Serial.println("TRIP0");
        killHeaters();
      } else {
        //Continue with heater heating
        analogWrite(HEATER_0_PIN, heater0Output);
        analogWrite(HEATER_1_PIN, heater0Output);
        Serial.print("H0: ");
        Serial.println(heater0Output);
      }
      //Have we hit temp?
      if(getThermistorValue(0) >= setTemp) {
        return true;
      }
    break;
    case 1:
      double heater1Output = m_pid1.callPIDController1(
          setTemp, 
          getThermistorValue(1)
      );
      
      if(checkThermalSaftey(heater1Output, getThermistorValue(1), 1)) {
        Serial.println("TRIP1");
        killHeaters();
      } else {
        //Continue with heater heating
        analogWrite(HEATER_1_PIN, heater1Output);
        Serial.print("H1: ");
        Serial.println(heater1Output);
      }
      //Have we hit temp?
      if(getThermistorValue(1) >= setTemp) {
        return true;
      }
    break;
    case 2:
      double heater2Output = m_pid2.callPIDController2(
          setTemp, 
          getThermistorValue(2)
      );
      
      if(checkThermalSaftey(heater0Output, getThermistorValue(2), 2)) {
        Serial.println("TRIP2");
        killHeaters();
      } else {
        //Continue with heater heating
        analogWrite(HEATER_2_PIN, heater2Output);
        Serial.print("H2: ");
        Serial.println(heater2Output);
      }
      //Have we hit temp?
      if(getThermistorValue(2) >= setTemp) {
        return true;
      }
    break;
    case 3:
      double heater3Output = m_pid3.callPIDController3(
          setTemp, 
          getThermistorValue(0)
      );
      
      if(checkThermalSaftey(heater3Output, getThermistorValue(3), 3)) {
        Serial.println("TRIP3");
        killHeaters();
      } else {
        //Continue with heater heating
        analogWrite(HEATER_3_PIN, heater3Output);
      }
      //Have we hit temp?
      if(getThermistorValue(3) >= setTemp) {
        return true;
      }
    break;
  }
  return false;
}



void setFans(float dutyCycle) {
  analogWrite(FAN_PIN, dutyCycle);
}

double convertVolumeToRPM(double volume) {
  return (volume / (SCREW_PITCH * SCREW_CROSSSECTIONAL_AREA));
}
