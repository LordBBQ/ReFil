#include <MobaTools.h> //stepper control via interval

#include "Arduino.h"
#define SPOOL_MOTOR_STEPS_PER_ROT 200
#define GANTRY_MOTOR_STEPS_PER_ROT 200

#define GANTRY_ENDSTOP_PIN 3

#define TARGET_FILAMENT_DIAMETER 1.75

#define SPOOL_MOTOR_STEP_PIN 36
#define SPOOL_MOTOR_DIR_PIN 34
#define SPOOL_MOTOR_EN_PIN 30

#define GANTRY_MOTOR_STEP_PIN 26
#define GANTRY_MOTOR_DIR_PIN 28
#define GANTRY_MOTOR_EN_PIN 24

#define GANTRY_MIN_POS 100
#define GANTRY_MAX_POS 2000

static unsigned long currentTime;
static unsigned long previousTime;
static unsigned long deltaTime;

static boolean stepState = false;

bool gantryDir = false;

int spoolStartingDiameter = 60;
int spoolEndDiameter = 200;
int spoolWidth = 60;
unsigned long previousSpoolMicros = micros();


bool spoolStepState = false;

bool gantryHomed = false;
bool gantryHomePreTrig = false;

float rotationCount = 0;
float subRotationCount = 0;

unsigned long gantryHomePreviousTime = 0;
int gantryHomeBounceTime = 1500;
boolean spoolWindingDirectionInvert = false; //default to back --> front

MoToStepper spoolStepper((SPOOL_MOTOR_STEPS_PER_ROT * 16*8), STEPDIR);
MoToStepper gantryStepper((SPOOL_MOTOR_STEPS_PER_ROT * 16), STEPDIR);

/**
 * Method to be executed before each new roll, and on first startup
 */
void initSpooler() {
  pinMode(GANTRY_ENDSTOP_PIN, INPUT);

  pinMode(SPOOL_MOTOR_EN_PIN, OUTPUT);
  spoolStepper.attach(SPOOL_MOTOR_STEP_PIN, SPOOL_MOTOR_DIR_PIN);
  spoolStepper.setRampLen((SPOOL_MOTOR_STEPS_PER_ROT)/6);

  pinMode(GANTRY_MOTOR_EN_PIN, OUTPUT);
  gantryStepper.attach(GANTRY_MOTOR_STEP_PIN, GANTRY_MOTOR_DIR_PIN);
  gantryStepper.setRampLen((GANTRY_MOTOR_STEPS_PER_ROT)/24);
  // spoolStepper.rotate(-1);
  //spoolStepper.setSpeed(0);

  currentTime = 0;
  previousTime = 0;
  deltaTime = 0;
  rotationCount = 0;
  subRotationCount = 0;
}

bool getGantryEndstop() {
  if(digitalRead(GANTRY_ENDSTOP_PIN) == HIGH) {
    return false;
  } else {
    return true;
  }
}

/**
 * Calculates the desired rotation based on how much filament has been spooled.
 */
static double getSpoolRotationDistance(double desiredPullSpeed, double spoolStartDiameter, double spoolStartPos, double spoolEndPos) {
  int spoolEffectiveWindingsPerLayer = (spoolEndPos - spoolStartPos) / TARGET_FILAMENT_DIAMETER;
  //calculate the effective diameter of the spool based on num of layers, remembering to add 1/2 of the diameter for the center of the filament strand.
  double spoolEffectiveDiameter = spoolStartDiameter + (rotationCount / spoolEffectiveWindingsPerLayer * TARGET_FILAMENT_DIAMETER + TARGET_FILAMENT_DIAMETER/2);
  //calculate the effective circumference of one layer of the spool based on effective diameter
  double spoolEffectiveCircumference = spoolEffectiveDiameter * PI;
  double requiredRotationDistance = spoolEffectiveCircumference / desiredPullSpeed; //caclulate requried rotation
  if(subRotationCount >= spoolEffectiveWindingsPerLayer) {
    rotationCount += 1;
    spoolWindingDirectionInvert = !spoolWindingDirectionInvert;
    subRotationCount =  subRotationCount + requiredRotationDistance - spoolEffectiveWindingsPerLayer;
  } else {
    subRotationCount += requiredRotationDistance;
  }
  return requiredRotationDistance;
}

/**
 * Returns the required gantry position
 * NOTE: Requies getSpoolRotationDistance() to be executed first to update values!!!!!!!
 */
static double getGantryAlignmentPosition(double spoolStartPos, double spoolEndPos) {
  int spoolEffectiveWindingsPerLayer = (spoolEndPos - spoolStartPos) / TARGET_FILAMENT_DIAMETER;
  if(spoolWindingDirectionInvert) {
    return spoolStartPos + ((spoolEndPos - spoolStartPos)/spoolEffectiveWindingsPerLayer * subRotationCount);
  } else {
    return spoolEndPos - ((spoolEndPos - spoolStartPos)/spoolEffectiveWindingsPerLayer * subRotationCount);
  }
}

static void moveSpoolMotor(double rpm, bool dir) {
  digitalWrite(SPOOL_MOTOR_EN_PIN, LOW);
  spoolStepper.setSpeed(rpm*10);
  if(dir) {
    spoolStepper.rotate(1);
  } else {
    spoolStepper.rotate(-1);
  }
  
} 

static void spinGantryMotor(double velocity) {
  digitalWrite(GANTRY_MOTOR_EN_PIN, LOW);
  gantryStepper.setSpeed(abs(velocity*10));
  if(velocity < 0) {
    gantryStepper.rotate(1);
  } else {
    gantryStepper.rotate(-1);

  }
  // currentTime = micros();
  // deltaTime = previousTime - currentTime;
  // unsigned long stepInterval = 1000000 / velocity; // Convert to microseconds
  // if(deltaTime >= stepInterval) {
  //   stepState = !stepState;
  // }
  // previousTime = micros();
  // digitalWrite(GANTRY_MOTOR_EN_PIN, HIGH); //enable the motor

  // if(velocity < 0) {
  //   digitalWrite(GANTRY_MOTOR_DIR_PIN, HIGH); //invert dir if neg. velocity
  // } else {
  //   digitalWrite(GANTRY_MOTOR_DIR_PIN, LOW);
  // }

  // if(stepState) {
  //   digitalWrite(GANTRY_MOTOR_STEP_PIN, HIGH);
  // } else {
  //   digitalWrite(GANTRY_MOTOR_STEP_PIN, LOW);
  // }
}

void homeGantry() {
  if(getGantryEndstop()) {
    spinGantryMotor(20);
    delay(1000);
  }
  while(getGantryEndstop() == false) {
    spinGantryMotor(-20);
  }
  spinGantryMotor(0);
  gantryStepper.setZero();
  gantryHomed = true;
}

void homeGantryNoHalt() {
  // if(getGantryEndstop() || gantryHomePreTrig) {
  //   if(getGantryEndstop() && !gantryHomePreTrig) {
  //     gantryHomePreviousTime = millis();
  //   }
  //   gantryHomePreTrig = true;
  //   if((millis() - gantryHomePreviousTime) >= gantryHomeBounceTime) {
  //     spinGantryMotor(20);
  //   } else {
  //     spinGantryMotor(0);
  //     gantryHomePreTrig = false;
  //   }
  // } else {
  //   if(getGantryEndstop()) {
  //     spinGantryMotor(0);
  //     gantryHomed = true;
  //     gantryStepper.setZero();      
  //   } else {
      spinGantryMotor(-20);
  //   }
  // }

}
/**
* 0= home
* 1= osc
*/
void moveGantry(double velocity, byte mode) {

  gantryStepper.setSpeed(velocity*1);
    switch(mode) {
      case 1: //0
          if(!gantryStepper.moving()) {
        if(gantryDir) {
          gantryStepper.moveTo(-GANTRY_MAX_POS);
        } else {
          gantryStepper.moveTo(-GANTRY_MIN_POS);
        }
        gantryDir = !gantryDir;
      }
      break;
    default:
      gantryStepper.moveTo(-GANTRY_MIN_POS);
    break;
  }
  
}
