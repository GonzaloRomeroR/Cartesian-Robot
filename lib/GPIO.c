#include "GPIO.h"

int pinD[] = {
  PORTD0, PORTD1, PORTD2, PORTD3 ,
  PORTD4, PORTD5, PORTD6, PORTD7
};

int pinB[] = {
  PORTB0, PORTB1, PORTB2,
  PORTB3, PORTB4, PORTB5
};

int pinC[] = {
  PORTC0, PORTC1, PORTC2,
  PORTC3, PORTC4, PORTC5
};

int DDpinD[] = {
  DDD0, DDD1, DDD2, DDD3 ,DDD4,
  DDD5, DDD5, DDD6, DDD7
};

int DDpinB[] = {
  DDB0, DDB1, DDB2,
  DDB3, DDB4, DDB5
};

void setPin(char PORT, int number, int state){
  if (state){
    if (PORT == 'D'){
      DDRD |= (1 << DDpinD[number]);
    }
    if (PORT == 'B'){
      DDRB |= (1 << DDpinB[number]);
    }
  }
  else{
    if (PORT == 'D'){
      DDRD &=~ (1 << DDpinD[number]);
    }
    if (PORT == 'B'){
      DDRB &=~ (1 << DDpinB[number]);
    }
  }
}

void pinOn(char PORT, int number){
  if (PORT == 'D'){
    PORTD |= (1 << pinD[number]);
  }
  if (PORT == 'B'){
    PORTB |= (1 << pinB[number]);
  }
}

void pinOff(char PORT, int number){
  if (PORT == 'D'){
    PORTD &=~ (1 << pinD[number]);
  }
  if (PORT == 'B'){
    PORTB &=~ (1 << pinB[number]);
  }
}

void pinToggle(char PORT, int number){
  if (PORT == 'D'){
    PORTD ^= (1 << pinD[number]);
  }
  if (PORT == 'B'){
    PORTB ^= (1 << pinB[number]);
  }
}

int readPin(char PORT, int number){
  if (PORT == 'D'){
    return (PIND & (1 << pinD[number]));
  }
  if (PORT == 'B'){
    return (PINB & (1 << pinB[number]));
  }
}
