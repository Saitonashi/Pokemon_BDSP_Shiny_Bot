//Servo pins: 6(HOME), 9(A), 10(X), 11(UP)
//LED pin: 8
//Color Sensor pin: A4(SDA), A5(SCL)

#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

Servo HOME_Servo;
Servo A_Servo;
Servo X_Servo;
Servo Up_Servo;
int max_pos = 30;
int min_pos = 0;
int pos;

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */
Adafruit_TCS34725 Color_Sensor = Adafruit_TCS34725();
uint16_t red, green, blue, c; //, colorTemp, lux;

int resets = 0;
bool shiny = false;

//PRESS BUTTON
//press_button(HOME_Servo);
//press_button(A_Servo);
//press_button(X_Servo);
//press_button(Up_Servo);
void press_button(Servo servo) {
  for (pos = min_pos; pos <= max_pos; pos++) {
    servo.write(pos);
    delay(15);
  }
  for (pos = max_pos; pos >= min_pos; pos--) {
    servo.write(pos);
    delay(15);
  }
}

void setup() {
  Serial.begin(9600);

  //Init servo
  HOME_Servo.attach(6);
  A_Servo.attach(9);
  X_Servo.attach(10);
  Up_Servo.attach(11);

  //Reset servos position
  HOME_Servo.write(0);
  delay(100);
  A_Servo.write(0);
  delay(100);
  X_Servo.write(0);
  delay(100);
  Up_Servo.write(0);
  delay(100);

  //Check color sensor connection
  if (Color_Sensor.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  //Init LED
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
}

void loop() {
  while (!shiny) {
    //Show reset count
    Serial.println("Reset " + resets);

    //Lauch game
    press_button(A_Servo);
    delay(1500);
    press_button(A_Servo);

    //Wait for load game
    delay(25000);
    press_button(A_Servo);
    delay(3000);
    press_button(A_Servo);
    delay(13000);

    //Interact with Dialga
    press_button(Up_Servo);
    delay(3000);
    press_button(A_Servo);
    delay(1000);
    press_button(A_Servo);
    delay(4000);

    //Check Dialga color
    Color_Sensor.getRawData(&red, &green, &blue, &c);
    Serial.print("R: " + red);
    Serial.print(" G: " + green);
    Serial.println(" B: " + blue);

    //If Diagla is not blue, is shiny
    if (blue < 100) {
      shiny = true;
      Serial.println("SHINY!!!");
      digitalWrite(8, HIGH);    //Light up LED to communicate shiny pokemon shows up
      continue;
    } else {
      Serial.println("Non Shiny :<");
    }

    //Exit game
    if (!shiny) {   //condition for a security against resetting the game if shiny shows up
      press_button(HOME_Servo);
      delay(1000);
      press_button(X_Servo);
      delay(1000);
      press_button(A_Servo);
      delay(1000);
      resets++;
    }
  }
}
