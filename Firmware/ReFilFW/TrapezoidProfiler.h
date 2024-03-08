#pragma once


class TrapezoidMotionProfiler {

  static TrapezoidMotionProfiler *m_instance;

  static unsigned long currentTime;
  static unsigned long previousTime;
  static unsigned long deltaTime;
  static double accelDistance;
  static double decelDistance;
  static double constantVelocityDistance;
  static double currentVelocity;
  static double startingPosition;
  static double maxDecelerationLimit;
  static double maxAccelerationLimit;
  static double maxVelocityLimit;


  TrapezoidMotionProfiler() {
    currentTime = 0;
    previousTime = 0;
    deltaTime = 0;
    accelDistance = 0;
    decelDistance = 0;
    constantVelocityDistance = 0;
    currentVelocity = 0;
    startingPosition = 0;
    maxDecelerationLimit = 0;
    maxAccelerationLimit = 0;
    maxVelocityLimit = 0;
  }

  // static TrapezoidMotionProfiler *getTrapezoidProfilerInstance() {
  //   if(!m_instance) {
  //     m_instance = new *TrapezoidMotionProfiler();
  //   }
  //   return m_instance;
  // }
  
  /**
   * Trapezoid motion profiler
   * @param int targetPosition - the desired position in mm
   * @param double maxAccel - maximum desired acceleration, in mm.s^-2
   * @param double maxDecel - maximum desired deceleration, in mm.s^-2
   * @param double maxVelocity - maximum desired velocity, in mm.s^-1
   */
  static void generateTrapezoidProfile(int targetPosition, int currentPosition, double maxAccel, double maxDecel, double maxVelocity) {
    maxDecelerationLimit = maxDecel;
    maxAccelerationLimit = maxAccel;
    maxVelocityLimit = maxVelocity;

    startingPosition = currentPosition;

    previousTime = 0;
    deltaTime = currentTime - previousTime;
  
    double accelTime = maxVelocity / maxAccelerationLimit;
    double decelTime = maxVelocity / maxDecelerationLimit;

    accelDistance = 0.5 * maxAccelerationLimit * accelTime * accelTime;
    decelDistance = 0.5 * maxDecelerationLimit * decelTime * decelTime;

    if((accelDistance + decelDistance) >= abs(targetPosition-currentPosition)) {
      accelDistance = abs(targetPosition-currentPosition)/2;
      decelDistance = abs(targetPosition-currentPosition)/2;

    }
    constantVelocityDistance = targetPosition - accelDistance;
  }

  
  static double runTrapezoidProfiler(int currentPosition) {
    int distanceTraveled = currentPosition - startingPosition; //calc how far we have come
    currentTime = micros();
    deltaTime = currentTime - previousTime; //calc time since last loop update
    if(distanceTraveled <= accelDistance) { //do we still need to accelerate
      currentVelocity += (maxAccelerationLimit * (deltaTime / 1000000));
    } else if(distanceTraveled <= (accelDistance + constantVelocityDistance)) {//are we running at max v
      currentVelocity = maxVelocityLimit;
    } else if(distanceTraveled <= (accelDistance + constantVelocityDistance + decelDistance)) { //do we need to start decelerating
      currentVelocity -= (maxDecelerationLimit * (deltaTime / 1000000));
    } else { //catch-all, should triger when finished path.
      currentVelocity = 0;
    }
    previousTime = micros();
    return currentVelocity;
  }

