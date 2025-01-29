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
    lcd.print("The initialization of the chip failed, please confirm whether the chip connection is correct");
    delay(3000);
    lcd.clear();
  }
  scale.setCalibration(2236.f);
  scale.peel();
  delay(1000);
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
}

bool isPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    loopFlag = false;
    return true;
  } else {
    return false;
  }
}

void loop() {
  delay(5000);
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
  lcd.setCursor(0,0);
  lcd.print("Weight: ");
  lcd.print(weight);
  lcd.print("g.");
  delay(3000);
  lcd.clear();
  if (weight >= 2.4 && weight <= 2.6) {
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
  } else if (weight >= 4.9 && weight <= 5.1) {
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
  } else if (weight >= 2.168 && weight <= 2.368) {
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
  } else if (weight >= 5.57 && weight <= 5.77) {
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