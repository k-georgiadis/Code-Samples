/*
 * 24c256.c
 *
 * Created: 4/8/2022 21:49:22
 * Author: Kosmas Georgiadis
 */

#include "ee24c256.h"
#include "twi.h"

uint8_t EE_WriteByte(uint8_t dev, uint16_t addr, uint8_t data)
{	
	if(addr >= EEMAXADDR)
		return ERROR;

	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;
	
	//Send device address.
	TWIWrite(EEDEVADDR | ((dev & 0x7) << 1));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;

	//Send the high byte of the data address.
	TWIWrite((addr & 0xEF00) >> 8);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;
		
	//Send the low byte of the data address.
	TWIWrite(addr & 0x00FF);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;

	//Write byte to EEPROM.
	TWIWrite(data);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;

	TWIStop();

	return SUCCESS;
}
uint8_t EE_WritePage(uint8_t dev, uint16_t addr, uint8_t *data)
{
	uint8_t i;

	if(addr >= EEMAXADDR)
		return ERROR;
		
	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;

	//Send device address.
	TWIWrite(EEDEVADDR | ((dev & 0x7) << 1));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;
			
	//Send the high byte of the data address.
	TWIWrite((addr & 0xEF00) >> 8);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;
	
	//Send the low byte of the data address.
	TWIWrite(addr & 0x00FF);
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
uint8_t EE_ReadByte(uint8_t dev, uint16_t addr, uint8_t *data)
{
	if(addr >= EEMAXADDR)
		return ERROR;

	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;
	
	//Begin dummy write sequence.
	TWIWrite(EEDEVADDR | ((dev & 0x7) << 1));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;

	//Send the upper byte of the data address.
	TWIWrite((addr & 0xEF00) >> 8);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;

	//Send the lower byte of the data address.
	TWIWrite(addr & 0x00FF);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;
	
	//Send START again.
	TWIStart();
	if(TWIGetStatus() != TWI_REPEAT_START)
		return ERROR;
		
	//Send device address again. Logic will switch to Master Receiver mode according to the data sheet.
	TWIWrite(EEDEVADDR | ((dev & 0x7) << 1) | 1);
	if(TWIGetStatus() != TWI_SLA_R_ACK)
		return ERROR;

	*data = TWIReadNACK();
	if(TWIGetStatus() != TWI_MASTER_R_DATA_NACK)
		return ERROR;

	TWIStop();

	return SUCCESS;
}
uint8_t EE_ReadPage(uint8_t dev, uint16_t addr, uint8_t *data)
{
	uint8_t i;

	if(addr >= EEMAXADDR)
		return ERROR;
		
	TWIStart();

	if(TWIGetStatus() != TWI_START)
		return ERROR;

	//Begin dummy write sequence.
	TWIWrite(EEDEVADDR | ((dev & 0x7) << 1));
	if(TWIGetStatus() != TWI_SLA_W_ACK)
		return ERROR;

	//Send the upper byte of the data address.
	TWIWrite((addr & 0xEF00) >> 8);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;
	
	//Send the lower byte of the data address.
	TWIWrite(addr & 0x00FF);
	if(TWIGetStatus() != TWI_MASTER_T_DATA_ACK)
		return ERROR;
	
	//Send START again.
	TWIStart();
	if(TWIGetStatus() != TWI_REPEAT_START)
		return ERROR;

	//Send device address again. Logic will switch to Master Receiver mode according to the data sheet.
	TWIWrite(EEDEVADDR | ((dev & 0x7) << 1) | 1);
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