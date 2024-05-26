#include "Thermistors.h"
#include "PIDController.h"

#define DRIVE_MOTOR_VP_PIN 8
#define DRIVE_MOTOR_RPM 45 //rpm


#define SCREW_PITCH 50 //mm
#define SCREW_CROSSSECTIONAL_AREA 380.1327111

#define FAN_PIN 60


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
      analogWrite(
        HEATER_0_PIN, m_pid0.callPIDController0(
          setTemp, 
          getThermistorValue(0)
        )
      );
    if(getThermistorValue(0) >= setTemp) {
      return true;
    }
    break;
    case 1:
      analogWrite(
        HEATER_1_PIN, m_pid1.callPIDController1(
          setTemp, 
          getThermistorValue(1)
        )
      );
    if(getThermistorValue(1) >= setTemp) {
      return true;
    }
    break;
    case 2:
      analogWrite(
        HEATER_2_PIN, m_pid2.callPIDController2(
          setTemp, 
          getThermistorValue(2)
        )
      );
    if(getThermistorValue(2) >= setTemp) {
      return true;
    }
    break;
    case 3:
      analogWrite(
        HEATER_3_PIN, m_pid3.callPIDController3(
          setTemp, 
          getThermistorValue(3)
        )
      );
    if(getThermistorValue(3) >= setTemp) {
      return true;
    }
    break;
  }
  return false;
}

void killHeaters() {
  analogWrite(HEATER_0_PIN, 0);
  analogWrite(HEATER_1_PIN, 0);
  analogWrite(HEATER_2_PIN, 0);
  analogWrite(HEATER_3_PIN, 0);
}

void setFans(float dutyCycle) {
  analogWrite(FAN_PIN, dutyCycle);
}

double convertVolumeToRPM(double volume) {
  return (volume / (SCREW_PITCH * SCREW_CROSSSECTIONAL_AREA));
}
