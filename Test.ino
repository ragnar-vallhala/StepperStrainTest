/*
 Name:		Test.ino
 Created:	3/25/2023 10:53:15 PM
 Author:	ashut
*/

// the setup function runs once when you press reset or power the board
#include "myStepper.h"

Stepper motor;


void setup() {
	motor.begin(8,9);
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	motor.stepper.moveTo(500);
	Serial.println(motor.getPosition());
}
