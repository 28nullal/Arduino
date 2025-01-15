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
//DFRobot_HX711_I2C scale(&Wire,/*addr=*/0x64);
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
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

void setup() {
  scale.begin();
  pinMode(BUTTON1, INPUT_PULLUP);
  lcd.begin(16, 2);
  prevButton1 = false;
  Serial.begin(9600);
  while (!scale.begin()) {
    lcd.print("The initialization of the chip failed, please confirm whether the chip connection is correct");
    delay(1000);
  }
  scale.setCalibration(2236.f);
  scale.peel();
}

// in INPUT_PULLUP mode, if the signal is HIGH
// then the button is unpressed.
bool isPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    return true;
  } else {
    return false;
  }
}

// this method checks the system to see
// if its state should change.
void checkState() {
  // check button 1.
  currButton1 = isPressed(BUTTON1);
  if (currButton1 == true && prevButton1 == false) {
    scale.peel();
    prevVal = currVal;
    currVal = 0;
    lcd.print("Value Cleared!");
    prevButton1 = currButton1;

  } else if (currButton1 == false && currButton1 != prevButton1) {
    prevButton1 = currButton1;
  }
}

void loop() {
  checkState();
  weight = scale.readWeight();
  if (weight >= 2.4 && weight <= 2.6) {
    currCoin = "penny";
  } else if (weight >= 4.9 && weight <= 5.1) {
    currCoin = "nickel";
  } else if (weight >= 2.168 && weight <= 2.368) {
    currCoin = "dime";
  } else if (weight >= 5.57 && weight <= 5.77) {
    currCoin = "quarter";
  } else {
    currCoin = "none";
  }

  if (currCoin == "penny") {
    currVal = prevVal + 0.01;
  } else if (currCoin == "nickel") {
    currVal = prevVal + 0.05;
  } else if (currCoin == "dime") {
    currVal = prevVal + 0.1;
  } else if (currCoin == "quarter") {
    currVal = prevVal + 0.25;
  } else if (currCoin == "half dollar") {
    currVal = prevVal + 0.5;
  } else if (currCoin == "dollar") {
    currVal = prevVal + 1;
  } else if (currCoin == "none") {
    lcd.print("Weird / No Coin");
  }
  lcd.print("There is a");
  lcd.print(currCoin);
  lcd.print("on the scale");

  lcd.print("Now, you have $");
  lcd.print(currVal);
  Serial.print("Now, you have $");
  Serial.print(currVal);
  prevVal = currVal;
  currVal = 0;
  delay(5000);
}