#include "timers.h"


int prescaler = 0;

void (*handler_Timer1)();
void (*handler_Timer0)();


void timer0Normal(void (*handler)()){
  TCCR0A &=~ (3 << WGM10);
  TCCR0B &=~ (3 << WGM12);
  TIMSK0 |= (1 << TOIE1);
  TCCR0B |= (3 << CS10);
  handler_Timer0 = handler;
}

void timer1CTC(float T, void (*handler)()){
  T = T / 2;
  setMode(4);
  setPrescaler(T);
  setInterrupt(0, 1, 0, 0);
  OCR1A = (int)((T / 1000.0) * (F_CPU / prescaler) + 1);
  handler_Timer1 = handler;
}

void timer1FastPWM(float T, float dutyCicleA, float dutyCicleB){
  setMode(14);
  setPrescaler(T);
  ICR1 = (int)((T / 1000.0) * (F_CPU / prescaler) + 1);
  OCR1A = (int)((dutyCicleA / 1000.0) * (F_CPU / prescaler) + 1);
  OCR1B = (int)((dutyCicleB / 1000.0) * (F_CPU / prescaler) + 1);
  configureOutputMode(3, 3);
  printf("ICR1 = %d\n", ICR1 );
  printf("OCR1A = %d\n", OCR1A );
  printf("OCR1B = %d\n", OCR1B );
}

void disableTimerInterrupts(){
    TIMSK1 &=~ (1 << ICIE1);
    TIMSK1 &=~ (1 << OCIE1A);
    TIMSK1 &=~ (1 << OCIE1B);
    TIMSK1 &=~ (1 << TOIE1);
}

void setInterrupt(int inputCapt, int outputCaptA, int outputCaptB, int overflow){
  if (inputCapt){
    TIMSK1 |= (1 << ICIE1);
  }
  if (outputCaptA){
    TIMSK1 |= (1 << OCIE1A);
  }
  if (outputCaptB){
    TIMSK1 |= (1 << OCIE1B);
  }
  if (overflow){
    TIMSK1 |= (1 << TOIE1);
  }
}

void configureOutputMode(int outA, int outB){
  TCCR1A &=~ (3 << COM1A0);
  TCCR1A &=~ (3 << COM1B0);
  switch (outA) {
    case 0:
      break;
    case 1:
      TCCR1A |= (1 << COM1A0);
      break;
    case 2:
      TCCR1A |= (1 << COM1A1);
      break;
    case 3:
      TCCR1A |= (3 << COM1A0);
      break;
  }
  switch (outB) {
    case 0:
      break;
    case 1:
      TCCR1A |= (1 << COM1B0);
      break;
    case 2:
      TCCR1A |= (1 << COM1B1);
      break;
    case 3:
      TCCR1A |= (3 << COM1B0);
      break;
  }
}

void setPrescaler(float T){
  T = (float)(T / 1000.0);
  float aux = (float)((pow(2, 16) - 1 )/ F_CPU);
  if (T <= aux){
    prescaler = 1;
  }
  else if (T <= 8 * aux){
    prescaler = 8;
  }
  else if (T <= 64 * aux){
    prescaler = 64;
  }

  else if (T <= 256 * aux){
    prescaler = 256;
  }
  else if (T <= 1024 * aux){
    prescaler = 1024;
  }

  TCCR1B &=~ (7 << CS10);
  switch (prescaler) {
    case 1:
      TCCR1B |= (1 << CS10);
      break;
    case 8:
      TCCR1B |= (2 << CS10);
      break;
    case 64:
      TCCR1B |= (3 << CS10);
      break;
    case 256:
      TCCR1B |= (4 << CS10);
      break;
    case 1024:
      TCCR1B |= (5 << CS10);
      break;
  }
}


void setMode(int mode){
  TCCR1A &=~ (3 << WGM10);
  TCCR1B &=~ (3 << WGM12);
  switch (mode) {
    case 0:
      break;
    case 1:
      TCCR1A |= (1 << WGM10);
      break;
    case 2:
      TCCR1A |= (1 << WGM11);
      break;
    case 3:
      TCCR1A |= (3 << WGM10);
      break;
    case 4:
      TCCR1B |= (1 << WGM12);
      break;
    case 5:
      TCCR1A |= (1 << WGM10);
      TCCR1B |= (1 << WGM12);
      break;
    case 6:
      TCCR1A |= (1 << WGM11);
      TCCR1B |= (1 << WGM12);
      break;
    case 7:
      TCCR1A |= (3 << WGM10);
      TCCR1B |= (1 << WGM12);
      break;
    case 8:
      TCCR1B |= (1 << WGM13);
      break;
    case 9:
      TCCR1A |= (1 << WGM10);
      TCCR1B |= (1 << WGM13);
      break;
    case 10:
      TCCR1A |= (1 << WGM11);
      TCCR1B |= (1 << WGM13);
      break;
    case 11:
      TCCR1A |= (3 << WGM10);
      TCCR1B |= (1 << WGM13);
      break;
    case 12:
      TCCR1B |= (1 << WGM12);
      break;
    case 13:
      TCCR1A |= (1 << WGM10);
      TCCR1B |= (3 << WGM12);
      break;
    case 14:
      TCCR1A |= (1 << WGM11);
      TCCR1B |= (3 << WGM12);
      break;
    case 15:
      TCCR1A |= (3 << WGM10);
      TCCR1B |= (3 << WGM12);
      break;
  }
}
ISR(TIMER0_OVF_vect){
  handler_Timer0();
}

ISR(TIMER1_COMPA_vect){
  handler_Timer1();
}
ISR(TIMER1_OVF_vect){
  handler_Timer1();
}
ISR(TIMER1_CAPT_vect){
  handler_Timer1();
}
ISR(TIMER1_COMPB_vect){
  handler_Timer1();
}
