
#define PULLER_MOTOR_STEP_PIN 101
#define PULLER_MOTOR_DIR_PIN 102
#define PULLER_MOTOR_EN_PIN 103

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

/**
 * Method to be executed before each new roll, and on first startup
 */
void initPuller() {
  pullerCurrentTime = 0;
  pullerPreviousTime = 0;
  pullerDeltaTime = 0;


  pinMode(PULLER_MOTOR_STEP_PIN, OUTPUT);
  pinMode(PULLER_MOTOR_DIR_PIN, OUTPUT);
  pinMode(PULLER_MOTOR_EN_PIN, OUTPUT);

}

void calculatePullerSpeed(double currentDiameter, double targetDiameter) {

  return 0;//return ((255 / PIDDiameterController::PIDController(PULLER_KP, PULLER_KI, PULLER_KD, currentDiameter, targetDiameter, 50)) + PULLER_DEFAULT_SPEED);
}

static void movePullerMotor(double rpm) {
  unsigned long previousMicros = micros();
  unsigned long desiredPulseMicros = ((60*1000*1000)/rpm); //converting rpm to min/r, then converting to sec, ms, us...
  digitalWrite(PULLER_MOTOR_EN_PIN, HIGH); //pull enable pin high to enguage the driver

  if(PULLER_MOTOR_DIR_PIN) { //set dir pin
    digitalWrite(PULLER_MOTOR_DIR_PIN, HIGH);
  } else {
    digitalWrite(PULLER_MOTOR_DIR_PIN, LOW);
  }

  if((micros() - previousMicros) >= (desiredPulseMicros/2)) {
    previousMicros = micros();
    digitalWrite(PULLER_MOTOR_STEP_PIN, HIGH);
    if((micros() - previousMicros) >= (desiredPulseMicros/2)) {
      previousMicros = micros();
      digitalWrite(PULLER_MOTOR_STEP_PIN, LOW);
    }
  }
} 
