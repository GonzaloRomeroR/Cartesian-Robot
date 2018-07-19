#include "stepper.h"

void enableMotors(Stepper PaPArray[], int number_steppers){
  for (int i = 0; i < number_steppers; i++){
    PaPArray[i].enable = ENABLE;
    PaPArray[i].steps = 0;
    counter[i] = 0;
    pinOff(PaPArray[i].enablePort, PaPArray[i].enablePin);

  }
}

void disableMotors(Stepper PaPArray[], int number_steppers){
  for (int i = 0; i < number_steppers; i++){
    PaPArray[i].enable = DISABLE;
    PaPArray[i].steps = 0;
    counter[i] = 0;
    pinOn(PaPArray[i].enablePort, PaPArray[i].enablePin);
  }
}


void stopMotors(Stepper PaPArray[], int number_steppers){
  for (int i = 0; i < number_steppers; i++){
    PaPArray[i].steps = 0;
  }
}

void setBaseTime(float base){
  baseTime = base;
}

void setSpeed(Stepper *PaP, int rpm){
  PaP->revoltionPerMinute = rpm;
}

void rotateSteps(Stepper *PaP, int steps, int dir){

  if (PaP->enable == ENABLE){
    PaP->steps = steps;
    if (dir == FORWARD){
      PaP->dir = FORWARD;
      pinOn(PaP->dirPort, PaP->dirPin);
      pinOn(PaP->stepPort, PaP->stepPin);
    }
    else if( dir == BACKWARD){
      PaP->dir = BACKWARD;
      pinOff(PaP->dirPort, PaP->dirPin);
      pinOn(PaP->stepPort, PaP->stepPin);
    }
  }
  else{
    printf("Order Failed: Motor is not enable\n");
  }
}


void homing(Stepper PaPArray[], int number_steppers){
    for (int i = 0; i < number_steppers; i++){
      PaPArray[i].homeRoutine = 1;
      int maxInt = INT_MAX;
      rotateSteps(&PaPArray[i], INT_MAX, FORWARD);
    }
}

int checkHoming(Stepper PaPArray[], int number_steppers){
  int success = 1;
  for (int i = 0; i < number_steppers; i++){
    if (PaPArray[i].homeRoutine == 1){
      success = 0;
    }
  }
  return success;
}

void absolutePosition(Stepper *PaP, int position){
  int difference =  position - PaP->position;
  if (difference > 0){
    rotateSteps(PaP, abs(difference), FORWARD);
  }
  else{
    rotateSteps(PaP, abs(difference), BACKWARD);
  }

}

int getRelative(Stepper *PaP){
  float relPosition = (float)PaP->position / PaP->maxSteps * 100.0 + 0.5;
  return (int)relPosition;

}

void relativePosition(Stepper *PaP, int percentage){
  float absPosition = (float)percentage / 100.0 * PaP->maxSteps;
  absolutePosition(PaP, (int)absPosition);
}

Stepper createStepper(char enPort, int enPin, char dPort,
   int dPin, char stPort, int stPin, float degreesPerStep, int rpm){
  Stepper motor;
  motor.enablePort = enPort;
  motor.enablePin = enPin;
  motor.dirPort = dPort;
  motor.dirPin = dPin;
  motor.stepPort = stPort;
  motor.stepPin = stPin;
  motor.enable = DISABLE;
  motor.degreesPerStep = degreesPerStep;
  motor.revoltionPerMinute = rpm;
  motor.steps = 0;
  motor.position = 0;
  motor.homeRoutine = 0;
  return motor;
}

void raceEnd(int motorNumber, int class){
  if (SteppersArray[motorNumber].homeRoutine == 1){
    if (class == START){
      int maxInt = INT_MAX;
      SteppersArray[motorNumber].dir = BACKWARD;
      rotateSteps(&SteppersArray[motorNumber],
        maxInt, SteppersArray[motorNumber].dir);
    }
    if (class == END){
      int maxInt = INT_MAX;
      SteppersArray[motorNumber].maxSteps = maxInt - SteppersArray[motorNumber].steps;
      SteppersArray[motorNumber].position = 0;
      SteppersArray[motorNumber].steps = 0;
      SteppersArray[motorNumber].homeRoutine = 0;
      printf("END interrupt motor %d. Max Steps = %d\n", motorNumber,
        SteppersArray[motorNumber].maxSteps);
    }
  }
}

void PCINT2Handler(){
  _delay_ms(20);
  if (readPin('C', 0) && readPin('C', 1)){
    raceEnd(1, START);
  }
  else if (readPin('C', 0)){
    raceEnd(0, START);
  }
  else if (readPin('C', 1)){
    raceEnd(0, END);
  }
  if (readPin('C', 2) && readPin('C', 3)){
    raceEnd(2, END);
  }
  else if (readPin('C', 2)){
    raceEnd(1, END);
  }
  else if (readPin('C', 3)){
    raceEnd(2, START);
  }

}


void timer1Handler(){
  for (int i = 0; i < NUMBER_STEPPERS; i++){
    if (SteppersArray[i].enable == ENABLE){
      if (SteppersArray[i].steps > 0){
        counter[i]++;
        float pulseDelay = 1000.0 / (SteppersArray[i].revoltionPerMinute * 360.0 / 60.0 /
          SteppersArray[i].degreesPerStep) / baseTime;
        if (counter[i] <= pulseDelay / 2){
          pinOn(SteppersArray[i].stepPort, SteppersArray[i].stepPin);
        }
        else{
          pinOff(SteppersArray[i].stepPort, SteppersArray[i].stepPin);
        }
        if (counter[i] >= pulseDelay){
          counter[i] = 0;
          SteppersArray[i].steps--;
          if (SteppersArray[i].dir == FORWARD){
            SteppersArray[i].position++;
          }
          else{
            SteppersArray[i].position--;
          }
        }
      }
    }
  }
}
