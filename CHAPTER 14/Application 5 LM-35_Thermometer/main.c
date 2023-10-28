/*
 * LM-35_Thermometer.c
 *
 * Created: 7/5/2016 10:53:41 μμ
 * Author : Κοσμάς Γεωργιάδης
 */ 
#define F_CPU 12000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
							
#define ADC_PIN 5 //Set PINC for ADC.

//-------------------------------------------------------------------------

volatile uint8_t MAX=3;
volatile uint8_t digits[4];
volatile uint8_t i=0,db=0,neg=0;

//-------------------------------------------------------------------------	

//Variables for storing ADC readings.
uint16_t adc=0;

//-------------------------------------------------------------------------

void init();
void InitADC();

uint16_t ReadADC(uint8_t ch);
void AverageReadings();
void SevenSegment(uint8_t n);
void Print(uint16_t num);
void PrintDouble(double num);

double Temperature=0;

void main()
{
	init();
	InitADC();
	
	while(1)
	{	
		AverageReadings();

		//float avg = adc/10;
		Temperature = (adc * 500) / 1024.0;
		//Temperature = ((adc)*5)/1024.0;
		PrintDouble(Temperature);
		_delay_ms(1000);	
	}
}

void init()
{
	// Prescaler = FCPU/1024
	TCCR0|=(1<<CS02);
	TIMSK|=(1<<TOIE0);
	TCNT0=0;

	//Set Ports-----------
	
	//--LED Display
	DDRB=0x0F;
	DDRD=0xFF;
	//---------------------

	//Enable Global Interrupts
	sei();
}

void InitADC()
{
	ADMUX|=(1<<REFS0);//refV=Vcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescaler=128
}

uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ch=ch&0b00000111;
	ADMUX=(ADMUX & 0xF8)|ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	ADCSRA|=(1<<ADIF);

	return(ADC);
}

void AverageReadings()
{
	//adc = 0;
	//uint8_t j;

	//for(j=0; j<10; j++)
	//{
		//adc += ReadADC(ADC_PIN);
		//_delay_ms(20);
	//}
	adc = ReadADC(ADC_PIN);
}

ISR(TIMER0_OVF_vect)
{
	if(i>=MAX)
		i=0;
	else
		i++;

	PORTB&=0xF0;
	PORTB|=(1<<i);
	
	if(neg) //If Negative mode is active.
	{
		if(i==MAX) //If we reached MAXth display.
			SevenSegment(10);  //Display Minus(-) symbol in MAXth display.
		else
			SevenSegment(digits[i]);
	}
	else if(i<=MAX)
		SevenSegment(digits[i]);
		
}

void SevenSegment(uint8_t n)
{
	if(n<11)
	{
		switch (n)
		{
			case 0:
			//.gfedcba
			PORTD=0b00111111;
			break;

			case 1:
			//.gfedcba
			PORTD=0b00000110;
			break;

			case 2:
			//.gfedcba
			PORTD=0b01011011;
			break;

			case 3:
			//.gfedcba
			PORTD=0b01001111;
			break;

			case 4:
			//.gfedcba
			PORTD=0b01100110;
			break;

			case 5:
			//.gfedcba
			PORTD=0b01101101;
			break;

			case 6:
			//.gfedcba
			PORTD=0b01111101;
			break;

			case 7:
			//.gfedcba
			PORTD=0b00000111;
			break;

			case 8:
			//.gfedcba
			PORTD=0b01111111;
			break;

			case 9:
			//.gfedcba
			PORTD=0b01101111;
			break;
			
			case 10:
			//.gfedcba
			PORTD=(1<<6);
		}
	}
	else
		PORTD=0xFF;
		
	if(i==1 && db)PORTD|=(1<<7); //If Decimal mode is active display decimal point (.) symbol in second display.
}

void Print(uint16_t num)
{
	uint8_t i=0;
	uint8_t j;
	
	MAX=3;
	if(num>9999) return;
	while(num)
	{
		digits[i]=num%10;
		i++;
		num=num/10;
	}
	for(j=i;j<4;j++) digits[j]=0;
	db=0; //De-activate Decimal Mode.
}

void PrintDouble(double num)
{
	uint8_t i=0;
	uint8_t j;

	if(num<-50 || num>125)
	{
		Print(3660); //Error. Value out of bounds.
		return;
	}
	
	if(num<0)
	{
		if(num>-10)
			MAX=2;
		else
			MAX=3;
			
		neg=1; //Activate Negative Mode.
		num=num*(-1.0); //Make number positive so we can display it.
	}
	else
	{
		if(num>=100) //Use all displays.
			MAX=3;
		else if(num>=10) //Use three displays.
			MAX=2;
		else
			MAX=1; //Use two displays.
		
		neg=0; //De-activate Negative Mode.
	}
	
	num=num*10.0; //Multiply by 10 to display decimal part.
	while(num)
	{
		digits[i]=(int)num%10;
		i++;
		num=(int)num/10;
	}
	for(j=i;j<4;j++) digits[j]=0; //Fill rest digits with 0.
		
	db=1; //Activate Decimal Mode.
}


