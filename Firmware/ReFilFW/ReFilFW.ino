#include "Thermistors.h"
#include "PIDController.h"
#include "FilamentSensor.h"
#include "EEPROMManager.h"
#include "Spooler.h"

//Heater 0 constants
#define HEATER_0_KP 3
#define HEATER_0_KI 100
#define HEATER_0_KD 0.0

//Heater 1 constants
#define HEATER_1_KP HEATER_0_KP
#define HEATER_1_KI HEATER_0_KI
#define HEATER_1_KD HEATER_0_KD

//Heater 2 constants
#define HEATER_2_KP HEATER_0_KP
#define HEATER_2_KI HEATER_0_KI
#define HEATER_2_KD HEATER_0_KD

//Heater 3 constants
#define HEATER_3_KP HEATER_0_KP
#define HEATER_3_KI HEATER_0_KI
#define HEATER_3_KD HEATER_0_KD

#define SERIAL_BAUDRATE 115200

int dutyCycle = 0;

void setup() {
  thermalSafteyHasInit = false;
  Serial.begin(SERIAL_BAUDRATE);  
  initPIDController();
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
  dutyCycle = PIDController(HEATER_0_KP, HEATER_0_KI, HEATER_0_KD, target, getThermistorValue(0), 50);

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
