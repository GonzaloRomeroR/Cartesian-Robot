#ifndef INTERRUPS_H
#define INTERRUPS_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>



void setPCINT(int pin, void (*handler_PC)());
void setINT(int interruption, void (*handler)(), int event);
void setEvent(int interrupt, int event);

#endif
