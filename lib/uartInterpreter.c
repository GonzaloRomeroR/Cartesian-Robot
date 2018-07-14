#include "uartInterpreter.h"


void help(){
  printf("\nUsage:\n");
  printf(":Fm.nnn -> :Forward[motor].steps\n");
  printf(":Bm.nnn -> :Backward[motor].steps\n");
  printf(":Rm.nnn -> :Relative[motor].percentage\n");
  printf(":Vm.nnn -> :Velocity[motor].revolutionsPerMinute\n");
  printf(":Am.nnn -> :AbsolutePosition[motor].position\n");
  printf(":S -> :Stop\n");
  printf(":G -> :GetRelativePositionMotors\n");
  printf(":g -> :getAbsolutePositionMotors\n");
  printf(":D -> :DisableMotors\n");
  printf(":E -> :EnableMotors\n");
  printf(":H -> :HomingRoutine\n");
  printf(":r -> :reset\n");
  printf(":h -> :help\n\n");
}


void commandInterpreter(char command[], int size){
  int number;
  int motor;
  if (size > 2){
    number = atoi(&command[4]);
    motor = command[2] - '0';
  }
  switch (command[1]){
    case 'F':
    if (size < 5){
      printf("Not enough arguments, use help command (:h). \n");
    }
    else{
      printf("Moving forward %d steps motor %d \n", number, motor);
      rotateSteps(&SteppersArray[motor], number, FORWARD);
    }
    break;
    case 'B':
    if (size < 5){
      printf("Not enough arguments, use help command (:h). \n");
    }
    else{
      printf("Moving backward %d steps motor %d \n", number, motor);
      rotateSteps(&SteppersArray[motor], number, BACKWARD);
    }
    break;
    case 'V':
    if (size < 5){
      printf("Not enough arguments, use help command (:h). \n");
    }
    else{
      printf("Motor %d speed changed to %d rpm \n", motor, number);
      setSpeed(&SteppersArray[motor], number);
    }
    break;
    case 'R':
    if (size < 5){
      printf("Not enough arguments, use help command (:h). \n");
    }
    else{
      printf("Moving motor %d to relative position %d\n", motor, number);
      relativePosition(&SteppersArray[motor], number);
    }
    break;
    case 'A':
    if (size < 5){
      printf("Not enough arguments, use help command (:h). \n");
    }
    else{
      printf("Motor %d absolute position changed to %d \n", motor, number);
      absolutePosition(&SteppersArray[motor], number);
    }
    break;
    case 'G':
    printf("Relative position of motors:\n");
    for (int i = 0; i < NUMBER_STEPPERS; i++){
      printf("Motor %d: %d%%\n", i, getRelative(&SteppersArray[i]));
    }
    break;
    case 'g':
    printf("Absolute position of motors:\n");
    for (int i = 0; i < NUMBER_STEPPERS; i++){
      printf("Motor %d: %d\n", i, SteppersArray[i].position);
    }
    break;
    case 'S':
    printf("Stopping motors\n");
    stopMotors(SteppersArray, NUMBER_STEPPERS);
    break;
    case 'D':
    printf("Motors disabled\n");
    disableMotors(SteppersArray, NUMBER_STEPPERS);
    break;
    case 'E':
    printf("Motors enabled\n");
    enableMotors(SteppersArray, NUMBER_STEPPERS);
    break;
    case 'H':
    printf("Starting homing routine\n");
    homing(SteppersArray, NUMBER_STEPPERS);
    break;
    case 'r':
    printf("Reseting\n");
    break;
    case 'h':
    help();
    break;
    default:
    printf("Command not found, use help command (:h). \n");
    break;
  }
}
