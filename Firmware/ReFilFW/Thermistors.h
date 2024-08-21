
#define THERMISTOR_0_PIN A13
#define THERMISTOR_1_PIN A14
#define THERMISTOR_2_PIN A7 //TODO
#define THERMISTOR_3_PIN A7 //TODO

#define HEATER_MINTEMP 10 //degrees c
#define HEATER_MAXTEMP 280 //degrees c

#define THERMAL_SAFTEY_FAULT_LEVEL_THRESH 200 //how many fault points before a failure is presumed
#define THERMAL_SAFTEY_FAULT_LEVEL_DEGREDATION 3 //how many fault points are lost per cycle
#define THERMAL_SAFTEY_CYCLE 500 //ms
#define THERMAL_RUNAWAY_MIN_GRADIENT_DUTYCYCLE 0
#define THERMAL_RUNAWAY_MIN_GRADIENT_TEMPERATURE 0

#define VIN 5.0 //Volts
#define R1 4700 //Ohms
#define NOMINAL_RESISTANCE 100000 //Nominal resistance, Ohms
#define NOMINAL_RESISTANCE_TEMP 25 //Temperature of nominal resistance reading, degrees celcius
#define SAMPLE_COUNT 5 //Number of samples
#define BETA_COEFFICIENT 3950

int heaterFaultLevel[4];
int heaterPreviousTemperature[4];
int heaterPreviousDutyCycle[4];
int heaterCurrentTemperature[4];
int heaterCurrentDutyCycle[4];

boolean thermalSafteyTrip = false;
boolean thermalSafteyCycleComplete;
boolean thermalSafteyHasInit = false;

unsigned long thermalSafteyCurrentTime;
unsigned long thermalSafteyPreviousTime;

void initThermalSaftey() {
  heaterFaultLevel[0] = 0;
  heaterFaultLevel[1] = 0;
  heaterFaultLevel[2] = 0;
  heaterFaultLevel[3] = 0;

  thermalSafteyTrip = false;
  thermalSafteyCycleComplete = true;
  thermalSafteyCurrentTime = millis();
  thermalSafteyPreviousTime = millis();
  thermalSafteyHasInit = true;

  heaterPreviousTemperature[0] = 0;
  heaterCurrentTemperature[0] = 0;
  heaterPreviousDutyCycle[0] = 0;
  heaterCurrentDutyCycle[0] = 0;
  heaterPreviousTemperature[1] = 0;
  heaterCurrentTemperature[1] = 0;
  heaterPreviousDutyCycle[1] = 0;
  heaterCurrentDutyCycle[1] = 0;
  heaterPreviousTemperature[2] = 0;
  heaterCurrentTemperature[2] = 0;
  heaterPreviousDutyCycle[2] = 0;
  heaterCurrentDutyCycle[2] = 0;
  heaterPreviousTemperature[3] = 0;
  heaterCurrentTemperature[3] = 0;
  heaterPreviousDutyCycle[3] = 0;
  heaterCurrentDutyCycle[3] = 0;


}
/**
 * @param byte heaterIndex: The index of the heater which's thermistor should be checked
 */
double calculateTemperature(double adcValue) {
  double thermistorVoltage = ((adcValue / 1024.0) * VIN);
  double thermistorResistance = (thermistorVoltage * R1) / (VIN - (thermistorVoltage));
  double temperature = 1.0 / (
    log(
          thermistorResistance / NOMINAL_RESISTANCE
        ) / BETA_COEFFICIENT + 1.0 / (
           NOMINAL_RESISTANCE_TEMP + 273.15
        )
     );
  return temperature - 273.15; //convert from K to C
}

unsigned char getThermistorPort(byte heaterIndex) {
  switch (heaterIndex) {
    case 0:
      return THERMISTOR_0_PIN;
    case 1:
      return THERMISTOR_1_PIN;
    case 2:
      return THERMISTOR_2_PIN;
    case 3:
      return THERMISTOR_3_PIN;
  }
}
 
double getThermistorValue(byte heaterIndex) {
  return (
    calculateTemperature(
      analogRead(
        getThermistorPort(heaterIndex)
      )
    )
  );
}

/**
 * Returns true if a thermal fault has occured
 * @param double requestedDutyCycle: the requested heater duty cycle
 * @param double feebackTemperature: the thermistor temperature
 */
boolean checkThermalSaftey(double requestedDutyCycle, double feedbackTemperature, byte heaterIndex) {
  //Serial.print(thermalSafteyTrip && thermalSafteyHasInit);

  if(heaterFaultLevel[heaterIndex] >= THERMAL_SAFTEY_FAULT_LEVEL_THRESH) { //Check if fault level is over threshold, if so trip protection.
    return true;
  } else { //if threshold is ok, continue to checks
    if(thermalSafteyCycleComplete) { //check if cycle has elapsed, if so run checks
      //Serial.print("trig");

      
      heaterCurrentTemperature[heaterIndex] = feedbackTemperature;
      heaterCurrentDutyCycle[heaterIndex] = requestedDutyCycle;
      
      if(feedbackTemperature <= HEATER_MINTEMP) {
        heaterFaultLevel[heaterIndex] = 100 + heaterFaultLevel[heaterIndex];
        //Serial.print("mintemp");
      }
      if(feedbackTemperature >= HEATER_MAXTEMP) {
        heaterFaultLevel[heaterIndex] = 100 + heaterFaultLevel[heaterIndex];
        //Serial.print("maxtemp");

      }

      if(heaterCurrentDutyCycle[heaterIndex] > THERMAL_RUNAWAY_MIN_GRADIENT_DUTYCYCLE && heaterPreviousDutyCycle[heaterIndex] > THERMAL_RUNAWAY_MIN_GRADIENT_DUTYCYCLE) {
        if((heaterCurrentTemperature[heaterIndex] - heaterPreviousTemperature[heaterIndex]) < THERMAL_RUNAWAY_MIN_GRADIENT_TEMPERATURE) {
          heaterFaultLevel[heaterIndex] = 9 + heaterFaultLevel[heaterIndex];
        }
      }

      if(heaterFaultLevel[heaterIndex] <= 0) {
        heaterFaultLevel[heaterIndex] = 0;
      } else {
        heaterFaultLevel[heaterIndex] -= THERMAL_SAFTEY_FAULT_LEVEL_DEGREDATION;
      }

      //
      thermalSafteyCycleComplete = false;
      heaterPreviousTemperature[heaterIndex] = heaterCurrentTemperature[heaterIndex];
      heaterPreviousDutyCycle[heaterIndex] = heaterCurrentDutyCycle[heaterIndex];
      
      
      thermalSafteyCurrentTime = millis();
      thermalSafteyPreviousTime = thermalSafteyCurrentTime;
    } else { //if has not has elapsed but not been checked, set flag to true so next execution runs checks
      if((thermalSafteyCurrentTime - thermalSafteyPreviousTime) >= THERMAL_SAFTEY_CYCLE) {
        thermalSafteyCycleComplete = true;
      } else { //if time has not elapsed, update the current time
        thermalSafteyCurrentTime = millis();
      }
    }
    
    
  }
  return false;
}
