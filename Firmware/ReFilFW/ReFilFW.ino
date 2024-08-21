#include <MobaTools.h>

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

// const byte stepPin = 54;
// const byte dirPin = 55;

// const int stepsPerRev = 200;    // Steps per revolution - may need to be adjusted

//MoToStepper stepper1( stepsPerRev, STEPDIR );  // create a stepper instance

void setup() {
  initLCD();
  thermalSafteyHasInit = false;
  Serial.begin(SERIAL_BAUDRATE);  
  //initPIDController();
  initThermalSaftey();
  heater0DutyCycle = 0;
  heater1DutyCycle = 0;
  initHeaters();
  initSpooler();
  initPuller();
  homeGantry();

  //moveSpoolMotor(60, true);

  //   pinMode(38, OUTPUT);
  // digitalWrite(38, LOW);
  // stepper1.attach( stepPin, dirPin );
  // stepper1.setSpeed( 300 );              // 30 rev/min (if stepsPerRev is set correctly)
  // stepper1.setRampLen( stepsPerRev / 2); // Ramp length is 1/2 revolution
  // stepper1.rotate(1);                    // start turning, 1=vorward, -1=backwards         
}

void loop() {
  updateMenuFromButtons();
  
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
    if(lcdControlHeaters) {
      lcdHome(0, getThermistorValue(0), 100, target);
    } else {
      lcdHome(0, getThermistorValue(0), 100, 0);
    }
  } else {
    updateLEDs(1);
    if(lcdControlHeaters) {
      lcdHome(2, getThermistorValue(0), 100, target);
    } else {
      lcdHome(2, getThermistorValue(0), 100, 0);
    }
  }

  // // if((checkThermalSaftey(heater0DutyCycle, getThermistorValue(0), 0) == true) || (checkThermalSaftey(heater1DutyCycle, getThermistorValue(1), 1) == true)) {
  // //   killHeaters();

  // // } else {
  //   Serial.print("Target: ");
  //   Serial.println(target);
  //   Serial.print("T0: ");
  //   Serial.print(getThermistorValue(0));
  //    Serial.print("T1: "); 
  //   Serial.println(getThermistorValue(1));   
  //   //setHeater(1, target);   
  //   setHeater(0, target);
  // spinDriveMotor(1);
  // if(getThermistorValue(0) >= (target-10)) {
  //   if(!startSoakTime) {
  //     soakTimeStart = millis();
  //     startSoakTime = true;
  //   } else {
  //     if(millis() > (soakTimeStart + soakTime)) {
  //       //spinDriveMotor(17);
  //     }
  //   }
  // } else {
  //   startSoakTime = false;
  // }

  if(lcdControlAutoExtrusion) {
    setFans(true);
    moveSpoolToPosition(20, getSpoolRotationDistance(1000, 60, GANTRY_MIN_POS, GANTRY_MAX_POS));
    moveGantryToPosition(20, getGantryAlignmentPosition(GANTRY_MIN_POS, GANTRY_MAX_POS));
    movePullerMotor(10);

    Serial.println(getGantryAlignmentPosition(GANTRY_MIN_POS, GANTRY_MAX_POS));

  } else {
    if(lcdControlGantry) {
      moveGantry(20, 1);
    } else {
      moveGantry(20, 0);
    }

    if(lcdControlPuller) {
      movePullerMotor(5);
    } else {
      movePullerMotor(0);
    }

    if(lcdControlSpool) {
      moveSpoolMotor(5, true);
    } else {
      moveSpoolMotor(0, true);
    }
    // if(getGantryEndstop()) {
    //   spinGantryMotor(0);
    // } else {
    //   spinGantryMotor(5);
    // }

    setFans(lcdControlFans);
  }
  


  //delay(100);
}
