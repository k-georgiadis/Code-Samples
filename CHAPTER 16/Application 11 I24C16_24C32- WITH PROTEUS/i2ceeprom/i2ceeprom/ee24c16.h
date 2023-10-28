/*
 * 24c16.h
 *
 * Created: 4/8/2022 21:51:12
 * Author: Kosmas Georgiadis
 */

#ifndef EE24C16_H_
#define EE24C16_H_

#include <avr/io.h>
#include "twi.h"

#define EEDEVADDR	0b10100000
#define EEPAGES		128
#define EEPAGESIZE	16
#define EEMAXADDR	0x800
#define ERROR		1
#define SUCCESS		(!ERROR)

uint8_t EE16_WriteByte(uint16_t addr, uint8_t data);
uint8_t EE16_ReadByte(uint16_t addr, uint8_t *data);
uint8_t EE16_ReadPage(uint16_t addr, uint8_t *data);
uint8_t EE16_WritePage(uint16_t addr, uint8_t *data);

#endif /* EE24C16_H_ */