#include "i2c_interpreter.h"

int POSITIONING = 0;
enum POS_STATE {NO_POS, TO_UP_PLACE, TO_DOWN_PLACE, TO_UP_REFERENCE, TO_DOWN_REFERENCE};
enum POS_STATE posState;
enum POS_STATE antPosState;
int counterPos = 0;

int X_REFERENCE = 5;
int Y_REFERENCE = 5;
int Z_REFERENCE_UP = 90;
int Z_REFERENCE_DOWN = 5;


int newCoordinate[] = {0, 0, 0};
int Diameter = 0;
int category = 0;

uint8_t counterI2C = 0;

char command[10];
char buffer[10];


void position(char selector){
	int motor;
	if (selector == 'X'){
		motor = 0;
	}
	if (selector == 'Y'){
		motor = 1;
	}
	if (selector == 'Z'){
		motor = 2;
	}
	int size;
	if (newCoordinate[motor] < 10){
		size = 1;
	}
	if (newCoordinate[motor] >= 10){
		size = 2;
	}
	if (newCoordinate[motor] == 100){
		size = 3;
	}
	if (newCoordinate[motor] > 100){
		printf("Error, out of range \n");
	}
	char str[size];
	sprintf(str, "%d", newCoordinate[motor]);

	char command[4 + size];
	command[0] = ':';
	command[1] = 'R';

	if (motor == 0){
		command[2] = '0';
	}
	if (motor == 1){
		command[2] = '1';
	}
	if (motor == 2){
		command[2] = '2';
	}
	command[3] = '.';

	for (int i = 4; i < (4 + size); i++){
			command[i] = str[i - 4];
	}
	int commandSize = 4 + size;
	char passedCommand[commandSize];
	for (int i = 0; i < commandSize; i++){
			passedCommand[i] = command[i];
	}
	commandInterpreter(passedCommand, commandSize);
}

void positionsMotor(){
	position('X');
	position('Y');
	position('Z');
}


void calculateSpeed(){
	int differenceArray[NUMBER_STEPPERS];
	for (int i = 0; i < 3; i++){
		differenceArray[i] = abs(newCoordinate[i] - getRelative(&SteppersArray[i]));
	}
	int aux = -1000;
	int maxRpm = 10;
	int maxDistanceMotor;

	for (int i = 0; i < NUMBER_STEPPERS; i++) {
		if(differenceArray[i] > aux){
			aux = differenceArray[i];
			maxDistanceMotor = i;
		}
	}
	for (int i = 0; i < NUMBER_STEPPERS; i++) {
		int speed = (int)((float)differenceArray[i] / aux * maxRpm + 0.5);
		if (speed == 0){
			speed = 1;
		}
		setSpeed(&SteppersArray[i], speed);
	}
}

void checkPositioning(){
	if (posState == TO_UP_PLACE){
		if (counterPos == 0){
			counterPos = 1;
			newCoordinate[2] = Z_REFERENCE_UP;
			calculateSpeed();
			position('X');
			position('Y');
			position('Z');
			printf("Position TO_UP_PLACE\n");
		}

		if(SteppersArray[0].steps < 5 && SteppersArray[1].steps < 5 && SteppersArray[2].steps < 5){
			if (antPosState == NO_POS){
				antPosState = TO_UP_PLACE;
				posState = TO_DOWN_PLACE;
				counterPos = 0;
			}
			if (antPosState == TO_DOWN_PLACE){
				antPosState = TO_UP_PLACE;
				posState = TO_UP_REFERENCE;
				counterPos = 0;
			}
		}
	}
	if (posState == TO_DOWN_PLACE){
		if (counterPos == 0) {
			counterPos = 1;
			position('X');
			position('Y');
			newCoordinate[2] = Z_REFERENCE_DOWN;
			calculateSpeed();
			position('Z');
			printf("Position TO_DOWN_PLACE\n");
		}

		if(SteppersArray[0].steps < 5 && SteppersArray[1].steps < 5 && SteppersArray[2].steps < 5){
				antPosState = TO_DOWN_PLACE;
				posState = TO_UP_PLACE;
				counterPos = 0;
		}
	}

	if (posState == TO_UP_REFERENCE){
		if (counterPos == 0){
			counterPos = 1;
			newCoordinate[0] = X_REFERENCE;
			newCoordinate[1] = Y_REFERENCE;
			newCoordinate[2] = Z_REFERENCE_UP;
			calculateSpeed();
			position('X');
			position('Y');
			position('Z');
			printf("Position TO_UP_REFERENCE\n");
		}

		if(SteppersArray[0].steps < 5 && SteppersArray[1].steps < 5 && SteppersArray[2].steps < 5){
			if (antPosState == TO_UP_PLACE){
				antPosState = TO_UP_REFERENCE;
				posState = TO_DOWN_REFERENCE;
				counterPos = 0;
			}
			if (antPosState == TO_DOWN_REFERENCE){
				antPosState = TO_UP_REFERENCE;
				posState = NO_POS;
				POSITIONING = 0;
				counterPos = 0;
			}
		}
	}

	if (posState == TO_DOWN_REFERENCE){
		if (counterPos == 0) {
			counterPos = 1;
			newCoordinate[0] = X_REFERENCE;
			newCoordinate[1] = Y_REFERENCE;
			newCoordinate[2] = Z_REFERENCE_DOWN;
			calculateSpeed();
			position('X');
			position('Y');
			position('Z');
			printf("Position TO_DOWN_REFERENCE\n");
		}

		if(SteppersArray[0].steps < 5 && SteppersArray[1].steps < 5 && SteppersArray[2].steps < 5){
				antPosState = TO_DOWN_REFERENCE;
				posState = TO_UP_REFERENCE;
				counterPos = 0;
		}
	}
}

void interpreterI2C(){
	if (POSITIONING == 0){
		switch (buffer[0]){
			case 'X':
				newCoordinate[0] = atoi(&buffer[1]);
				break;
			case 'Y':
				newCoordinate[1] = atoi(&buffer[1]);
				break;
			case 'Z':
				newCoordinate[2] = atoi(&buffer[1]);
			break;
			case 'C':
				if (buffer[1] == 't'){
					if(buffer[2] == 'u' ){
						X_REFERENCE = 90;
						Y_REFERENCE = 90;
						Z_REFERENCE_UP = 90;
						Z_REFERENCE_DOWN = 5;

					}
					if (buffer[2] == 'o'){
						X_REFERENCE = 5;
						Y_REFERENCE = 5;
						Z_REFERENCE_UP = 90;
						Z_REFERENCE_DOWN = 5;
					}

				}

			break;
			case 'D':
				Diameter = atoi(&buffer[1]);
			break;
			case 'G':
				positionsMotor();
			break;
			case 'P':
				POSITIONING = 1;
				antPosState = NO_POS;
				posState = TO_UP_PLACE;
			break;
			case 'S':
				calculateSpeed();
			break;
			default:
			break;
		}
	}
}


void I2C_received(uint8_t received_data){
	char datum;
  datum = received_data;
	switch(datum)
	{
		case 'X':
		case 'Y':
		case 'Z':
		case 'C':
		case 'D':
		case 'G':
		case 'S':
		case 'P':
		counterI2C = 0;
		buffer[counterI2C++]=datum;
		break;
		case '-' :
		buffer[counterI2C]=0;
		interpreterI2C();
		memset(buffer, 0, 10);
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
