#ifndef STEPPER_H
#define STEPPER_H

#define ENABLE 1
#define DISABLE 0
#define FORWARD 1
#define BACKWARD 0
#define START 1
#define END 0
#define NUMBER_STEPPERS 3

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "GPIO.h"


#define reset()             \
    cli();                   \
    wdt_enable(WDTO_15MS);  \
    while (1);


float baseTime;
int counter[NUMBER_STEPPERS];
int homingSuccess[NUMBER_STEPPERS];
int stepsCounter[NUMBER_STEPPERS];

typedef struct {
  char enablePort;
  int enablePin;
  char dirPort;
  int dirPin;
  char stepPort;
  int stepPin;
  int position;
  int maxSteps;
  int enable;
  int dir;
  int steps;
  int revoltionPerMinute;
  float degreesPerStep;
  int homeRoutine;
} Stepper;

Stepper SteppersArray[NUMBER_STEPPERS];

Stepper createStepper(char enPort, int enPin, char dPort,
   int dPin, char stPort, int stPin, float degreesPerStep, int rpm);
void enableMotors(Stepper PaPArray[], int number_steppers);
void disableMotors(Stepper PaPArray[], int number_steppers);
void rotateSteps(Stepper *PaP, int steps, int dir);
void stopMotors(Stepper PaPArray[], int number_steppers);
void setSpeed(Stepper *PaP, int rpm);
void timer1Handler();
void setBaseTime(float base);
void homing(Stepper PaPArray[], int number_steppers);
void raceEnd(int motorNumber, int class);
void absolutePosition(Stepper *PaP, int position);
void relativePosition(Stepper *PaP, int position);
int getRelative(Stepper *PaP);
int checkHoming(Stepper PaPArray[], int number_steppers);


#endif
