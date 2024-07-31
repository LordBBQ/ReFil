#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define printByte(args)  write(args)
#define FAULT_LED_PIN 57
#define STATUS_LED_PIN 58

#define FAN_PIN 52


//LCD main code
LiquidCrystal_I2C lcd(0x20,20,4); //define lcd on port 0x20

unsigned long blinkLast = 0;
unsigned long blinkCurrent = 0;

unsigned long lcdLastUpdate = 0;
unsigned long lcdCurrentTime = 0;

bool blinkState = false;

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

void clearLine(int line) {
  lcd.setCursor(0, line);
  for(byte n = 0; n < 16; n++) {
    lcd.print(" ");
  }
}

void lcdHome(byte status, int temp, int motor) {
  lcdCurrentTime = millis();
  if((lcdCurrentTime-lcdLastUpdate) > 1000) {
    clearLine(0);
    clearLine(1);
    delay(1);
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

      lcd.setCursor(5,1);
      lcd.printByte(3);
      lcd.print(motor);
      lcd.print("%");

      lcdLastUpdate = millis();
  } else {

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
  lcd.printByte(1);

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
  lcd.printByte(1);
  lcd.printByte(1);
  lcd.printByte(1);
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