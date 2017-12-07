/*
 Stepper Motor Control - one step at a time

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor will step one step at a time, very slowly.  You can use this to
 test that you've got the four wires of your stepper wired to the correct
 pins. If wired correctly, all steps should be in the same direction.

 Use this also to count the number of steps per revolution of your motor,
 if you don't know it.  Then plug that number into the oneRevolution
 example to see if you got it right.

 Created 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
int analogPin1 = A4;
int analogPin2 = A2;
bool locked = true;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // initialize the serial port:
  Serial.begin(9600);

    // initialize digital pin LED_BUILTIN as an output.
  pinMode(07, OUTPUT);
  digitalWrite(07, HIGH);
}

void loop() {

  int a1Value = (analogRead(analogPin1) / 1023) * 5;
  Serial.println(a1Value);
  if(a1Value > 2.5)
  {
    for(int i = 0; i < 50; i++){
        myStepper.step(-1);
        delay(10);
    }

    locked = false;
  }

  /*
  else if(a1Value > 2.5 && !locked)
  {
      for(int i =0; i < 50; i++){
        myStepper.step(1);
        delay(10);
    }

    locked = true;
  }
  */
}
