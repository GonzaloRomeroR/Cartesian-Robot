#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>

void setPin(char PORT, int number, int state);
void pinOn(char PORT, int number);
void pinOff(char PORT, int number);
void pinToggle(char PORT, int number);
int readPin(char PORT, int number);

#endif
