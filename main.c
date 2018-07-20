#define brate0 57600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "lib/GPIO.h"
#include "lib/uart.h"
#include "lib/uartInterpreter.h"
#include "lib/interrupts.h"
#include "lib/timers.h"
#include "lib/stepper.h"
#include "lib/i2c_interpreter.h"
#include "lib/i2c.h"


FILE uart_io = FDEV_SETUP_STREAM(echoChar, getChar, _FDEV_SETUP_RW);
enum State {INI, ACT, HOME, POS};
enum State state;

void configureGPIO(){
  setPin('B', 5, 1);
  setPin('B', 4, 1);
  setPin('B', 3, 1);
  setPin('B', 2, 1);
  setPin('B', 1, 1);
  setPin('B', 0, 1);
  setPin('D', 7, 1);
  setPin('D', 6, 1);
  setPin('D', 5, 1);
  setPin('D', 4, 1);
  setPin('D', 3, 1);
}

void INTOHandler(){
  _delay_ms(20);
  if (readPin('D', 2)){
      state = ACT;
      printf("ACT\n");
  }
  else {
    reset();
  }
}

void timer0Handler(){
  checkPositioning();
}

int main(){
  state = INI;
  stdout = stdin = &uart_io;
  initializeUART(F_CPU, brate0, commandInterpreter);
  setINT(0, INTOHandler, 1);
  configureGPIO();

  SteppersArray[0] = createStepper('D', 3, 'D', 4, 'D', 5, 1.8, 10);
  SteppersArray[1] = createStepper('D', 6, 'D', 7, 'B', 0, 1.8, 10);
  SteppersArray[2] = createStepper('B', 1, 'B', 2, 'B', 3, 1.8, 10);
  float baseTime = 1.0;
  //timer1FastPWM(500, 450, 10);
  setPCINT(8, PCINT2Handler);
  setPCINT(9, PCINT2Handler);
  setPCINT(10, PCINT2Handler);
  setPCINT(11, PCINT2Handler);
  sei();

  printf("INI\n");
  while (1){
    switch (state){
      case INI:
        pinOn('B', 5);
        pinOff('B', 4);
        break;
      case ACT:
        printf("ACT\n");
        pinOff('B', 5);
        pinOn('B', 4);
        state = HOME;
        break;
      case HOME:
        printf("HOME\n");
        setBaseTime(baseTime);
        enableMotors(SteppersArray, NUMBER_STEPPERS);
        timer1CTC(baseTime, timer1Handler);
        homing(SteppersArray, NUMBER_STEPPERS);

        while (1){
          if (checkHoming(SteppersArray, NUMBER_STEPPERS)){
            state = POS;
            printf("POS\n");
            break;
          }
        }
        I2C_start();
        break;
      case POS:
        timer0Normal(timer0Handler);
        _delay_ms(10000);
        break;
      default:
        break;
    }
  }
}
