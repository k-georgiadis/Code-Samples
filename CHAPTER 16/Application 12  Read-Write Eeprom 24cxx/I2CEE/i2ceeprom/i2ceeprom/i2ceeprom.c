#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "ee24c16.h"
//#include "ee24c32.h"
#include "ee24c256.h"
#include "twi.h"

#define EE_SUCCESS				1
#define EE_READ_ERROR			2
#define EE_WRITE_ERROR			4
#define EE_VERIFY_ERROR			6

static void InitADC();
static uint16_t ReadADC(uint8_t ch);

int main(void)
{
	uint8_t i, temp;
	uint16_t addr = 0x0; //Address in EEPROM to write.
	
	uint8_t eereadpage[EEPAGESIZE]; //Buffer for reading values.
	uint8_t eewritepage[EEPAGESIZE] = {
											10, 44, 255, 46, 80, 87, 43, 130,
											210, 23, 1, 58, 46, 150, 12, 46,
											1, 2, 3, 4, 5, 6, 7, 8,
											11, 12, 13, 14, 15, 16, 17, 18,

											11, 24, 225, 26, 20, 27, 23, 230,
											220, 21, 2, 28, 26, 20, 22, 26,
											21, 22, 23, 24, 25, 26, 27, 28,
											211,212, 213, 214, 215, 216, 217, 218
									   }; //Array with values to write into EEPROM.

	//LED port init.
	DDRB = 7;
	PORTB = 0;

	InitADC();
	TWIInit();

	_delay_ms(1000);

	//Write full page to EEPROM.
	if(EE_WritePage(0, addr, eewritepage) != ERROR)
	{
		_delay_ms(10);

		//Read page bytes and print them.
		if(EE_ReadPage(0, addr, eereadpage) != ERROR)
		{
			//Verify bytes.
			for(i = 0; i < EEPAGESIZE; i++)
			{
				if(eereadpage[i] != eewritepage[i])
					break;
			}

			if(i == EEPAGESIZE)
				PORTB = EE_SUCCESS; //Page write/read was successful.
			else
				PORTB = EE_VERIFY_ERROR; //Verification failed.
		}
		else
			PORTB = EE_READ_ERROR; //Page read failed.
	}
	else
		PORTB = EE_WRITE_ERROR; //Page write failed.

	_delay_ms(1000);

	//Continue testing with ADC noise values.
	while(1)
	{ 
		PORTB = 0;

		temp = (uint8_t)ReadADC(2); //Read noise value from ADC2.

		//Write noise value at EEPROM address 0x10.
		if(EE_WriteByte(0, 0x10, temp) != ERROR)
		{
			_delay_ms(10);

			//Read byte and print it.
			if(EE_ReadByte(0, 0x10, &i) != ERROR)
			{
				//Verify value.
				if(i != temp)
					PORTB = EE_VERIFY_ERROR; //Verification failed.
				else
					PORTB = EE_SUCCESS;
			}
			else
				PORTB = EE_READ_ERROR; //Read failed.
		}
		else
			PORTB = EE_WRITE_ERROR; //Write failed.

		_delay_ms(1000);
	}
}

void InitADC()
{
	ADMUX |= (1 << REFS0); //refV = Vcc
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Prescaler = 128
}
uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7.
	ch &= 7;
	ADMUX = (ADMUX & 0xF8) | ch;

	_delay_us(10);

	//Start Single conversion.
	ADCSRA |= (1 << ADSC);

	//Wait for conversion to complete.
	while(!(ADCSRA & (1 << ADIF)));

	//Clear ADIF by writing one (1) to it.
	ADCSRA |= (1 << ADIF);

	return(ADC);
}