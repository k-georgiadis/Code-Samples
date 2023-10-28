/*
 * 24c16.c
 *
 * Created: 4/8/2022 11:22:21
 * Author: Kosmas Georgiadis
 */ 
#include "ee24c16.h"

uint8_t EE_WriteByte(uint16_t addr, uint8_t data)
{
	if(addr >= EEMAXADDR)
		return ERROR;

	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;
	
	//Send device address.
	TWIWrite(EEDEVADDR | (uint8_t)((addr & 0x700) >> 7));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;
	
	//Send data address.
	TWIWrite(addr);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;

	//Write byte to EEPROM.
	TWIWrite(data);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;

	TWIStop();

	return SUCCESS;
}
uint8_t EE_WritePage(uint16_t addr, uint8_t *data)
{
	uint8_t i;

	if(addr >= EEMAXADDR)
		return ERROR;
	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;
		
	//Send device address.
	TWIWrite(EEDEVADDR | (uint8_t)((addr & 0x700) >> 7));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;
		
	//Send data address.
	TWIWrite(addr);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;

	//Write page to EEPROM.
	for(i = 0; i < EEPAGESIZE; i++)
	{
		TWIWrite(*data++);
		if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
			return ERROR;
	}

	TWIStop();

	return SUCCESS;
}
uint8_t EE_ReadByte(uint16_t addr, uint8_t *data)
{
	if(addr >= EEMAXADDR)
		return ERROR;

	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;
	
	//Begin dummy write sequence.
	TWIWrite(EEDEVADDR | (uint8_t)((addr & 0x700) >> 7));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;
		
	//Send data address.
	TWIWrite(addr);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;
		
	//Send START again.
	TWIStart();
	if(TWIGetStatus() != TWI_REPEAT_START)
		return ERROR;
		
	//Send device address again. Logic will switch to Master Receiver mode according to the data sheet.
	TWIWrite(EEDEVADDR | (uint8_t)((addr & 0x700) >> 7) | 1);
	if(TWIGetStatus() != TWI_SLA_R_ACK)
		return ERROR;

	*data = TWIReadNACK();
	if(TWIGetStatus() != TWI_MASTER_R_DATA_NACK)
		return ERROR;

	TWIStop();

	return SUCCESS;
}
uint8_t EE_ReadPage(uint16_t addr, uint8_t *data)
{
	uint8_t i;

	if(addr >= EEMAXADDR)
		return ERROR;

	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;
		
	//Begin dummy write sequence.
	TWIWrite(EEDEVADDR | (uint8_t)((addr & 0x700) >> 7));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;
	
	//Send data address.
	TWIWrite(addr);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;

	//Send START again.
	TWIStart();

	if(TWIGetStatus() != TWI_REPEAT_START)
		return ERROR;
		
	//Send device address again. Logic will switch to Master Receiver mode according to the data sheet.
	TWIWrite(EEDEVADDR | 1);
	if(TWIGetStatus() != TWI_SLA_R_ACK)
		return ERROR;

	for(i = 0; i < EEPAGESIZE - 1; i++)
	{
		*data++ = TWIReadACK();

		if(TWIGetStatus() != TWI_MASTER_R_DATA_ACK)
			return ERROR;
	}

	*data = TWIReadNACK();
	if(TWIGetStatus() != TWI_MASTER_R_DATA_NACK)
		return ERROR;
		
	TWIStop();

	return SUCCESS;
}