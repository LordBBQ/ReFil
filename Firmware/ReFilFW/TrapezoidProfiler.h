  static unsigned long currentTime = 0;
  static unsigned long previousTime = 0;
  static unsigned long deltaTime = 0;


/**
 * Trapezoid motion profiler
 * @param int targetPosition - the desired position in mm
 * @param double maxAccel - maximum desired acceleration, in mm.s^-2
 * @param double maxDecel - maximum desired deceleration, in mm.s^-2
 * @param double maxVelocity - maximum desired velocity, in mm.s^-1
 */
static boolean generateTrapezoidProfile(int targetPosition, int currentPosition, double maxAccel, double maxDecel, double maxVelocity) {
  currentTime = micros();
  previousTime = 0;
  deltaTime = currentTime - previousTime
  
  double accelTime = maxVelocity / maxAccel;
  double decelTime = maxVelocity / maxDecel;

  double accelDistance = 0.5 * maxAccel * accelTime * accelTime;
  double decelDistance = 0.5 * maxDecel * decelTime * decelTime;

  if((accelDistance + decelDistance) >= math.abs(targetPosition-currentPosition)) {
    accelDistance = math.abs(targetPosition-currentPosition)/2;
    dccelDistance = math.abs(targetPosition-currentPosition)/2;

  }
  double constantVelocityDistance = targetPosition - accelDistance

}
