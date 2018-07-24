#include <util/twi.h>
#include <avr/interrupt.h>

#include "i2c.h"

static void (*I2C_recv)(uint8_t);
static void (*I2C_req)();

void I2C_setCallbacks(void (*recv)(uint8_t), void (*req)())
{
  I2C_recv = recv;
  I2C_req = req;
}

void I2C_init(uint8_t address)
{
  cli();
  // Save slave adress
  TWAR = address << 1;
  // TWINT: interruption flag , TWIE: enable interruption TWI,
  // TWEN: enables communication TWI, TWEA: enable bit of Acknowlegde pulse
  TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
  sei();
}

void I2C_stop(void)
{
  cli();
  // clear registers, stop I2C
  TWCR = 0;
  TWAR = 0;
  sei();
}

ISR(TWI_vect)
{
  switch(TW_STATUS)
  {
    case TW_SR_DATA_ACK:
      // data received, ACK returned
      I2C_recv(TWDR);
      // TWDR contains the next datum to be sended or the last received datum
      TWCR = (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    case TW_ST_SLA_ACK:
      // SLA+R received, ACK returned  An address packet consisting of a slave address and a READ or
      // a WRITE bit is called SLA+R or SLA+W, respectivel
      I2C_req();
      TWCR = (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    case TW_ST_DATA_ACK:
      // data transmitted, ACK received
      I2C_req();
      TWCR = (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    case TW_BUS_ERROR:
      // illegal start or stop condition
      TWCR = 0;
      TWCR = (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
    default:
      TWCR = (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
      break;
  }
}
