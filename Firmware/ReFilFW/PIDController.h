#include <FastPID.h>


//Heater 0 constants
#define HEATER_0_PIN 9

//Heater 1 constants
#define HEATER_1_PIN 10
#define HEATER_1_KP HEATER_0_KP
#define HEATER_1_KI HEATER_0_KI
#define HEATER_1_KD HEATER_0_KD

//Heater 2 constants
#define HEATER_2_PIN 22
#define HEATER_2_KP HEATER_0_KP
#define HEATER_2_KI HEATER_0_KI
#define HEATER_2_KD HEATER_0_KD

//Heater 3 constants
#define HEATER_3_PIN 23
#define HEATER_3_KP HEATER_0_KP
#define HEATER_3_KI HEATER_0_KI
#define HEATER_3_KD HEATER_0_KD

float heater0kP = 3;
float heater0kI = 100;
float heater0kD = 0;
FastPID pidController0(heater0kP, heater0kI, heater0kD, 8, false);

float heater1kP = heater0kP;
float heater1kI = heater0kI;
float heater1kD = heater0kD;
FastPID pidController1(heater1kP, heater1kI, heater1kD, 8, false);

float heater2kP = heater0kP;
float heater2kI = heater0kI;
float heater2kD = heater0kD;
FastPID pidController2(heater2kP, heater2kI, heater2kD, 8, false);

float heater3kP = heater0kP;
float heater3kI = heater0kI;
float heater3kD = heater0kD;
FastPID pidController3(heater3kP, heater3kI, heater3kD, 8, false);

class PIDController0 {
  private:

    uint32_t lastCycle = 0;
    int cycleDelay = 100; //ms
    uint8_t output = 0;

  /**
   * @param float setpoint
   * @param double feedback
   * @return double output duty cycle
   */
  public:
    double callPIDController0(float setpoint, double feedback) {
      uint32_t before, after;
      if((micros() - lastCycle) > cycleDelay) {
        before = micros();
        output = pidController0.step(setpoint, feedback);
        after = micros();
        
        lastCycle = micros();
        return output;
      } else {
        return output;  
      }
      
    }
  
};

class PIDController1 {
  private:

    uint32_t lastCycle = 0;
    int cycleDelay = 100; //ms
    uint8_t output = 0;

  public:
    /**
     * @param float setpoint
     * @param double feedback
     * @return double output duty cycle
     */
    double callPIDController1(float setpoint, double feedback) {
      uint32_t before, after;
      if((micros() - lastCycle) > cycleDelay) {
        before = micros();
        output = pidController1.step(setpoint, feedback);
        after = micros();
        
        lastCycle = micros();
        return output;
      } else {
        return output;  
      }
      
    }
  
};

class PIDController2 {
  private:

    uint32_t lastCycle = 0;
    int cycleDelay = 100; //ms
    uint8_t output = 0;

  public:
  /**
   * @param float setpoint
   * @param double feedback
   * @return double output duty cycle
   */
    double callPIDController2(float setpoint, double feedback) {
      uint32_t before, after;
      if((micros() - lastCycle) > cycleDelay) {
        before = micros();
        output = pidController2.step(setpoint, feedback);
        after = micros();
        
        lastCycle = micros();
        return output;
      } else {
        return output;  
      }
      
    }
  
};

class PIDController3 {
  private:

    uint32_t lastCycle = 0;
    int cycleDelay = 100; //ms
    uint8_t output = 0;

  public:
    /**
     * @param float setpoint
     * @param double feedback
     * @return double output duty cycle
     */
    double callPIDController3(float setpoint, double feedback) {
      uint32_t before, after;
      if((micros() - lastCycle) > cycleDelay) {
        before = micros();
        output = pidController3.step(setpoint, feedback);
        after = micros();
        
        lastCycle = micros();
        return output;
      } else {
        return output;  
      }
      
    }
  
};
