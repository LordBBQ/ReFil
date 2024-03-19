#include "Thermistors.h"
#include "PIDController.h"

#define DRIVE_MOTOR_VP_PIN 10
#define DRIVE_MOTOR_RPM 45 //rpm

//Heater 0 constants
#define HEATER_0_PIN 20
#define HEATER_0_KP 3
#define HEATER_0_KI 100
#define HEATER_0_KD 0.0

//Heater 1 constants
#define HEATER_1_PIN 21
#define HEATER_1_KP HEATER_0_KP
#define HEATER_1_KI HEATER_0_KI
#define HEATER_1_KD HEATER_0_KD

//Heater 2 constants
#define HEATER_2_PIN 22
#define HEATER_2_KP HEATER_0_KP
#define HEATER_2_KI HEATER_0_KI
#define HEATER_2_KD HEATER_0_KD

//Heater 3 constants
#define HEATER_3_PIN 23
#define HEATER_3_KP HEATER_0_KP
#define HEATER_3_KI HEATER_0_KI
#define HEATER_3_KD HEATER_0_KD

#define SCREW_PITCH 50 //mm
#define SCREW_CROSSSECTIONAL_AREA 380.1327111

#define FAN_PIN 25


void spinDriveMotor(double rpm) {
  float driveOutput = rpm / DRIVE_MOTOR_RPM; //calculate reqd duty cycle of motor based off max rpm
  analogWrite(DRIVE_MOTOR_VP_PIN, driveOutput);
}

bool setHeater(int zone, float setTemp) {
  switch(zone) {
    case 0:
      analogWrite(
        HEATER_0_PIN, PIDTempController0::PIDController(
          HEATER_0_KP, 
          HEATER_0_KI, 
          HEATER_0_KD, 
          setTemp, 
          getThermistorValue(0), 
          50
        )
      );
    if(getThermistorValue(0) >= setTemp) {
      return true;
    }
    break;
    case 1:
      analogWrite(
        HEATER_1_PIN, PIDTempController1::PIDController(
          HEATER_1_KP, 
          HEATER_1_KI, 
          HEATER_1_KD, 
          setTemp, 
          getThermistorValue(1), 
          50
        )
      );
    if(getThermistorValue(1) >= setTemp) {
      return true;
    }
    break;
    case 2:
      analogWrite(
        HEATER_2_PIN, PIDTempController2::PIDController(
          HEATER_2_KP, 
          HEATER_2_KI, 
          HEATER_2_KD, 
          setTemp, 
          getThermistorValue(2), 
          50
        )
      );
    if(getThermistorValue(2) >= setTemp) {
      return true;
    }
    break;
    case 3:
      analogWrite(
        HEATER_3_PIN, PIDTempController3::PIDController(
          HEATER_3_KP, 
          HEATER_3_KI, 
          HEATER_3_KD, 
          setTemp, 
          getThermistorValue(3), 
          50
        )
      );
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