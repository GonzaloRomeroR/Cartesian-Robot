#ifndef UART_H
#define UART_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

extern FILE uart_io;

void initializeUART(long int f_cpu, unsigned int ubrr, void (*interpreter)(char *, int));
int echoChar(char c, FILE *stream);
int getChar(FILE *stream);

#endif
