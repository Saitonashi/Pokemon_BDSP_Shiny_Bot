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

//HOME BUTTON
void press_Home_button() {
  //Press button
  for (pos = min_pos; pos <= max_pos; pos++) {
    HOME_Servo.write(pos);
    delay(15);
  }

  //Go back
  for (pos = max_pos; pos >= min_pos; pos--) {
    HOME_Servo.write(pos);
    delay(15);
  }
}

//A BUTTON
void press_A_button() {
  //Press button
  for (pos = min_pos; pos <= max_pos; pos++) {
    A_Servo.write(pos);
    delay(15);
  }

  //Go back
  for (pos = max_pos; pos >= min_pos; pos--) {
    A_Servo.write(pos);
    delay(15);
  }
}

//X BUTTON
void press_X_button() {
  //Press button
  for (pos = min_pos; pos <= max_pos; pos++) {
    X_Servo.write(pos);
    delay(15);
  }

  //Go back
  for (pos = max_pos; pos >= min_pos; pos--) {
    X_Servo.write(pos);
    delay(15);
  }
}

//Up BUTTON
void press_Up_button() {
  //Press button
  for (pos = min_pos; pos <= max_pos; pos++) {
    Up_Servo.write(pos);
    delay(15);
  }

  //Go back
  for (pos = max_pos; pos >= min_pos; pos--) {
    Up_Servo.write(pos);
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

  //Reset servos posiotion
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
    press_A_button();
    delay(1500);
    press_A_button();

    //Wait for load game
    delay(25000);
    press_A_button();
    delay(3000);
    press_A_button();
    delay(13000);

    //Interact with Dialga
    press_Up_button();
    delay(3000);
    press_A_button();
    delay(1000);
    press_A_button();
    delay(4000);

    //Check if Dialga is shiny
    Color_Sensor.getRawData(&red, &green, &blue, &c);
    //Serial.println("R: " + red + " G: " + green + " B: " + blue);
    Serial.print("R: " + red);
    Serial.print(" G: " + green);
    Serial.println(" B: " + blue);

    if (blue < 100) {
      shiny = true;
      Serial.println("SHINY!!!");
      digitalWrite(8, HIGH);
      continue;
    } else {
      Serial.println("Non Shiny :<");
    }

    //Exit game
    if (!shiny) {
      press_Home_button();
      delay(1000);
      press_X_button();
      delay(1000);
      press_A_button();
      delay(1000);
      resets++;
    }
  }
}
