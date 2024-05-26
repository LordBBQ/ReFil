//#include "PIDController.h"
//#include "FilamentSensor.h"
#include "EEPROMManager.h"
//#include "Spooler.h"
//#include "TrapezoidProfiler.h"
//#include "Extruder.h"
#include "ExtrusionController.h"

#define SERIAL_BAUDRATE 115200

int heater0DutyCycle = 0;
int heater1DutyCycle = 0;

void setup() {
  thermalSafteyHasInit = false;
  Serial.begin(SERIAL_BAUDRATE);  
  //initPIDController();
  initThermalSaftey();
  heater0DutyCycle = 0;
  heater1DutyCycle = 0;
}

void loop() {
  double target = 240;
  // put your main code here, to run repeatedly:
//  double thermistorVoltage = ((analogRead(A7) / 1024.0) * 5.0);
//  double R2 = (thermistorVoltage * R1) / (VIN - (thermistorVoltage));
//  Serial.print(thermistorVoltage);
//  Serial.println(" Volts");
//  Serial.print(R2);
//  Serial.println(" Ohms");
//  delay(1000);


  Serial.print("Thermistor:");
  Serial.print(getThermistorValue(0));
  Serial.print(",");
  Serial.print("Output:");
  Serial.print(heater0DutyCycle);
  Serial.print(",");
  Serial.print("Target:");
  Serial.println(target);
  //analogWrite(13, 255);
  Serial.print("saftey thresh:");
  Serial.println(heaterFaultLevel[0]);
  if((checkThermalSaftey(heater0DutyCycle, getThermistorValue(0), 0) == true) || (checkThermalSaftey(heater1DutyCycle, getThermistorValue(1), 1) == true)) {
    //Serial.print("TRIP");
  } else {

  }

  if((checkThermalSaftey(heater0DutyCycle, getThermistorValue(0), 0) == true) || (checkThermalSaftey(heater1DutyCycle, getThermistorValue(1), 1) == true)) {
    killHeaters();

  } else {
    setHeater(0, target);
    setHeater(1, target);   
  }
  
}
