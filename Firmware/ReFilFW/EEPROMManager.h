#include <EEPROM.h>

#define EEPROM_DELAY 4 //ms per write

#define EEPROM_CHK_ADDRESS 1
#define EEPROM_START_ADDRESS 23
#define EEPROM_LENGTH 254

#define DOUBLE_BYTE_LENGTH 8
#define WIDTH_SENSOR_STORED_CAL_1MM_EEPROM_ADDR EEPROM_START_ADDRESS
#define WIDTH_SENSOR_STORED_CAL_2MM_EEPROM_ADDR WIDTH_SENSOR_STORED_CAL_1MM_EEPROM_ADDR + DOUBLE_BYTE_LENGTH
#define WIDTH_SENSOR_STORED_CAL_2MM_EEPROM_ADDR WIDTH_SENSOR_STORED_CAL_2MM_EEPROM_ADDR + DOUBLE_BYTE_LENGTH

String eepromInitCode = "INIT_OK"
boolean EEPROMInitExec = false;

/**
 * Check if EEPROM has been initalised, if so continue with bootup. Otherwise will overrite
 * all the EEPROM memory
 */
boolean initEEPROM() {
  if(EEPROM.read(EEPROM_CHK_ADDRESS) == eepromInitCode) {
    EEPROMInitExec = true;
    return true;
  } else {
    if(!EEPROMInitExec) {
      for(i = 1; i <= EEPROM_LENGTH; i++) {
        EEPROM.update(i, 0); 
        delay(EEPROM_DELAY); //EEPROM takes 3.3ms to udpate datapoint
      }
      EEPROM.update(EEPROM_CHK_ADDRESS, eepromInitCode); //Write code to signify that EEPROM has been configrued
      EEPROMInitExec = true;
      return true;
    } else {
      return false;
    }
  }
}

/**
 * Write width sensor values to EEPROM. Note, this process takes around 15ms.
 */
boolean writeWidthSensorCalibration(double cal1, double cal2, double cal3) {
  EEPROM.update(WIDTH_SENSOR_STORED_CAL_1MM_EEPROM_ADDR, cal1); //write 1st cal value to EEPROM
  delay(EEPROM_DELAY); //delay to finish write
  EEPROM.update(WIDTH_SENSOR_STORED_CAL_2MM_EEPROM_ADDR, cal2); //write second
  delay(EEPROM_DELAY); //and so on.....
  EEPROM.update(WIDTH_SENSOR_STORED_CAL_3MM_EEPROM_ADDR, cal3);
  delay(EEPROM_DELAY);
}
