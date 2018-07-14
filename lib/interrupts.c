// EVENT: Low level: 0, Any flank: 1, Falling flank: 2, Raising flank: 3

#include "interrupts.h"


int PCINTs[] = {
  PCINT0, PCINT1, PCINT2, PCINT3, PCINT4, PCINT5,
  PCINT6, PCINT7, PCINT8, PCINT9, PCINT10, PCINT11,
  PCINT12, PCINT13, PCINT14, PCINT14, PCINT16, PCINT17,
  PCINT18, PCINT19, PCINT20, PCINT21, PCINT22, PCINT23
};

void (*handler_0)();
void (*handler_1)();

void (*handler_PCINT0)();
void (*handler_PCINT1)();
void (*handler_PCINT2)();

void setPCINT(int pin, void (*handler_PC)()){
  if (pin < 8){
    PCICR |= (1 << PCIE0);
    handler_PCINT0 = handler_PC;
    PCMSK0 |= (1 << PCINTs[pin]);
  }
  if (pin < 15 && pin >= 8){
    PCICR |= (1 << PCIE1);
    handler_PCINT1 = handler_PC;
    PCMSK1 |= (1 << PCINTs[pin]);
  }
  if (pin > 15){
    PCICR |= (1 << PCIE2);
    handler_PCINT2 = handler_PC;
    PCMSK2 |= (1 << PCINTs[pin]);
  }
}



void setINT(int interruption, void (*handler)(), int event){

  if (interruption == 0){
    setEvent(0, event);
    EIMSK |= (1 << INT0);
    handler_0 = handler;
  }
  if (interruption == 1){
    setEvent(1, event);
    EIMSK |= (1 << INT1);
    handler_1 = handler;
  }
}

ISR(INT0_vect) {
  handler_0();
 }
ISR(INT1_vect){
  handler_1();
}

ISR(PCINT0_vect){
  handler_PCINT0();
}
ISR(PCINT1_vect){
  handler_PCINT1();
}
ISR(PCINT2_vect){
  handler_PCINT2();
}

void setEvent(int interrupt, int event){
  if (interrupt == 0){
    switch (event){
      case 0:
      break;
      case 1:
      EICRA = (1 << ISC00);
      break;
      case 2:
      EICRA = (1 << ISC01);
      break;
      case 3:
      EICRA = (1 << ISC00) | (1 << ISC01);
      break;
    }
  }
  if (interrupt == 1){
    switch (event){
      case 0:
      break;
      case 1:
      EICRA = (1 << ISC10);
      break;
      case 2:
      EICRA = (1 << ISC11);
      break;
      case 3:
      EICRA = (1 << ISC10) | (1 << ISC11);
      break;
    }
  }
}
