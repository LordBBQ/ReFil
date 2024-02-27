#define SPOOL_MOTOR_STEPS_PER_ROT 100

#define GANTRY_ENDSTOP_PIN 20

#define TARGET_FILAMENT_DIAMETER 1.75

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
  rotationCount = 0;
  subRotationCount = 0;
}

/**
 * Calculates the desired rotation based on how much filament has been spooled.
 */
static double getSpoolRotationDistance(double desiredPullDistance, double spoolStartDiameter, double spoolStartPos, double spoolEndPos) {
  int spoolEffectiveWindingsPerLayer = (spoolEndPos - spoolStartPos) / TARGET_FILAMENT_DIAMETER;
  //calculate the effective diameter of the spool based on num of layers, remembering to add 1/2 of the diameter for the center of the filament strand.
  double spoolEffectiveDiameter = spoolStartDiameter + (rotationCount / spoolEffectiveWindingsPerLayer * TARGET_FILAMENT_DIAMETER + TARGET_FILAMENT_DIAMETER/2);
  //calculate the effective circumference of one layer of the spool based on effective diameter
  double spoolEffectiveCircumference = spoolEffectiveDiameter * PI;
  double requiredRotationDistance = spoolEffectiveCircumference / desiredPullDistance; //caclulate requried rotation
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
