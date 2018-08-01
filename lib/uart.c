#include "uart.h"

uint8_t counter;
char command[30];

void (*command_interpreter)(char *, int);

void initializeUART(long int f_cpu, unsigned int ubrr, void (*interpreter)(char *, int)){
	// UBRR0 required transmition speed
	UBRR0 = f_cpu / 16 / ubrr - 1;
	// TXEN0 enables pin Tx. RXEN0 enables Rx
	// RXCIE0 enables interruption complete reception
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	// USBS0 stop bits (0: 1 bit, 1: 2 bits)
	// UCSZ00 character size in bits
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
  command_interpreter = interpreter;
}


int echoChar(char c, FILE *stream){
	// UDRE0 empty data register flag
	while(!(UCSR0A & (1 << UDRE0)) );
	// UDR0 input and output data register
	UDR0 = c;
	return 0;
}


int getChar(FILE *stream){
	// RXC0 complete reception flag
	while ( !(UCSR0A & (1 << RXC0)) );
	return UDR0;
}


ISR(USART_RX_vect){
	char data;
	data = getChar(&uart_io);
	switch(data)
	{
		case ':':
  		counter=0;
			command[counter++] = data;
  		break;
		case '\r':
  		command[counter] = 0;
  		command_interpreter(command, counter);
  		break;
    case '\n':
      command[counter] = 0;
      command_interpreter(command, counter);
      break;
		default:
  		command[counter++] = data;
  		break;
	}
}
