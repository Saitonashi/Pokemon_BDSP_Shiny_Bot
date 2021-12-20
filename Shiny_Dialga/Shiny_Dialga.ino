//@author Saitonashi

//Servo pins: 6(HOME), 9(A), 10(X), 11(UP)
//LED pin: 4 (optional)
//Color Sensor pin: A4(SDA), A5(SCL)

#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

//Servos
Servo HOME_Servo;
Servo A_Servo;
Servo X_Servo;
Servo Up_Servo;
int pos;

//Color Sensor
/* Connect LED    to analog 0 (optional)
   Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5V DC
   Connect GROUND to common ground */
Adafruit_TCS34725 Color_Sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
uint16_t red, green, blue, c;   // colorTemp, lux;

//LCD
const int rs = 12, en = 13, d4 = 7, d5 = 5, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//EEPROM
uint32_t resets;
int eeAddress = 0;

bool shiny = false;

//---------------------------------------------------------------------
//PRESS HOME BUTTON
void press_Home_button() {
  for (pos = 90; pos <= 142; pos += 1) {
    HOME_Servo.write(pos);
    delay(5);
  }
  delay(100);
  for (pos = 142; pos >= 90; pos -= 1) {
    HOME_Servo.write(pos);
    delay(5);
  }
}

//PRESS A BUTTON
void press_A_button() {
  for (pos = 90; pos >= 38; pos -= 1) {
    A_Servo.write(pos);
    delay(5);
  }
  for (pos = 38 ; pos <= 90; pos += 1) {
    A_Servo.write(pos);
    delay(5);
  }
}

//PRESS X BUTTON
void press_X_button() {
  for (pos = 90; pos >= 40; pos -= 1) {
    X_Servo.write(pos);
    delay(5);
  }
  for (pos = 40; pos <= 90; pos += 1) {
    X_Servo.write(pos);
    delay(5);
  }
}

//PRESS UP BUTTON
void press_Up_button() {
  for (pos = 90; pos >= 40; pos -= 1) {
    Up_Servo.write(pos);
    delay(5);
  }
  delay(250);
  for (pos = 40; pos <= 90; pos += 1) {
    Up_Servo.write(pos);
    delay(5);
  }
}
//---------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  //Init LCD
  lcd.begin(16,2);  //(col,row)
  lcd.print("Shiny Bot V2.0");
  lcd.setCursor(0,1);
  //lcd.print("Poke bot 2.0");
  
  //Init servo
  HOME_Servo.attach(6);
  A_Servo.attach(9);
  X_Servo.attach(10);
  Up_Servo.attach(11);
  //Init LED
  pinMode(4, OUTPUT);
  pinMode(A0, OUTPUT);
  digitalWrite(4, HIGH);

  //Reset servos position
  Serial.println("Reset servos position");
  lcd.print("Reset servos pos");
  delay(4000);
  HOME_Servo.write(90);
  delay(500);
  A_Servo.write(90);
  delay(500);
  X_Servo.write(90);
  delay(500);
  Up_Servo.write(90);
  delay(500);

  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);

  //Check color sensor connection
  if (Color_Sensor.begin()) {
    Serial.println("Found sensor");
    lcd.print("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    lcd.print("No TCS34725 found");
    while (1);
  }

  //Read data from EEPROM
  EEPROM.get(eeAddress, resets);

  digitalWrite(A0, HIGH);
  digitalWrite(4, LOW);
}

void loop() {
  while (!shiny) {
    //Show reset count
    Serial.print("Reset ");
    Serial.print(resets);
    Serial.print(" ; ");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reset:");
    lcd.setCursor(6,0);
    lcd.print(resets);

    //Lauch game
    press_A_button();       //delay 500
    delay(1000);
    press_A_button();       //delay 500

    //Wait for load game
    delay(25000);
    press_A_button();       //delay 500
    delay(2500);
    press_A_button();       //delay 500
    delay(13000);

    //Interact with Dialga
    press_Up_button();      //delay 750
    delay(2000);
    press_A_button();       //delay 500
    delay(500);
    press_A_button();       //delay 500
    delay(4150);

    //Check Dialga color
    Color_Sensor.getRawData(&red, &green, &blue, &c);
    Serial.print("R: ");
    Serial.print(red, DEC);
    Serial.print(" G: ");
    Serial.print(green, DEC);
    Serial.print(" B: ");
    Serial.print(blue, DEC);
    Serial.print(" ; ");
    lcd.setCursor(0,1);
    lcd.print("B:");
    lcd.setCursor(2,1);
    lcd.print(blue, DEC);
    lcd.setCursor(6,1);
        
    //If Diagla is not blue, is shiny
    if (blue < 600) {
      shiny = true;
      Serial.println("SHINY!!!");
      lcd.print("SHINY!!!");
      digitalWrite(4, HIGH);    //Light up LED to communicate shiny pokemon shows up
      continue;
    } else {
      Serial.println("Non Shiny :<");
      lcd.print("Non Shiny");
    }

    //Exit game
    if (!shiny) {   //condition for a security against resetting the game if shiny shows up
      press_Home_button();  //delay 520
      delay(500);
      press_X_button();     //delay 500
      delay(500);
      press_A_button();     //delay 500
      delay(500);
      resets++;             
                            //sumary delay 58920ms
                            //61 resets per hour
                            //sumary delay after optymalization 54920ms (4 sec diffferent)
                            //65 resets per hour
      EEPROM.put(eeAddress, resets);
    }
  }
}
