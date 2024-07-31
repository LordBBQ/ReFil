//#include "PIDController.h"
//#include "FilamentSensor.h"
#include "EEPROMManager.h"
#include "Spooler.h"
//#include "TrapezoidProfiler.h"
//#include "Extruder.h"
#include "ExtrusionController.h"
#include "LCD.h"

#define SERIAL_BAUDRATE 115200

int heater0DutyCycle = 0;
int heater1DutyCycle = 0;

unsigned long soakTimeStart = 0;
int soakTime = 360*1000;
bool startSoakTime = false;

void setup() {
  initLCD();
  thermalSafteyHasInit = false;
  Serial.begin(SERIAL_BAUDRATE);  
  //initPIDController();
  initThermalSaftey();
  heater0DutyCycle = 0;
  heater1DutyCycle = 0;
  initHeaters();
}

void loop() {
  
  double target = 260;
  //spinDriveMotor(25);

  // put your main code here, to run repeatedly:
//  double thermistorVoltage = ((analogRead(A7) / 1024.0) * 5.0);
//  double R2 = (thermistorVoltage * R1) / (VIN - (thermistorVoltage));
//  Serial.print(thermistorVoltage);
//  Serial.println(" Volts");
//  Serial.print(R2);
//  Serial.println(" Ohms");
//  delay(1000);


  // Serial.print("Thermistor:");
  // Serial.print(getThermistorValue(0));
  // Serial.print(",");
  // Serial.print("Output:");
  // Serial.print(heater0DutyCycle);
  // Serial.print(",");
  // Serial.print("Target:");
  // Serial.println(target);
  // //analogWrite(13, 255);
  // Serial.print("saftey thresh:");
  // Serial.println(heaterFaultLevel[0]);
  if((checkThermalSaftey(heater0DutyCycle, getThermistorValue(0), 0) == true) || (checkThermalSaftey(heater1DutyCycle, getThermistorValue(1), 1) == true)) {
    updateLEDs(10);
    lcdHome(0, getThermistorValue(0), 100);
  } else {
    updateLEDs(1);
    lcdHome(2, getThermistorValue(0), 100);
  }

  // if((checkThermalSaftey(heater0DutyCycle, getThermistorValue(0), 0) == true) || (checkThermalSaftey(heater1DutyCycle, getThermistorValue(1), 1) == true)) {
  //   killHeaters();

  // } else {
    Serial.print("Target: ");
    Serial.println(target);
    Serial.print("T0: ");
    Serial.print(getThermistorValue(0));
     Serial.print("T1: "); 
    Serial.println(getThermistorValue(1));   
    //setHeater(1, target);   
    setHeater(0, target);
  spinDriveMotor(1);
  if(getThermistorValue(0) >= (target-10)) {
    if(!startSoakTime) {
      soakTimeStart = millis();
      startSoakTime = true;
    } else {
      if(millis() > (soakTimeStart + soakTime)) {
        //spinDriveMotor(17);
      }
    }
  } else {
    startSoakTime = false;
  }
  moveSpoolMotor(1, true);
  //delay(100);
}
