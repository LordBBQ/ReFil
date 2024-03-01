/**
 * Trapezoid motion profiler
 * @param int targetPosition - the desired position in mm
 * @param double maxAccel - maximum desired acceleration, in mm.s^-2
 * @param double maxDecel - maximum desired deceleration, in mm.s^-2
 * @param double maxVelocity - maximum desired velocity, in mm.s^-1
 */
static boolean trapezoidMotionProfiler(int targetPosition, double maxAccel, double maxDecel, double maxVelocity) {
  double accelTime = maxVelocity / maxAccel;
  double decelTime = maxVelocity / maxDecel;

  double accelDistance = 0.5 * maxAccel * accelTime * accelTime;
  double decelDistance = 0.5 * maxDecel * decelTime * decelTime;

  if((accelDistance + decelDistance) >= targetPosition

  double constantVelocityDistance = targetPosition - accelDistance

}
