#ifndef UARTINTERPRETER_H
#define UARTINTERPRETER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "GPIO.h"
#include "uart.h"
#include "interrupts.h"
#include "timers.h"
#include "stepper.h"

void commandInterpreter(char command[], int size);
void help();

#endif
