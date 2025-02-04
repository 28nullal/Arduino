/*!
 * @file readWeight.ino
 * @brief Get the weight of the object, press the RST button on the module, the 
 * @n program will automatically remove the skin
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-12-26
 * @https://github.com/DFRobot/DFRobot_HX711_I2C
 * @brief Constructor 
 * @param pWire I2c controller
 * @param addr  I2C address(0x64/0x65/0x660x67)
 */
#include <LiquidCrystal.h>
#include <DFRobot_HX711_I2C.h>
#define BUTTON1 7
DFRobot_HX711_I2C scale;
int tim = 250;
bool prevButton1;
bool currButton1;
float weight;
char currCoin;
float prevVal;
float currVal;
bool loopFlag;
const int rs = 4, en = 6, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  loopFlag = true;
  prevButton1 = false;
  scale.begin();
  pinMode(BUTTON1, INPUT_PULLUP);
  lcd.begin(16, 2);
  while (!scale.begin()) {
    lcd.setCursor(0,0);
    lcd.print("Initialization");
    lcd.setCursor(0,1);
    lcd.print("Failed");
    delay(3000);
    lcd.clear();
  }
  //calibration();
  scale.setCalibration(2024.5);
  lcd.setCursor(0,0);
  lcd.print("Calibration");
  lcd.setCursor(0,1);
  lcd.print("Completed!");
  delay(3000);
  lcd.setCursor(0,0);
  lcd.print("Press button to");
  lcd.setCursor(0,1);
  lcd.print("start weighing ");
  delay(3000);
  lcd.clear();
  while (loopFlag) {
    isPressed(BUTTON1);
    delay(100);
  }
  loopFlag = true;
  lcd.print("Weighing Started");
  delay(1000);
  lcd.clear();
}

bool isPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    loopFlag = false;
    return true;
  } else {
    return false;
  }
}

/*
void calibration() {
  scale.setCalWeight(300);
  scale.setThreshold(10);  

  lcd.setCursor(0,0);
  lcd.print("Put 300g weight");
  lcd.setCursor(0,1);
  lcd.print("on scale in 5s");
  scale.enableCal();
  delay(3000);
  lcd.clear();
  long time1 = millis();
  while(!scale.getCalFlag()){
       delay(1000);
       if((millis()-time1) > 7000){ 
          lcd.setCursor(0,0);
          lcd.print("Calibration");
          lcd.setCursor(0,1);
          lcd.print("Failed!");
          delay(3000);
          lcd.clear();
          break;
       }
  }
  lcd.setCursor(0,0);
  lcd.print("The calibration");
  lcd.setCursor(0,1);
  lcd.print("value is ");
  lcd.print(scale.getCalibration());
  delay(3000);
  lcd.clear();
  scale.setCalibration(scale.getCalibration());
}
*/
void loop() {
  lcd.setCursor(0,0);
  lcd.print("Press button");
  lcd.setCursor(0,1);
  lcd.print("to clear balance");
  delay(3000);
  lcd.clear();
  currButton1 = isPressed(BUTTON1);
  if (currButton1 == true && prevButton1 == false) {
    scale.peel();
    prevVal = currVal;
    currVal = 0;
    lcd.setCursor(0,0);
    lcd.print("Value Cleared!");
    delay(3000);
    lcd.clear();
    prevButton1 = currButton1;

  } else if (currButton1 == false && currButton1 != prevButton1) {
    prevButton1 = currButton1;
  }
  weight = scale.readWeight();
  weight = weight,1;
  lcd.setCursor(0,0);
  lcd.print("Weight: ");
  lcd.print(weight);
  lcd.print("g.");
  delay(3000);
  lcd.clear();
  if (weight >= 2.385 && weight <= 2.65) {
    currCoin = "penny";
    lcd.setCursor(0,0);
    lcd.print("A penny is on");
    lcd.setCursor(0,1);
    lcd.print("the scale");
    currVal = prevVal + 0.01;
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Take coin off");
    delay(3000);
    lcd.clear();
  } else if (weight >= 4.85 && weight <= 5.15) {
    currCoin = "nickel";
    lcd.setCursor(0,0);
    lcd.print("A nickel is on");
    lcd.setCursor(0,1);
    lcd.print("the scale");
    currVal = prevVal + 0.05;
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Take coin off");
    delay(3000);
    lcd.clear();
  } else if (weight >= 2.118 && weight <= 2.384) {
    currCoin = "dime";
    lcd.setCursor(0,0);
    lcd.print("A dime is on");
    lcd.setCursor(0,1);
    lcd.print("the scale");
    currVal = prevVal + 0.10;
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Take coin off");
    delay(3000);
    lcd.clear();
  } else if (weight >= 5.52 && weight <= 5.82) {
    currCoin = "quarter";
    lcd.setCursor(0,0);
    lcd.print("A quarter is on");
    lcd.setCursor(0,1);
    lcd.print("the scale");
    currVal = prevVal + 0.25;
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Take coin off");
    delay(3000);
    lcd.clear();
  } else {
    currCoin = "none";
    lcd.print("Weird / No Coin");
    currVal = prevVal + 0;
    delay(3000);
    lcd.clear();
  }

  lcd.setCursor(0,0);
  lcd.print("You have $");
  lcd.print(currVal);
  delay(3000);
  lcd.clear();
  prevVal = currVal;
  currVal = 0;
}