#include "myStepper.h"


bool Stepper::begin(int dirPin, int speedPin) {
    AccelStepper stepper1(AccelStepper::DRIVER, dirPin, speedPin);

    this->stepper = stepper1;

    return true;
}

/*
@param newSpeed is the new speed of the motor in mm per second
*/
Stepper* Stepper::setSpeed(int newSpeed)
{   
    this->speed = (newSpeed*(*this).stepsPerRev)*((*this).pitch);       //convert from mm per sec to steps per sec

    return this;
}

/*
@param newPosition sets value of current position to the newPosition
*/
Stepper* Stepper::setPosition(int newPosition)
{
    this->position = newPosition;
    return this;
}
/*
@param new value for the steps per Revolution
*/
Stepper* Stepper::setStepsPerRev(int newStepsPerRev)
{
    this->stepsPerRev = newStepsPerRev;
    return this;
}

/*
@param pitch of the screw
*/
Stepper* Stepper::setPitch(int newPitch)
{
    this->pitch = newPitch;
    return this;
}


/*
getters
*/
int Stepper::getSpeed()
{
    return this->speed;
}

int Stepper::getPosition()
{
    return this->position;
}

int Stepper::getStepsPerRev()
{
    return this->stepsPerRev;
}

int Stepper::getPitch()
{
    return this->pitch;
}

void Stepper::goToStraight(int pos)
{
    int diff = abs(this->position - pos);
    unsigned long steps = (diff * (this->stepsPerRev)) / (this->pitch);
    if (this->position > pos && diff>0){
        this->position += step2mm(moveClockwise());
    }
    else if (this->position < pos && diff>0) {
        this->position += step2mm(moveCounterClockwise());
    }

}

bool Stepper::goToWithBreaks(int pos, int travelSpeed, int breakDistance, int breakInterval) {
    if (pos == this->position) return true;
    else {
        if (abs(pos) >= abs(breakDistance + this->position) &&(this->stopTime+breakInterval<millis() )) {
            if (goToWithPosSpeed(this->position + breakDistance, travelSpeed)) {
                this->stopTime = millis();
            }
        }
        else if (this->stopTime + breakInterval < millis()) {
            if (goToWithPosSpeed(pos, travelSpeed)) {
                this->stopTime = millis();
            }
        }
        return goToWithBreaks(pos, travelSpeed, breakDistance, breakInterval);
    }

}

bool Stepper::goToWithPosSpeed(int pos, int travelSpeed)
{

    int diff = abs(this->position - pos);
    unsigned long steps = (diff * (this->stepsPerRev)) / (this->pitch);
    if (this->position > pos && diff > 0) {
        this->position += step2mm(moveClockwise());
    }
    else if (this->position < pos && diff>0) {
        this->position += step2mm(moveCounterClockwise());
    }
    if (diff == 0)return 0;
    return false;
}


bool Stepper::goToWithPosSpeedOscillator(int pos, int travelSpeed, int oscillations) {
    this->cycles = oscillations;
    if (oscillations == 0 or oscillations < 0) {
        this->cycles = 0;
        return true;
    }
    else {
        if (goToWithPosSpeed(pos, travelSpeed)) {
            oscillations--;
            travelSpeed = -travelSpeed;
            pos = -pos;
        }
        return goToWithPosSpeedOscillator(pos, travelSpeed, oscillations);
    }

}
bool Stepper::goToWithBreaksOscillator(int pos, int travelSpeed, int breakDistance, int breakInterval, int oscillations) {
    this->cycles = oscillations;
    if (oscillations == 0 or oscillations < 0) {
        this->cycles = 0;
        return true;
    }
    else {
        if (goToWithBreaks(pos, travelSpeed, breakDistance, breakInterval)) {
            oscillations--;
            travelSpeed = -travelSpeed;
            pos = -pos;
        }
        return goToWithBreaksOscillator(pos, travelSpeed, breakDistance, breakDistance, oscillations);
    }

}

int Stepper::moveClockwise()
{
    this->lastMove = millis();
    this->stepper.setSpeed(this->speed);
    this->stepper.runSpeed();

    return (millis() - this->lastMove) * this->speed;
 
}

int Stepper::moveCounterClockwise()
{
    this->lastMove = millis();
    this->stepper.setSpeed(-(this->speed));
    this->stepper.runSpeed();
    return -(millis() - this->lastMove) * this->speed;
}

float Stepper::step2mm(int stp)
{   

    return ((float)(stp*(this->pitch)))/this->stepsPerRev;
}

int Stepper::mm2step(float mm)
{
    return (int)(mm*this->stepsPerRev/this->pitch);
}
