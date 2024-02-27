#include <EEPROM.h>
#include <InterpolationLib.h>

#define WIDTH_SENSOR_PIN "A8"
#define WIDTH_SENSOR_FACTORY_CAL_1MM 1
#define WIDTH_SENSOR_FACTORY_CAL_2MM 3
#define WIDTH_SENSOR_FACTORY_CAL_3MM 5

#define WIDTH_SENSOR_SAMPLE_TIME 50 //ms

double widthSensorUserCalStored1MM;
double widthSensorUserCalStored2MM;
double widthSensorUserCalStored3MM;

double widthSensorUserCal1MM;
double widthSensorUserCal2MM;
double widthSensorUserCal3MM;

double widthVal = 1.75;
double lastWidth = 1.75;
double secondLastWidth = 1.75;
double thirdLastWidth = 1.75;

unsigned long widthSensorCurrentTime = 0;
unsigned long widthSensorPreviousTime = 0;

Interpolation widthSensorCalibrationSpline;

double widthSensorCalibrationPoints[] {
  1,
  2,
  3,
};

double widthSensorCalibrationValue[] {
  0,
  0,
  0,
};

/**
 * Reset all the s
 */
void initWidthSensor() {
  //Init stored values

  //Init working values
  widthSensorUserCal1MM = WIDTH_SENSOR_FACTORY_CAL_1MM;
  widthSensorUserCal2MM = WIDTH_SENSOR_FACTORY_CAL_2MM;
  widthSensorUserCal3MM = WIDTH_SENSOR_FACTORY_CAL_3MM;

  widthSensorCalibrationValue[0] = widthSensorUserCal1MM;
  widthSensorCalibrationValue[1] = widthSensorUserCal2MM;
  widthSensorCalibrationValue[2] = widthSensorUserCal3MM;
}

static double getFilamentSensor() {
  widthSensorCurrentTime = millis();
  double widthVoltage = analogRead(WIDTH_SENSOR_PIN); //read the pin
  
  if((widthSensorCurrentTime - widthSensorPreviousTime) >= WIDTH_SENSOR_SAMPLE_TIME) {
    thirdLastWidth = secondLastWidth;
    secondLastWidth = lastWidth;
    lastWidth = widthVal;
    widthVal = Interpolation::CatmullSpline(widthSensorCalibrationValue, widthSensorCalibrationPoints, 3, widthVoltage); //interpolate the values
    widthSensorPreviousTime = widthSensorCurrentTime;
  }
  
  return ((widthVal + lastWidth + secondLastWidth + thirdLastWidth) / 4);
}
