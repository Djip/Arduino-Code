#include <HCWS2812.h>

#include <Servo.h>

#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;
HCWS2812 hcws2812;


// Use this code to test your motor with the Arduino board:

// if you need PWM, just use the PWM outputs on the Arduino
// and instead of digitalWrite, you should use the analogWrite command

// --------------------------------------------------------------------------- Motors
Servo servo;

int motor[] = {5, 6};
int pos = 0;

// --------------------------------------------------------------------------- Setup
void setup() {
  servo.attach(10);
  pinMode(10, OUTPUT); // change this to 53 on a mega  // don't follow this!!
  digitalWrite(10, HIGH);
  Serial.begin(9600);
  // Setup motors
  int i;
  for(i = 0; i < 2; i++){
    pinMode(motor[i], OUTPUT);
  }

  tmrpcm.speakerPin = 9;
  if (!SD.begin(10)) {
    Serial.println("SD fail");
    return;
  }
  //tmrpcm.setVolume(6);
  //tmrpcm.play("music.wav");

  hcws2812.SetBG(0,0,0);
  hcws2812.ClearBuffer();
}

// --------------------------------------------------------------------------- Loop
void loop() {
  /*
  for (int i = 0; i < 16; i++)
  {
    RGBBuffer[RED][i] = random(0, 255);
    RGBBuffer[GREEN][i] = random(0, 255);
    RGBBuffer[BLUE][i] = random(0, 255);
  }
  hcws2812.Refresh();
  delay(300);
*/

/*
  for (pos = 60; pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 120; pos >= 60; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  */
drive_backward();
  Serial.println("Type 1 to drive forward, 2 for backwards and 3 to stop");
  int number = Serial.parseInt();
  switch (number) {
    case 1:
      drive_forward();
      break;
    case 2:
      drive_backward();
      break;
    case 3:
      motor_stop();
      break;
    default :
        break;
  }
  
}

// --------------------------------------------------------------------------- Drive

void motor_stop() {
  analogWrite(motor[0], 0);
  analogWrite(motor[1], 0);
  delay(25);
}

void drive_forward() {
  analogWrite(motor[0], 200);
}

void drive_backward() {
  analogWrite(motor[1], 255);
}
