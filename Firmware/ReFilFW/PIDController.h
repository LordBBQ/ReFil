//#include "Utilities.h"

unsigned long pidControllerCurrentTIme;
unsigned long pidControllerpidControllerpidControllerCurrentCycleTime;
unsigned long pidControllerElapsedTime;
unsigned long pidControllerCurrentCycleTime;
unsigned long pidControllerPreviousCycleTime;

double previousError;

boolean debugMessages = false;
boolean cycleComplete = true;

double calculatedD;
double PIDValue;

void initPIDController() {
  pidControllerCurrentTIme = millis(); //Init time variables
  pidControllerpidControllerpidControllerCurrentCycleTime = millis();
  previousError = 0;
  pidControllerElapsedTime = 0;
  pidControllerCurrentCycleTime = 0;
  pidControllerPreviousCycleTime = 0;
  PIDValue = 0;
  calculatedD = 0;
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
double PIDController(double kP, double kI, double kD, double setPoint, double feedback, int cycleDelay) {
  
  if(cycleComplete) { //This code is a delay based on the parameter, however it does not 'stop' the code while waiting
    double error = (setPoint - feedback);
    double calculatedP = kP * error;
    double calculatedI;

    //Calculate Integral coefficient
    if(-3 < error < 3) {
      calculatedI = calculatedI + (kI * error);
    } else {
      if (calculatedI == NULL) { //check if calculatedI is initalised
        calculatedI = 0;
      }
    }

    //Calculate derivitave coefficent
    pidControllerpidControllerpidControllerCurrentCycleTime = pidControllerCurrentTIme; //Since we have not updated pidControllerCurrentTIme, it is the last time the cycle was run so thus can be our previous time starting point
    pidControllerCurrentTIme = millis(); //Now we update the current time variable

    pidControllerElapsedTime = (cycleDelay/1000);//((pidControllerCurrentTIme-pidControllerpidControllerpidControllerCurrentCycleTime) / 1000); //Elapsed time converted from ms to s
    //double calculatedD = 0;
    if(pidControllerElapsedTime != 0) { //Failsafe to prevent div/0 errors if loop runs too fast.
      calculatedD = kD * ((error - previousError) / pidControllerElapsedTime);
    } else {
      calculatedD = 0;
    }

    //Calculate PID value
    PIDValue = (calculatedP + calculatedI + calculatedD);




    if(debugMessages) {
      Serial.print("PID Controller execution with cycle: ");
      Serial.print(PIDValue);
      Serial.print(", P: ");
      Serial.print(calculatedP);
      Serial.print(", I: ");
      Serial.print(calculatedI);
      Serial.print(", D: ");
      Serial.print(calculatedD);
      Serial.print(", previousError: ");
      Serial.print(previousError);
      Serial.print(", and error: ");
      Serial.println(error);
    }
    previousError = error; //update previous error value
    pidControllerCurrentCycleTime = millis();
    pidControllerPreviousCycleTime = pidControllerCurrentCycleTime;
  
    cycleComplete = false; //reset so it counts time next cycle
    
   } else {
     if((pidControllerCurrentCycleTime - pidControllerPreviousCycleTime) >= cycleDelay) { //This checks for if the cycleDelay has elapsed
       cycleComplete = true;
     } else {
       pidControllerCurrentCycleTime = millis();
     }
   }

  //Since Arduino PWM ranges between 0 and 255, we must limit it as so
  if(PIDValue < 0) {
    PIDValue = 0.0;
  } else if(PIDValue > 255) {
    PIDValue = 255.0;
  }

  return PIDValue;
  
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
//    calculatedD = kD * ((error - previousError) / pidControllerElapsedTime);
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
