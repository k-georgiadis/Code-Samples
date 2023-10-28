/*
 * 24c32.h
 *
 * Created: 4/8/2022 21:49:22
 * Author: Kosmas Georgiadis
 */

#ifndef EE24C32_H_
#define EE24C32_H_

#include <avr/io.h>
#include "twi.h"

#define EEDEVADDR	0b10100000
#define EEPAGES		256
#define EEPAGESIZE	32
#define EEMAXADDR	0x1000
#define ERROR		1
#define SUCCESS		(!ERROR)

uint8_t EE_WriteByte(uint16_t addr, uint8_t data);
uint8_t EE_ReadByte(uint16_t addr, uint8_t *data);
uint8_t EE_ReadPage(uint16_t addr, uint8_t *data);
uint8_t EE_WritePage(uint16_t addr, uint8_t *data);

#endif /* EE24C32_H_ */