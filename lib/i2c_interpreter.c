#include "i2c_interpreter.h"

int newCoordinateX = 0;
int newCoordinateY = 0;
int Diameter = 0;
int category = 0;

uint8_t counterI2C = 0;

char command[10];
char buffer[10];


void positionX(){
	int size;
	if (newCoordinateX < 10){
		size = 1;
	}
	if (newCoordinateX > 10){
		size = 2;
	}
	if (newCoordinateX == 100){
		size = 3;
	}
	char str[size];
	sprintf(str, "%d", newCoordinateX);
	//:Rn:
	char command[4 + size];
	command[0] = ':';
	command[1] = 'R';
	command[2] = '0';
	command[3] = '.';

	for (int i = 4; i < (4 + size); i++){
			command[i] = str[i - 4];
	}

	int commandSize = 4 + size;

	commandInterpreter(command, commandSize);

}

void positionY(){

	int size;
	if (newCoordinateY < 10){
		size = 1;
	}
	if (newCoordinateY > 10){
		size = 2;
	}
	if (newCoordinateY == 100){
		size = 3;
	}
	char str[size];
	sprintf(str, "%d", newCoordinateY);
	//:Rn:
	char command[4 + size];
	command[0] = ':';
	command[1] = 'R';
	command[2] = '1';
	command[3] = '.';

	for (int i = 4; i < (4 + size); i++){
			command[i] = str[i - 4];
	}

	int commandSize = 4 + size;

	commandInterpreter(command, commandSize);
}

void positionsMotor(){
	positionX();
	positionY();
}


void interpreterI2C(){
	switch (buffer[0]){
		case 'X':
		newCoordinateX = atoi(&buffer[1]);
		break;
		case 'Y':
		newCoordinateY = atoi(&buffer[1]);
		break;

		case 'C':
		if (buffer[1]=='t'){
			if (buffer[2] == 'o'){
				category = 1;
			}
			if (buffer[2] == 'u'){
				category = 0;
			}
		}
		break;
		case 'D':
		Diameter = atoi(&buffer[1]);
		positionsMotor();
		break;
		default:
		break;
	}
}


void I2C_received(uint8_t received_data){
	char datum;
  datum = received_data;
	switch(datum)
	{
		case 'X':
		counterI2C = 0;
		buffer[counterI2C++]=datum;
		break;
		case 'Y':
		counterI2C = 0;
		buffer[counterI2C++]=datum;
		break;
		case 'C':
		counterI2C = 0;
		buffer[counterI2C++] = datum;
		break;
		case 'D':
		counterI2C = 0;
		buffer[counterI2C++] = datum;
		break;
		case '-' :
		buffer[counterI2C]=0;
		interpreterI2C();
		break;
		default:
		buffer[counterI2C++] = datum;
		break;
	}
}

void I2C_requested(){
	char datum = 'N';
  I2C_transmitByte(datum);
}

void setupI2C(){
  I2C_setCallbacks(I2C_received, I2C_requested);
  I2C_init(I2C_ADDR);
}

void I2C_start(){
  setupI2C();
}
