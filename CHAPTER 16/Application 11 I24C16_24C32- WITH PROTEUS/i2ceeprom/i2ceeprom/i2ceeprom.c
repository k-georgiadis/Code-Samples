 #define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>

//#include "ee24c16.h"
#include "ee24c32.h"

#define DISPLAY_DDR			DDRB
#define DISPLAY_PORT		PORTB

#define SEVEN_SEGMENT_DDR	DDRD
#define SEVEN_SEGMENT_PORT	PORTD

#define MAX_DISPLAYS		4 //Number of 7SEG digits we want to use.

//Variables for 7SEG display.
volatile uint8_t MAX =	MAX_DISPLAYS;
volatile uint8_t digits[MAX_DISPLAYS];
volatile uint8_t i = 0;

static void init();
static void InitADC();
static uint16_t ReadADC(uint8_t ch);

static void SevenSegment(uint8_t n);
static void Print(int16_t num);
static void PrintString(const char *string);

int main(void)
{

	uint8_t i, temp;
	uint16_t addr = 0x100;
	uint8_t eereadpage[EEPAGESIZE];
	uint8_t eewritepage[EEPAGESIZE] = { 10, 44, 255, 46, 80, 87, 43, 130,
										210, 23, 1, 58, 46, 150, 12, 46,
										1, 2, 3, 4, 5, 6, 7, 8,
										11, 12, 13, 14, 15, 16, 17, 18
									  };
	init();
	InitADC();
	TWIInit();
	
	PrintString("PAGE");
	_delay_ms(1000);

	//Write 32 bytes to EEPROM.
	if(EE32_WritePage(addr, eewritepage) != ERROR)
	{
		_delay_ms(10);

		//Read page bytes and print them.
		if(EE32_ReadPage(addr, eereadpage) != ERROR)
		{
			//Verify bytes.
			for(i = 0; i < EEPAGESIZE; i++)
			{
				if (eereadpage[i] != eewritepage[i])
						break;
					else
					{
						Print(eereadpage[i]);
						_delay_ms(250);
					}
			}

			if(i == EEPAGESIZE)
				PrintString("GOOD"); //Print OK if page write/read was successful.
			else
				PrintString("PERROR"); //Print error if verification fails.

		} 
		else
			PrintString("RERROR"); //Print error if page read fails.

	}
	else
		PrintString("TERROR"); //Print error if page write fails.

	_delay_ms(1000);

	//Continue testing with ADC noise values.
	while(1)
	{
		temp = ReadADC(2);

		//Write noise value at EEPROM address 0x10.
		if(EE32_WriteByte(0x10, temp) != ERROR)
		{
			_delay_ms(10);

			//Read byte and print it.
			if(EE32_ReadByte(0x10, &temp) != ERROR)
				Print(temp);
			else
				PrintString("RERROR"); //Print error if read fails.

		}
		else
			PrintString("TERROR"); //Print error if write fails.

		_delay_ms(250);
	}
}

void init()
{
	//Timer0.
	TIMSK = (1 << TOIE0);
	TCNT0 = 0;
	TCCR0 |= (1 << CS01) | (1 << CS00); //Set prescaler to 64 and start the timer.
		
	//LED Display.
	DISPLAY_DDR |= 0x0F;
	SEVEN_SEGMENT_DDR = 0xFF;

	sei();

	return;
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

ISR(TIMER0_OVF_vect)
{
	DISPLAY_PORT |= ~(0xFF << MAX_DISPLAYS); //Turn off all displays.
	
	//Go back to first display if beyond last.
	if(i >= MAX)
		i = 0;

	//Display digit.
	SevenSegment(digits[i]);

	//Turn on displays.
	DISPLAY_PORT &= ~(1 << i);

	i++;
}
void SevenSegment(uint8_t n)
{
	if(n < 128)
	{
		switch (n)
		{
			//Numbers.
			//-------------------------------------
			case 0:
				//.gfedcba
				SEVEN_SEGMENT_PORT = 0b00111111;
				break;
			case 1:
				SEVEN_SEGMENT_PORT = 0b00000110;
				break;
			case 2:
				SEVEN_SEGMENT_PORT = 0b01011011;
				break;
			case 3:
				SEVEN_SEGMENT_PORT = 0b01001111;
				break;
			case 4:
				SEVEN_SEGMENT_PORT = 0b01100110;
				break;
			case 5:
				SEVEN_SEGMENT_PORT = 0b01101101;
				break;
			case 6:
				SEVEN_SEGMENT_PORT = 0b01111101;
				break;
			case 7:
				SEVEN_SEGMENT_PORT = 0b00000111;
				break;
			case 8:
				SEVEN_SEGMENT_PORT = 0b01111111;
				break;
			case 9:
				SEVEN_SEGMENT_PORT = 0b01101111;
				break;
			//-------------------------------------
			//Letters.
			//-------------------------------------
			case 'A':
				SEVEN_SEGMENT_PORT = 0b01110111; //"A"
				break;
			case 'B':
				SEVEN_SEGMENT_PORT = 0b01111100; //"b"
				break;
			case 'C':
				SEVEN_SEGMENT_PORT = 0b00111001; //"C"
				break;
			case 'D':
				SEVEN_SEGMENT_PORT = 0b01011110; //"d"
				break;
			case 'E':
				SEVEN_SEGMENT_PORT = 0b01111001; //"E"
				break;
			case 'F':
				SEVEN_SEGMENT_PORT = 0b01110001; //"F"
				break;
			case 'G':
				SEVEN_SEGMENT_PORT = 0b01101111; //"g"
				break;
			case 'H':
				SEVEN_SEGMENT_PORT = 0b01110110; //"H"
				break;			
			case 'h':
				SEVEN_SEGMENT_PORT = 0b01110100; //"h"
				break;
			case 'I':
				SEVEN_SEGMENT_PORT = 0b00110000; //"i"
				break;
			case 'L':
				SEVEN_SEGMENT_PORT = 0b00111000; //"L"
				break;
			case 'M':
				SEVEN_SEGMENT_PORT = 0b00010101; //"M"
				break;
			case 'N':
				SEVEN_SEGMENT_PORT = 0b01010100; //"n"
				break;
			case 'O':
				SEVEN_SEGMENT_PORT = 0b00111111; //"O"
				break;
			case 'P':
				SEVEN_SEGMENT_PORT = 0b01110011; //"P"
				break;
			case 'R':
				SEVEN_SEGMENT_PORT = 0b01010000; //"r"
				break;
			case 'S':
				SEVEN_SEGMENT_PORT = 0b01101101; //"S"
				break;
			case 'T':
				SEVEN_SEGMENT_PORT = 0b01111000; //"t"
				break;
			case 'U':
				SEVEN_SEGMENT_PORT = 0b00111110; //"U"
				break;
			case 'Y':
				SEVEN_SEGMENT_PORT = 0b01101110; //"Y"
				break;			
			case 'Z':
				SEVEN_SEGMENT_PORT = 0b01011011; //"Z"
				break;
			case '-':
				SEVEN_SEGMENT_PORT = 0b01000000; //"-"
				break;
		}
	}
	else
		SEVEN_SEGMENT_PORT = 0b01111111; //Display "888" in case of symbol error.

	return;
}

void Print(int16_t num)
{
	uint8_t i = 0, j;

	if(!num) //If num is zero, add zero value to digit array.
		digits[0] = 0;

	while(num)
	{
		digits[i++] = num % 10;
		num = num / 10;
	}

	MAX = i;

	//Check max value.
	if(MAX >= MAX_DISPLAYS)
		MAX = MAX_DISPLAYS;

	//Fill rest digits with zero (0).
	for(j = i; j < MAX_DISPLAYS; j++)
		digits[j] = 0;

	return;
}
void PrintString(const char *string)
{
	uint8_t i, tempMAX = strlen(string);

	if(tempMAX > MAX_DISPLAYS)
		MAX = MAX_DISPLAYS; //Set maximum available digits.
	else
		MAX = tempMAX;

	//Store string characters.
	for(i = 0; i < MAX; i++)
		digits[i] = string[MAX - i - 1];

	return;
}