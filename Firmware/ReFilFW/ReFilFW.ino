//#include "PIDController.h"
//#include "FilamentSensor.h"
#include "EEPROMManager.h"
//#include "Spooler.h"
//#include "TrapezoidProfiler.h"
//#include "Extruder.h"
#include "ExtrusionController.h"

#define SERIAL_BAUDRATE 115200

int dutyCycle = 0;

void setup() {
  thermalSafteyHasInit = false;
  Serial.begin(SERIAL_BAUDRATE);  
  //initPIDController();
  initThermalSaftey();
  dutyCycle = 0;
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
  //dutyCycle = PIDController(HEATER_0_KP, HEATER_0_KI, HEATER_0_KD, target, getThermistorValue(0), 50);

  Serial.print("Thermistor:");
  Serial.print(getThermistorValue(0));
  Serial.print(",");
  Serial.print("Output:");
  Serial.print(dutyCycle);
  Serial.print(",");
  Serial.print("Target:");
  Serial.println(target);
  //analogWrite(13, 255);
  Serial.print("saftey thresh:");
  Serial.println(heaterFaultLevel[0]);
  if(checkThermalSaftey(dutyCycle, getThermistorValue(0), 0) == true) {
    //Serial.print("TRIP");
  } else {

  }

  if(checkThermalSaftey(dutyCycle, getThermistorValue(0), 0) == true) {
    analogWrite(13, 0);
    Serial.print("TRIP!!!");
  } else {
    analogWrite(13, dutyCycle);
  }
  
}
