#ifndef I2C_INTERPRETER_H
#define I2C_INTERPRETER_H
#define I2C_ADDR 0x8

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <util/delay.h>
#include "GPIO.h"
#include "uart.h"
#include "i2c.h"
#include "uartInterpreter.h"


void interpreterI2C();
void positionsMotor();
void positionX();
void positionY();
void I2C_received(uint8_t received_data);
void I2C_requested();
void setupI2C();
void I2C_start();


#endif
