#ifndef TIMERS_H
#define TIMERS_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <util/delay.h>
#include "GPIO.h"
#include "uart.h"

void timer1CTC(float T, void (*handler)());
void setInterrupt(int inputCapt, int outputCaptA, int outputCaptB, int overflow);
void configureOutputMode(int outA, int outB);
void setPrescaler(float T);
void setMode(int mode);
void timer1FastPWM(float T, float dutyCicleA, float dutyCicleB);
void disableTimerInterrupts();
void timer0Normal(void (*handler)());

#endif
