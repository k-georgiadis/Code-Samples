/*
 * twi.h
 *
 * Created: 4/8/2022 21:49:22
 * Author: Kosmas Georgiadis
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

#define	TWI_START				0x08
#define	TWI_REPEAT_START		0x10
#define	TWI_SLA_W_ACK			0x18
#define	TWI_SLA_W_NACK			0x20
#define	TWI_MASTER_T_DATA_ACK	0x28
#define	TWI_MASTER_T_DATA_NACK	0x30
#define	TWI_SLA_R_ACK			0x40
#define	TWI_SLA_R_NACK			0x48
#define	TWI_MASTER_R_DATA_ACK	0x50
#define	TWI_MASTER_R_DATA_NACK	0x58

void TWIInit(void);
void TWIStart(void);
void TWIStop(void);
void TWIWrite(uint8_t u8data);
uint8_t TWIReadACK(void);
uint8_t TWIReadNACK(void);
uint8_t TWIGetStatus(void);

#endif /* TWI_H_ */