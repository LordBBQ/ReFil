#include <MobaTools.h>


#define PULLER_MOTOR_STEP_PIN 54
#define PULLER_MOTOR_DIR_PIN 55
#define PULLER_MOTOR_EN_PIN 38
#define PULLER_MOTOR_STEPS_PER_ROT 200

#define PULLER_KP 3
#define PULLER_KI 100
#define PULLER_KD 0.0

#define PULLER_MIN_DIAMETER 1
#define PULLER_MAX_DIAMETER 2.5

#define PULLER_DEFAULT_SPEED 1

static unsigned long pullerCurrentTime;
static unsigned long pullerPreviousTime;
static unsigned long pullerDeltaTime;

static boolean pullerStepState = false;

MoToStepper pullerStepper((PULLER_MOTOR_STEPS_PER_ROT * 16), STEPDIR);


/**
 * Method to be executed before each new roll, and on first startup
 */
void initPuller() {
  pullerCurrentTime = 0;
  pullerPreviousTime = 0;
  pullerDeltaTime = 0;

  pinMode(PULLER_MOTOR_EN_PIN, OUTPUT);
  pullerStepper.attach(PULLER_MOTOR_STEP_PIN, PULLER_MOTOR_DIR_PIN);
  pullerStepper.setRampLen(0.1);
}



static void movePullerMotor(double rpm, bool dir) {
  digitalWrite(PULLER_MOTOR_EN_PIN, LOW);
  pullerStepper.setSpeed(10*rpm);
  if(!dir) {
    pullerStepper.rotate(1);

  } else {
    pullerStepper.rotate(-1);   
  }

} 
