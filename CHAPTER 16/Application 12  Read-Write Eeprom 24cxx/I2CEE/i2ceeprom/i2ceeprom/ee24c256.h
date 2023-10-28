/*
 * 24c256.h
 *
 * Created: 4/8/2022 21:49:22
 * Author: Kosmas Georgiadis
 */

#ifndef EE24C256_H_
#define EE24C256_H_

#include <avr/io.h>

#define EEDEVADDR	0b10100000
#define EEPAGES		512
#define EEPAGESIZE	64
#define EEMAXADDR	0x8000
#define ERROR		1
#define SUCCESS		(!ERROR)

uint8_t EE_WriteByte(uint8_t dev, uint16_t addr, uint8_t data);
uint8_t EE_ReadByte(uint8_t dev, uint16_t addr, uint8_t *data);
uint8_t EE_ReadPage(uint8_t dev, uint16_t addr, uint8_t *data);
uint8_t EE_WritePage(uint8_t dev, uint16_t addr, uint8_t *data);

#endif /* EE24C256_H_ */