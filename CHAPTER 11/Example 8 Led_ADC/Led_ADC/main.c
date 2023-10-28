/*
 * Led_ADC.c
 *
 * Created: 12/4/2020 19:42:16
 * Author : Γεωργιάδης Γεώργιος
 */ 
 #define F_CPU 16000000UL //Define CPU clock frequency here.

#include <avr/io.h>
#include <util/delay.h>

//Set port for LED's.
#define LED_DDR		DDRD	
#define LED_PORT	PORTD

//Set ADC pin for input.
#define ADC_PIN 5

//Declare prototypes.
void init();
void InitADC();
uint16_t ReadADC(uint8_t ch);

int main(void)
{
	init(); //Init LED port.
	InitADC(); //Init ADC.

	uint16_t val; //Store ADC value here.

    while(1) 
    {
		val = ReadADC(ADC_PIN); //Read ADC value.

		//Since the ADC accuracy is 10-bit, there are 2 ^ 10 = 1024 possible values: [0, 1023] --> [0, 5] Volts

		if(val > 512) //2,5 Volts.
			LED_PORT = 0x01; //LED ON.
		else
			LED_PORT = 0x00; //LED OFF.
    }
}

void init()
{
	LED_DDR |= 0xFF; //Set LED port as output.
	LED_PORT = 0x00; //Set default state low (0).
}

void InitADC()
{
	ADMUX|=(1 << REFS0); //Set REFV = VCC.
	ADCSRA=(1 << ADEN)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0); //Enable ADC and set Prescaler = 128.
}

uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7.
	ch = ch&0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;

	_delay_us(10);

	//Start Single conversion.
	ADCSRA |= (1 << ADSC);

	//Wait for conversion to complete.
	while(!(ADCSRA & (1 << ADIF)));

	//Clear ADIF by writing one (1) to it.
	ADCSRA |= (1 << ADIF);

	return(ADC);
}