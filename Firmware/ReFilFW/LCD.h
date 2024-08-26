#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define printByte(args)  write(args)
#define FAULT_LED_PIN 57
#define STATUS_LED_PIN 58

#define FAN_PIN 52

#define BUTTON_DOWN_PIN 14
#define BUTTON_UP_PIN 2
#define BUTTON_RIGHT_PIN 15
#define BUTTON_LEFT_PIN 18
#define BUTTON_OK_PIN 19

#define MENU_ITEMS 7
#define MENU_DEPTH 2

//LCD main code
LiquidCrystal_I2C lcd(0x20,20,4); //define lcd on port 0x20

unsigned long blinkLast = 0;
unsigned long blinkCurrent = 0;

unsigned long lcdLastUpdate = 0;
unsigned long lcdCurrentTime = 0;

unsigned long lastButtonUpdate = 0;
int buttonUpdateDelay = 250; //ms

bool blinkState = false;

byte lcdLastStatus = 0;
int lcdLastTemp = 0;
int lcdLastMotorSpeed = 0;
int lcdLastMenu = 10;
int lcdLastSetpoint = 0;

bool lcdLastControlFans = false;
bool lcdLastControlHeaters = false;
bool lcdLastControlGantry = false;
bool lcdLastControlSpooler = false;
bool lcdLastControlPuller = false;
bool lcdLastControlAutoExtrusion = false;
bool lcdLastControlMotor = false;
bool lcdLastControlDirInvert = false;


bool lcdControlHeaters = false;
bool lcdControlFans = false;
bool lcdControlSpool = false;
bool lcdControlPuller = false;
bool lcdControlGantry = false;
bool lcdControlAutoExtrusion = false;
bool lcdControlMotor = false;
bool lcdControlDirInvert = false;

bool readyForMotor = false;

bool buttonWasJustPushed = false;

int directionMultiplier = 1;


/**
* 01 = home
* 10 = menu 1 item 1
* 11 = menu 1 item 2
* 20 = menu 2 item 1
* etc.
*/
int currentMenu = 10; 

String lcdMenu = "HOME";


byte elipsis[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10101,
  B00000
};

byte blockChar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte temp[] = {
  B01100,
  B01100,
  B01100,
  B01100,
  B01100,
  B10010,
  B10010,
  B01100
};

byte motor[] = {
  B01100,
  B00110,
  B00100,
  B01100,
  B00110,
  B00100,
  B01100,
  B00110
};

byte arrow[] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

void clearLine(int line) {
  lcd.setCursor(0, line);
  for(byte n = 0; n < 16; n++) {
    lcd.print(" ");
  }
}

bool getAnyButton() {
  return(
    digitalRead(BUTTON_DOWN_PIN) == LOW ||
    digitalRead(BUTTON_UP_PIN) == LOW ||
    digitalRead(BUTTON_LEFT_PIN) == LOW ||
    digitalRead(BUTTON_RIGHT_PIN) == LOW ||
    digitalRead(BUTTON_OK_PIN) == LOW
  );
}

void lcdHome(byte status, int temp, int motor, int setpoint) {

  if(
      (
        !((lcdLastTemp - 1) <= temp <= (lcdLastTemp + 1))||
        lcdLastStatus != status ||
        lcdLastMotorSpeed != motor ||
        lcdLastSetpoint != setpoint
      ) || 
      (lcdLastMenu =! currentMenu) ||
      getAnyButton()) {

    switch((currentMenu / 10)) {
      case 1:
        clearLine(0);
        clearLine(1);
        delay(100);
        Serial.print("ehs");
        //delay(1);
        lcd.setCursor(0,0);
        lcd.print("reFil");
        switch(status) {
          default: //fault
            lcd.setCursor((16-5),0);
            lcd.print("FAULT");
          break;
          case 1: //ok
            lcd.setCursor((16-2),0);
            lcd.print("OK");
          break;
          case 2:
            lcd.setCursor((16-3),0);
            lcd.print("RUN");
          break;
        }
        lcd.setCursor(0,1);
        lcd.printByte(2);
        lcd.print(temp);

        if(setpoint != 0) {
          if(temp<100) {
            lcd.setCursor(3,1);
            lcd.printByte(4);
            lcd.print(setpoint);
          } else {
            lcd.setCursor(4,1);
            lcd.printByte(4);
            lcd.print(setpoint);            
          }
        }

        lcd.setCursor(10,1);
        lcd.printByte(3);
        lcd.print(motor);
        lcd.print("%");


      break;
      case 2:
        clearLine(0);
        clearLine(1);
        delay(100);
        lcd.setCursor(0,0);
        lcd.print("Menu:");
        switch(currentMenu%10) {
          case 0:
            if(lcdLastControlHeaters =! lcdControlHeaters) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Heaters:");
            if(lcdControlAutoExtrusion) {
              lcd.setCursor(16-1, 1);
              lcd.print("-");
            } else if(lcdControlHeaters) {
              lcd.setCursor(16-2, 1);
              lcd.print("ON");
            } else {
              lcd.setCursor(16-3, 1);
              lcd.print("OFF");         
            }
          break;
          case 1:
            if(lcdLastControlFans =! lcdControlFans) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Fans:");
            if(lcdControlAutoExtrusion) {
              lcd.setCursor(16-1, 1);
              lcd.print("-");
            } else if(lcdControlFans) {
              lcd.setCursor(16-2, 1);
              lcd.print("ON");
            } else {
              lcd.setCursor(16-3, 1);
              lcd.print("OFF");         
            }
          break;
          case 2:
            if(lcdLastControlSpooler =! lcdControlSpool) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Spooler:");
            if(lcdControlAutoExtrusion) {
              lcd.setCursor(16-1, 1);
              lcd.print("-");
            } else if(lcdControlSpool) {
              lcd.setCursor(16-2, 1);
              lcd.print("ON");
            } else {
              lcd.setCursor(16-3, 1);
              lcd.print("OFF");         
            }
          break;
          case 3:
            if(lcdLastControlPuller =! lcdControlPuller) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Puller:");
            if(lcdControlAutoExtrusion) {
              lcd.setCursor(16-1, 1);
              lcd.print("-");
            } else if(lcdControlPuller) {
              lcd.setCursor(16-2, 1);
              lcd.print("ON");
            } else {
              lcd.setCursor(16-3, 1);
              lcd.print("OFF");         
            }
          break;
          case 4:
            if(lcdLastControlGantry =! lcdControlGantry) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Gantry:");
            if(lcdControlAutoExtrusion) {
              lcd.setCursor(16-1, 1);
              lcd.print("-");
            } else if(lcdControlGantry) {
              lcd.setCursor(16-2, 1);
              lcd.print("ON");
            } else {
              lcd.setCursor(16-3, 1);
              lcd.print("OFF");         
            }
          break;
          case 6:
            if(lcdLastControlAutoExtrusion =! lcdControlAutoExtrusion) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Mode:");
            if(lcdControlAutoExtrusion) {
              lcd.setCursor(16-4, 1);
              lcd.print("AUTO");
            } else {
              lcd.setCursor(16-6, 1);
              lcd.print("MANUAL");         
            }
          break;
          case 5:
            if(lcdLastControlMotor =! lcdControlMotor) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Motor:");
            if(lcdControlAutoExtrusion) {
              lcd.setCursor(16-1, 1);
              lcd.print("-");
            } else if(lcdControlMotor) {
              if(readyForMotor) {
                lcd.setCursor(16-2, 1);
                lcd.print("ON");
              } else {
                lcd.setCursor(16-4, 1);
                lcd.print("WAIT");                
              }

            } else {
              lcd.setCursor(16-3, 1);
              lcd.print("OFF");         
            }
          break;
          case 7:
            if(lcdLastControlDirInvert =! lcdControlDirInvert) {
              clearLine(1);
            }
            lcd.setCursor(0, 1);
            lcd.print("Dir:");
            if(!lcdControlDirInvert) {
              lcd.setCursor(16-5, 1);
              lcd.print("SPOOL");

            } else {
              lcd.setCursor(16-7, 1);
              lcd.print("UNSPOOL");         
            }
          break;
        }
      break; 
    }
    lcdLastMenu = currentMenu;
    lcdLastControlAutoExtrusion = lcdControlAutoExtrusion;
    lcdLastControlFans = lcdControlFans;
    lcdLastControlGantry = lcdControlGantry;
    lcdLastControlPuller = lcdControlPuller;
    lcdLastControlSpooler = lcdControlSpool;
    lcdLastControlHeaters = lcdControlHeaters;
    lcdLastControlMotor = lcdControlMotor;
    lcdLastControlDirInvert = lcdControlDirInvert;

    lcdLastUpdate = millis();
    lcdLastTemp = temp;
    lcdLastMotorSpeed = motor;
    lcdLastStatus = status;
    lcdLastSetpoint = setpoint;
  }
  
}

void initLCD() {
  pinMode(FAULT_LED_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("reFil 0.9");
  lcd.setCursor(0,1);
  lcd.createChar(0, elipsis);
  lcd.setCursor(0,1);
  lcd.print("LCD");
  lcd.printByte(0);

  lcd.createChar(1, blockChar);
  lcd.createChar(2, temp);
  lcd.createChar(3, motor);
  lcd.createChar(4, arrow);

  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OK_PIN, INPUT_PULLUP);  

  lcd.setCursor(8,1);
  lcd.printByte(1);
  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("reFil 0.9");
  lcd.setCursor(0,1);
  lcd.print("MOTION");
  lcd.printByte(0);
  lcd.setCursor(8,1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  //lcd.printByte(1);

  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("reFil 0.9");
  lcd.setCursor(0,1);

  lcd.print("TEST");
  lcd.printByte(0);
  lcd.setCursor(8,1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  //lcd.printByte(1);
  //lcd.printByte(1);
  //lcd.printByte(1);
  delay(1000);
  digitalWrite(FAULT_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(STATUS_LED_PIN, LOW);
  digitalWrite(FAULT_LED_PIN, LOW);
  delay(1000);
  digitalWrite(FAN_PIN, HIGH);
  delay(5000);
  digitalWrite(FAN_PIN, LOW);

  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("reFil 0.9");
  lcd.setCursor(0,1);

  lcd.print("CAL");
  lcd.printByte(0);
  lcd.setCursor(8,1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);

  blinkCurrent = millis();
  blinkLast = millis();

  //lcdHome(2, 0, 100);
}

/**
* 0 = full fault
* 1 = idle
* 2 = running
* 10 = heater fault
*/
void updateLEDs(byte state) {
  blinkCurrent = millis();

  switch(state) {
    default:
      digitalWrite(FAULT_LED_PIN, HIGH);
      digitalWrite(STATUS_LED_PIN, LOW);
    break;
    case 1:
      digitalWrite(FAULT_LED_PIN, LOW);
      digitalWrite(STATUS_LED_PIN, LOW);
    break;
    case 2:
      digitalWrite(FAULT_LED_PIN, LOW);
      digitalWrite(STATUS_LED_PIN, HIGH);    
    break;
    case 10:
      if((blinkCurrent-blinkLast) > 500) {
        if(blinkState) {
          digitalWrite(FAULT_LED_PIN, HIGH);
          digitalWrite(STATUS_LED_PIN, HIGH);
          blinkState = false;
        } else {
          digitalWrite(FAULT_LED_PIN, LOW);
          digitalWrite(STATUS_LED_PIN, LOW);         
          blinkState = true;
        }
        blinkLast = blinkCurrent;
      } else {
        blinkCurrent = millis();
      }
  }
}

void updateMenuFromButtons() {
  if(!buttonWasJustPushed) {//(millis() - lastButtonUpdate) >= buttonUpdateDelay) {
    buttonWasJustPushed = true;
    if((digitalRead(BUTTON_DOWN_PIN) == LOW) && currentMenu%10 != MENU_ITEMS) {
      currentMenu += 1;
    } else if((digitalRead(BUTTON_UP_PIN) == LOW) && currentMenu%10 != 0) {

      currentMenu += -1;    
    } else if((digitalRead(BUTTON_LEFT_PIN) == LOW) && currentMenu/10 != 1 && currentMenu%10 != 1) {
      currentMenu += -10;
    } else if((digitalRead(BUTTON_RIGHT_PIN) == LOW) && currentMenu/10 != MENU_DEPTH && currentMenu%10 != 1) {
      currentMenu += 10;
    } else if ((digitalRead(BUTTON_OK_PIN) == LOW) && (currentMenu/10 != 1)) {
      switch(currentMenu%10) {
        case 0:
          lcdControlHeaters = !lcdControlHeaters;
        break;
        case 1:
          lcdControlFans = !lcdControlFans;
        break;
        case 2:
          lcdControlSpool = !lcdControlSpool;
        break;
        case 3:
          lcdControlPuller = !lcdControlPuller;
        break;
        case 4:
          lcdControlGantry = !lcdControlGantry;
        break;
        case 6:
          lcdControlAutoExtrusion = !lcdControlAutoExtrusion;
          lcdControlHeaters = false;
          lcdControlFans = false;
          lcdControlSpool = false;
          lcdControlPuller = false;
          lcdControlGantry = false;
        break;
        case 5:
          lcdControlMotor = !lcdControlMotor;
        break;
        case 7:
          lcdControlDirInvert = !lcdControlDirInvert;
          if(lcdControlDirInvert) {
            directionMultiplier = -1;
          } else {
            directionMultiplier = 1;
          }
        break;
        default:
        break;
      }
    }
    // if(getAnyButton()) {
    //   lastButtonUpdate = millis();    

    // }
  }
  if(!getAnyButton()) {
    buttonWasJustPushed = false;
  }
  
}




void lcdUpdateMenu() {

}