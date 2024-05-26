#include "Extruder.h"
#include "Puller.h"
#include "FilamentSensor.h"

bool zone0AtTemp = false;
bool zone1AtTemp = false;
bool zone2AtTemp = false;
bool zone3AtTemp = false;

bool extruderReadyFlag = false;

void initExtruder() {
  zone0AtTemp = false;
  zone1AtTemp = false;
  zone2AtTemp = false;
  zone3AtTemp = false;
  extruderReadyFlag = false;
}

/** 
* @param zone0 temperature for first zone heater
* @param zone1 temperature for second zone heater
* @param zone2 temperature for third zone heater
* @param zone3 temperature for fourth zone heater
* @param targetDiameter target diameter
* @param fanRPM set fan speed
* @return when complete
*
* Begins heating, gets system ready, turns on fan etc.
*/
bool start(float zone0, float zone1, float zone2, float zone3, float targetDiameter, float fanRPM) {
  setFans(fanRPM); //startup the cooling fans

  setHeater(0, zone0); //begin heating
  setHeater(1, zone1);
  setHeater(2, zone2);
  setHeater(3, zone3);

  if(setHeater(0, zone0) || zone0AtTemp) { //check if at temp or have already hit temp before returning true to havinig initialised
    zone0AtTemp = true;
    if(setHeater(1, zone1) || zone1AtTemp) { //check if at temp or have already hit temp before returning true to havinig initialised
    zone1AtTemp = true;
      if(setHeater(2, zone2) || zone2AtTemp) { //check if at temp or have already hit temp before returning true to havinig initialised
        zone2AtTemp = true;
        if(setHeater(3, zone3) || zone3AtTemp) { //check if at temp or have already hit temp before returning true to havinig initialised
          zone3AtTemp = true;
          extruderReadyFlag = true;
          return true; //extruder is all good and ready to go!
        }
      }
    }
  }
  extruderReadyFlag = false;
  return false; //still needs to heat/init
}

/** 
* @param zone0 temperature for first zone heater
* @param zone1 temperature for second zone heater
* @param zone2 temperature for third zone heater
* @param zone3 temperature for fourth zone heater
* @param targetDiameter target diameter
* @param fanRPM set fan speed
* @param flowRate desired filament extrusion rate, in mm/s
*
* Starts pushing the filament
*/
boolean extrude(float zone0, float zone1, float zone2, float zone3, float targetDiameter, float fanRPM, double flowRate) {

  if(!extruderReadyFlag) {
    start(zone0, zone1, zone2, zone3, targetDiameter, fanRPM); //check if at temp
  } else {
    setHeater(0, zone0); //keep them heating
    setHeater(1, zone1);
    setHeater(2, zone2);
    setHeater(3, zone3);

    spinDriveMotor(convertVolumeToRPM(flowRate));
    //movePullerMotor(calculatePullerSpeed(targetDiameter, getFilamentSensor()));
  }
return false;

}
