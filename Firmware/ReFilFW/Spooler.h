#include <MobaTools.h> //stepper control via interval

#include "Arduino.h"
#define SPOOL_MOTOR_STEPS_PER_ROT 200*16*4*2.5*2
#define GANTRY_MOTOR_STEPS_PER_ROT 200 * 2

#define GANTRY_ENDSTOP_PIN 3

#define TARGET_FILAMENT_DIAMETER 1.75

#define SPOOL_MOTOR_STEP_PIN 36
#define SPOOL_MOTOR_DIR_PIN 34
#define SPOOL_MOTOR_EN_PIN 30

#define GANTRY_MOTOR_STEP_PIN 26
#define GANTRY_MOTOR_DIR_PIN 28
#define GANTRY_MOTOR_EN_PIN 24

#define GANTRY_MIN_POS 100
#define GANTRY_MAX_POS 4750

static unsigned long currentTime;
static unsigned long previousTime;
static unsigned long deltaTime;

static boolean stepState = false;

bool gantryDir = false;

int spoolStartingDiameter = 83;
int spoolEndDiameter = 200;
int spoolWidth = 60;
int windingsPerLayer = spoolWidth / TARGET_FILAMENT_DIAMETER;
 

double spoolDefaultTangentalRatio = spoolStartingDiameter*PI;
double pullerDefaultTangentalRatio = 6.5*PI;
double spoolerToPullerRatio =  (spoolDefaultTangentalRatio/pullerDefaultTangentalRatio) * 10;//4.5;
double spoolerToGantryRatio = TARGET_FILAMENT_DIAMETER / ((GANTRY_MAX_POS - GANTRY_MIN_POS) / spoolWidth); //1 rev = fil_dia travel / (end-start)

unsigned long currentSpoolMicros = micros();
unsigned long previousSpoolMicros = micros();


bool spoolStepState = false;

bool gantryHomed = false;
bool gantryHomePreTrig = false;

double rotationCount = 0;
double subRotationCount = 0;

long spoolCurrentPosition = 0;


unsigned long gantryHomePreviousTime = 0;
int gantryHomeBounceTime = 1500;
boolean spoolWindingDirectionInvert = false; //default to back --> front

int spoolBaseRotTime = 12;
int timeForFullLayer = spoolBaseRotTime * windingsPerLayer;

MoToStepper spoolStepper((SPOOL_MOTOR_STEPS_PER_ROT), STEPDIR);
MoToStepper gantryStepper((GANTRY_MOTOR_STEPS_PER_ROT * 16), STEPDIR);

/**
 * Method to be executed before each new roll, and on first startup
 */
void initSpooler() {
  pinMode(GANTRY_ENDSTOP_PIN, INPUT);

  pinMode(SPOOL_MOTOR_EN_PIN, OUTPUT);
  spoolStepper.attach(SPOOL_MOTOR_STEP_PIN, SPOOL_MOTOR_DIR_PIN);
  spoolStepper.setRampLen(1);

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

  currentSpoolMicros = micros();
  previousSpoolMicros = micros();
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
static long getSpoolRotationDistance(double desiredPullSpeed, double spoolStartDiameter, double spoolStartPos, double spoolEndPos) {
  int spoolEffectiveWindingsPerLayer = 20;//int((spoolEndPos - spoolStartPos) / TARGET_FILAMENT_DIAMETER);
  //calculate the effective diameter of the spool based on num of layers, remembering to add 1/2 of the diameter for the center of the filament strand.
  double spoolEffectiveDiameter = spoolStartDiameter + (rotationCount * TARGET_FILAMENT_DIAMETER) + (TARGET_FILAMENT_DIAMETER/2);
  //calculate the effective circumference of one layer of the spool based on effective diameter
  double spoolEffectiveCircumference = spoolEffectiveDiameter * PI;
  unsigned long timeSinceLastUpdate = currentSpoolMicros - previousSpoolMicros;

  double requiredRotationDistance = desiredPullSpeed * (timeSinceLastUpdate/1000/1000) ; //caclulate requried extrusion ammount
  double requiredRotationSteps = requiredRotationDistance/spoolEffectiveCircumference * SPOOL_MOTOR_STEPS_PER_ROT;

  // Serial.print("Windings per layer ");
  // Serial.println(spoolEffectiveWindingsPerLayer);
  // Serial.print("Effective diameter ");
  // Serial.println(spoolEffectiveDiameter);

  // Serial.print("Effective circumference ");
  // Serial.println(spoolEffectiveCircumference);

  // Serial.print("Rot distance ");
  // Serial.println(requiredRotationDistance);

  Serial.print("Rot steps ");
  Serial.println(requiredRotationSteps);



  if(subRotationCount >= spoolEffectiveWindingsPerLayer) {
    rotationCount += 1;
    spoolWindingDirectionInvert = !spoolWindingDirectionInvert;
    subRotationCount =  0;//subRotationCount + requiredRotationDistance/spoolEffectiveCircumference - spoolEffectiveWindingsPerLayer;
  } else {
    subRotationCount += requiredRotationDistance/spoolEffectiveCircumference;
  }
  Serial.print("spool pos ");
  Serial.println(spoolCurrentPosition);
  currentSpoolMicros = micros();
  previousSpoolMicros = currentSpoolMicros;

  //return (spoolCurrentPosition += requiredRotationSteps);
  return requiredRotationSteps;
}

void calculatePullerSpeed(double currentDiameter, double targetDiameter) {

  return 0;
}

/**
 * Returns the required gantry position
 * NOTE: Requies getSpoolRotationDistance() to be executed first to update values!!!!!!!
 */
static double getGantryAlignmentPosition(double spoolStartPos, double spoolEndPos) {
  int spoolEffectiveWindingsPerLayer = 20;//int((spoolEndPos - spoolStartPos) / 1.75);//TARGET_FILAMENT_DIAMETER;
  int spoolWidth = spoolEndPos - spoolStartPos;
  double stepsPerFilWinding = spoolWidth/spoolEffectiveWindingsPerLayer;
  double requiredMovementDistance = stepsPerFilWinding * subRotationCount;

  if(spoolWindingDirectionInvert) {
    return spoolStartPos + requiredMovementDistance;
  } else {
    return spoolEndPos - requiredMovementDistance;
  }
}

static void moveSpoolMotor(double rpm, bool dir) {
  digitalWrite(SPOOL_MOTOR_EN_PIN, LOW);
  spoolStepper.setSpeed(( rpm));//*10*(5/12));
  if(dir) {
    spoolStepper.rotate(1);
  } else {
    spoolStepper.rotate(-1);
  }
  
} 

static void moveSpoolToPosition(double velocity, int position) {
  spoolStepper.setSpeed(velocity*10);
  spoolStepper.move(position);
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

  gantryStepper.setSpeed(velocity  * 10);
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

void moveGantryToPosition(double velocity, int position) {
  gantryStepper.setSpeed(velocity*1);
  gantryStepper.moveTo(-position);

}
