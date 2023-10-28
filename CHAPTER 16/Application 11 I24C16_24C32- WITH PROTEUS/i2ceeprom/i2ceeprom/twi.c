/*
 * twi.c
 *
 * Created: 4/8/2022 21:49:22
 * Author: Kosmas Georgiadis
*/
#include "twi.h"

void TWIInit(void)
{
	//Set SCL to 400kHz (F_CPU = 8MHz).
	TWSR = 0x0;
	TWBR = 0x02;

	//Enable TWI.
	TWCR = (1 << TWEN);
}

void TWIStart(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}
void TWIStop(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
void TWIWrite(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}

uint8_t TWIReadACK(void)
{
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}
uint8_t TWIReadNACK(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}
uint8_t TWIGetStatus(void)
{
	uint8_t status;

	status = TWSR & 0xF8;
	return status;
}