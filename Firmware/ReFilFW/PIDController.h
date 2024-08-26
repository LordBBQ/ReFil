#include <FastPID.h>


//Heater 0 constants
#define HEATER_0_PIN 9
#define HEATER_0_KP 3
#define HEATER_0_KI 100
#define HEATER_0_KD 0.0

//Heater 1 constants
#define HEATER_1_PIN 10
#define HEATER_1_KP HEATER_0_KP
#define HEATER_1_KI HEATER_0_KI
#define HEATER_1_KD HEATER_0_KD

//Heater 2 constants
#define HEATER_2_PIN 60
#define HEATER_2_KP HEATER_0_KP
#define HEATER_2_KI HEATER_0_KI
#define HEATER_2_KD HEATER_0_KD

//Heater 3 constants
#define HEATER_3_PIN 23
#define HEATER_3_KP HEATER_0_KP
#define HEATER_3_KI HEATER_0_KI
#define HEATER_3_KD HEATER_0_KD
//PID0
unsigned long pidController0CurrentTIme;
unsigned long pidController0pidControllerpidControllerCurrentCycleTime;
unsigned long pidController0ElapsedTime;
unsigned long pidController0CurrentCycleTime;
unsigned long pidController0PreviousCycleTime;

double previousError0;

boolean debugMessages = true;
boolean cycleComplete0 = true;

double calculatedD0;
double PIDValue0;

//PID1
unsigned long pidController1CurrentTIme;
unsigned long pidController1pidControllerpidControllerCurrentCycleTime;
unsigned long pidController1ElapsedTime;
unsigned long pidController1CurrentCycleTime;
unsigned long pidController1PreviousCycleTime;

double previousError1;

boolean cycleComplete1 = true;

double calculatedD1;
double PIDValue1;

void initPIDController() {
  pidController0CurrentTIme = millis(); //Init time variables
  pidController0pidControllerpidControllerCurrentCycleTime = millis();
  previousError0 = 0;
  pidController0ElapsedTime = 0;
  pidController0CurrentCycleTime = 0;
  pidController0PreviousCycleTime = 0;
  PIDValue0 = 0;
  calculatedD0 = 0;

  pidController1CurrentTIme = millis(); //Init time variables
  pidController1pidControllerpidControllerCurrentCycleTime = millis();
  previousError1 = 0;
  pidController1ElapsedTime = 0;
  pidController1CurrentCycleTime = 0;
  pidController1PreviousCycleTime = 0;
  PIDValue1 = 0;
  calculatedD1 = 0;
}

/**
 * @param double kP: The preportonal constant
 * @param double kI: The integral constant
 * @param double kD: The derivative constant
 * @param double setPoint: The setpoint of the heater
 * @param double feedback: The value of the heater's sensor
 * @param int cycleDelay: the delay in hte cycle
 * @return double PIDOutput: The duty cycle output of the PID controller
 */
double PIDController0(double kP, double kI, double kD, double setPoint, double feedback, int cycleDelay) {

  if(cycleComplete0) { //This code is a delay based on the parameter, however it does not 'stop' the code while waiting
    double error0 = (setPoint - feedback);
    double calculatedP = kP * error0;
    double calculatedI;

    //Calculate Integral coefficient
    if(-3 < error0 < 3) {
      calculatedI = calculatedI + (kI * error0);
    } else {
      if (calculatedI == NULL) { //check if calculatedI is initalised
        calculatedI = 0;
      }
    }

    //Calculate derivitave coefficent
    pidController0pidControllerpidControllerCurrentCycleTime = pidController0CurrentTIme; //Since we have not updated pidController0CurrentTIme, it is the last time the cycle was run so thus can be our previous time starting point
    pidController0CurrentTIme = millis(); //Now we update the current time variable

    pidController0ElapsedTime = (cycleDelay/1000);//((pidController0CurrentTIme-pidController0pidControllerpidControllerCurrentCycleTime) / 1000); //Elapsed time converted from ms to s
    //double calculatedD = 0;
    if(pidController0ElapsedTime != 0) { //Failsafe to prevent div/0 errors if loop runs too fast.
      calculatedD0 = kD * ((error0 - previousError0) / pidController0ElapsedTime);
    } else {
      calculatedD0 = 0;
    }

    //Calculate PID value
    PIDValue0 = (calculatedP + calculatedI + calculatedD0);




    if(debugMessages) {
      Serial.print("PID Controller execution with cycle: ");
      Serial.print(PIDValue0);
      Serial.print(", P: ");
      Serial.print(calculatedP);
      Serial.print(", I: ");
      Serial.print(calculatedI);
      Serial.print(", D: ");
      Serial.print(calculatedD0);
      Serial.print(", previousError0: ");
      Serial.print(previousError0);
      Serial.print(", and error: ");
      Serial.println(error0);
    }
    previousError0 = error0; //update previous error value
    pidController0CurrentCycleTime = millis();
    pidController0PreviousCycleTime = pidController0CurrentCycleTime;

    cycleComplete0 = false; //reset so it counts time next cycle

   } else {
     if((pidController0CurrentCycleTime - pidController0PreviousCycleTime) >= cycleDelay) { //This checks for if the cycleDelay has elapsed
       cycleComplete0 = true;
     } else {
       pidController0CurrentCycleTime = millis();
     }
   }

  //Since Arduino PWM ranges between 0 and 255, we must limit it as so
  if(PIDValue0 < 0) {
    PIDValue0 = 0.0;
  } else if(PIDValue0 > 255) {
    PIDValue0 = 255.0;
  }

  return PIDValue0;

}

/**
 * @param double kP: The preportonal constant
 * @param double kI: The integral constant
 * @param double kD: The derivative constant
 * @param double setPoint: The setpoint of the heater
 * @param double feedback: The value of the heater's sensor
 * @param int cycleDelay: the delay in hte cycle
 * @return double PIDOutput: The duty cycle output of the PID controller
 */
double PIDController1(double kP, double kI, double kD, double setPoint, double feedback, int cycleDelay) {

  if(cycleComplete1) { //This code is a delay based on the parameter, however it does not 'stop' the code while waiting
    double error1 = (setPoint - feedback);
    double calculatedP = kP * error1;
    double calculatedI;

    //Calculate Integral coefficient
    if(-3 < error1 < 3) {
      calculatedI = calculatedI + (kI * error1);
    } else {
      if (calculatedI == NULL) { //check if calculatedI is initalised
        calculatedI = 0;
      }
    }

    //Calculate derivitave coefficent
    pidController1pidControllerpidControllerCurrentCycleTime = pidController1CurrentTIme; //Since we have not updated pidController0CurrentTIme, it is the last time the cycle was run so thus can be our previous time starting point
    pidController1CurrentTIme = millis(); //Now we update the current time variable

    pidController1ElapsedTime = (cycleDelay/1000);//((pidController0CurrentTIme-pidController0pidControllerpidControllerCurrentCycleTime) / 1000); //Elapsed time converted from ms to s
    //double calculatedD = 0;
    if(pidController1ElapsedTime != 0) { //Failsafe to prevent div/0 errors if loop runs too fast.
      calculatedD1 = kD * ((error1 - previousError1) / pidController1ElapsedTime);
    } else {
      calculatedD1 = 0;
    }

    //Calculate PID value
    PIDValue1 = (calculatedP + calculatedI + calculatedD1);




    if(debugMessages) {
      Serial.print("PID1 Controller execution with cycle: ");
      Serial.print(PIDValue1);
      Serial.print(", P: ");
      Serial.print(calculatedP);
      Serial.print(", I: ");
      Serial.print(calculatedI);
      Serial.print(", D: ");
      Serial.print(calculatedD1);
      Serial.print(", previousError1: ");
      Serial.print(previousError1);
      Serial.print(", and error: ");
      Serial.println(error1);
    }
    previousError1 = error1; //update previous error value
    pidController1CurrentCycleTime = millis();
    pidController1PreviousCycleTime = pidController1CurrentCycleTime;

    cycleComplete1 = false; //reset so it counts time next cycle

   } else {
     if((pidController1CurrentCycleTime - pidController1PreviousCycleTime) >= cycleDelay) { //This checks for if the cycleDelay has elapsed
       cycleComplete1 = true;
     } else {
       pidController1CurrentCycleTime = millis();
     }
   }

  //Since Arduino PWM ranges between 0 and 255, we must limit it as so
  if(PIDValue1 < 0) {
    PIDValue1 = 0.0;
  } else if(PIDValue1 > 255) {
    PIDValue1 = 255.0;
  }

  return PIDValue1;

}

//double PIDController(double kP, double kI, double kD, double setPoint, double feedback, int cycleDelay) {
//  float error = (setPoint - feedback);
//  float calculatedP = kP * error;
//  float calculatedI;
//
//  if(cycleComplete) { //This code is a delay based on the parameter, however it does not 'stop' the code while waiting
//    pidControllerCurrentCycleTime = millis();
//    pidControllerPreviousCycleTime = pidControllerCurrentCycleTime;
//    cycleComplete = false; //reset so it counts time next cycle
//  } else {
//    if((pidControllerCurrentCycleTime - pidControllerPreviousCycleTime) >= cycleDelay) { //This checks for if the cycleDelay has elapsed
//      cycleComplete = true;
//    } else {
//      pidControllerCurrentCycleTime = millis();
//    }
//  }
//
//  //Calculate Integral coefficient
//  if(-3 < error < 3) {
//    calculatedI = calculatedI + (kI * error);
//  } else {
//    if (calculatedI == NULL) { //check if calculatedI is initalised
//      calculatedI = 0;
//    }
//  }
//
//  //Calculate derivitave coefficent
//  pidControllerpidControllerpidControllerCurrentCycleTime = pidControllerCurrentTIme; //Since we have not updated pidControllerCurrentTIme, it is the last time the cycle was run so thus can be our previous time starting point
//  pidControllerCurrentTIme = millis(); //Now we update the current time variable
//
//  double calculatedD = 0;
//  if(pidControllerElapsedTime != 0) { //Failsafe to prevent div/0 errors if loop runs too fast.
//    calculatedD = kD * ((error - previousError0) / pidControllerElapsedTime);
//  } else {
//    calculatedD = 0;
//  }
//
//  //Calculate PID value
//  double PIDValue = (calculatedP + calculatedI + calculatedD);
//
//  //Since Arduino PWM ranges between 0 and 255, we must limit it as so
//  if(PIDValue < 0) {
//    PIDValue = 0.0;
//  } else if(PIDValue > 255) {
//    PIDValue = 255.0;
//  }
//
//  previousError = error; //update previous error value
//  if(debugMessages) {
//    Serial.print("PID Controller execution with cycle: ");
//    Serial.print(PIDValue);
//    Serial.print(", P: ");
//    Serial.print(calculatedP);
//    Serial.print(", I: ");
//    Serial.print(calculatedI);
//    Serial.print(", D: ");
//    Serial.print(calculatedD);
//    Serial.print(", and error: ");
//    Serial.println(error);
//  }
//  return PIDValue;
//  
//}