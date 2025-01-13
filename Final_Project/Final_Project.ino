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
//DFRobot_HX711_I2C Scale(&Wire,/*addr=*/0x64);
DFRobot_HX711_I2C Scale;
float weight;
char currCoin;
float prevVal
float currVal;
int tim = 250;

char clearArray[] = " Value Cleared! ";
int clearPosCount;
void clearPrint() {
  lcd.setCursor(15,0);
  for (clearPosCount = 0; clearPosCount < 15; clearPosCount++) {
     lcd.scrollDisplayLeft();
     lcd.print(clearArray[clearPosCount]);
     delay(tim);
  }
  delay(2000);
  lcd.clear();
}

char initArray1[] = "The initialization of the chip failed, please";
int initPosCount1;
void initPrint1() {
  lcd.setCursor(15,0);
  for (initPosCount1 = 0; initPosCount1 < 44; initPosCount1++) {
     lcd.scrollDisplayLeft();
     lcd.print(initArray1[initPosCount1]);
     delay(tim);
  }
  delay(2000);
  lcd.clear();
}

char initArray2[] = "confirm whether the chip connection is correct."
int initPosCount2; 
void initPrint2() {
  lcd.setCursor(15,1);
  for (initPosCount2 = 0; initPosCount2 < 46; initPosCount2++) {
     lcd.scrollDisplayLeft();
     lcd.print(initArray2[initPosCount2]);
     delay(tim);
  }
  delay(2000);
  lcd.clear();
}

char weirdArray[] = "Weird / No Coin ";
int weirdPosCount;
void weirdPrint() {
  lcd.setCursor(15,0);
  for (weirdPosCount = 0; weirdPosCount < 16; weirdPosCount++) {
    lcd.scrollDisplayLeft();
    lcd.print(weirdArray[weirdPosCount]);
    delay(tim);
  }
  delay(2000);
  lcd.clear();
}

char scaleArray[] = "";
int scalePosCount;
void scalePrint() {
  lcd.setCursor(15,0);
  for (scalePosCount = 0; scalePosCount < 16; scalePosCount++) {
    lcd.scrollDisplayLeft();
    lcd.print(scaleArray[scalePosCount]);
    delay(tim);
  }
  delay(2000);
  lcd.clear();
}

char valPenny[] = "";
int pennyPosCount;

char valNickel[] = "";
int nickelPosCount;

char valDime[] = "";
int dimePosCount;

char valQuarter[] = "";
int quarterPosCount;



#include <LiquidCrystal.h>
#include <DFRobot_HX711_I2C.h>
#define BUTTON1 7

bool prevButton1;
bool currButton1;

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
    Scale.tare();
    prevVal = currVal;
    currVal = 0;
    clearPrint();
    prevButton1 = currButton1;

   } else if (currButton1 == false && currButton1 != prevButton1) {
    prevButton1 = currButton1;
  }
  LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
}


void setup() {
  pinMode(BUTTON1, INPUT_PULLUP);
  lcd.begin(16, 2);
  prevButton1 = false;
  Serial.begin(9600);
  while (!Scale.begin()) {
    initPrint1();
    initPrint2();
    delay(1000);
  }
  Scale.setCalibration(2236.f);
  Scale.peel();
}

void loop() {
  checkState();
  weight = Scale.readWeight();
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
    weirdPrint();
  }
  scalePrint();

  Serial.print("You currently have $");
  Serial.print(currVal);
  Serial.println("!");
  prevVal = currVal;
  currVal = 0;
  delay(5000);
}