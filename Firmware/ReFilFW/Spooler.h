#define SPOOL_MOTOR_STEPS_PER_ROT 200
#define GANTRY_MOTOR_STEPS_PER_ROT 200

#define GANTRY_ENDSTOP_PIN 20

#define TARGET_FILAMENT_DIAMETER 1.75

#define SPOOL_MOTOR_STEP_PIN 4
#define SPOOL_MOTOR_DIR_PIN 5
#define SPOOL_MOTOR_EN_PIN 6

static unsigned long currentTime;
static unsigned long previousTime;
static unsigned long deltaTime;

static boolean stepState = false;

int spoolStartingDiameter = 60;
int spoolEndDiameter = 200;
int spoolWidth = 60;

float rotationCount = 0;
float subRotationCount = 0;
boolean spoolWindingDirectionInvert = false; //default to back --> front
/**
 * Method to be executed before each new roll, and on first startup
 */
void initSpooler() {
  currentTime = 0;
  previousTime = 0;
  deltaTime = 0;
  rotationCount = 0;
  subRotationCount = 0;

  pinMode(SPOOL_MOTOR_STEP_PIN, OUTPUT);
  pinMode(SPOOL_MOTOR_DIR_PIN, OUTPUT);
  pinMode(SPOOL_MOTOR_EN_PIN, OUTPUT);

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

static void moveSpoolMotor(double rpm, boolean dir) {
  unsigned long previousMicros = micros();
  unsigned long desiredPulseMicros = ((60*1000*1000)/rpm); //converting rpm to min/r, then converting to sec, ms, us...
  digitalWrite(SPOOL_MOTOR_EN_PIN, HIGH); //pull enable pin high to enguage the driver

  if(SPOOL_MOTOR_DIR_PIN) { //set dir pin
    digitalWrite(SPOOL_MOTOR_DIR_PIN, HIGH);
  } else {
    digitalWrite(SPOOL_MOTOR_DIR_PIN, LOW);
  }

  if((micros() - previousMicros) >= (desiredPulseMicros/2)) {
    previousMicros = micros();
    digitalWrite(SPOOL_MOTOR_STEP_PIN, HIGH);
    if((micros() - previousMicros) >= (desiredPulseMicros/2)) {
      previousMicros = micros();
      digitalWrite(SPOOL_MOTOR_STEP_PIN, LOW);
    }
  }
} 

static boolean moveStepperAtVelocity(double velocity) {
  currentTime = micros();
  deltaTime = previousTime - currentTime;
  unsigned long stepInterval = 1000000 / velocity; // Convert to microseconds
  if(deltaTime >= stepInterval) {
    stepState = !stepState;
  }
  previousTime = micros();
  return stepState;
}
