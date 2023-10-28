/*
 * Clock.c
 *
 * Created: 1/11/2015 5:00:26 μμ
 * Author : CosmaOne
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

#define SEVEN_SEGMENT_PORT	PORTD
#define SEVEN_SEGMENT_DDR	DDRD

volatile uint8_t digits[4];

uint8_t blink=0;
uint8_t stop_blink=0;

uint8_t Inc_pressed=0;
uint8_t Dec_pressed=0;
uint8_t Enter_pressed=0;

uint16_t tInc=0;
uint16_t tDec=0;
uint16_t tEnter=0;

uint8_t Inc_Flag=0;
uint8_t Dec_Flag=0;
uint8_t Enter_Flag=0;

volatile uint8_t h=0;
volatile uint8_t m=0;
volatile uint8_t s=0;

uint8_t Timer_Started=0;

void init();
void SetTime();

void Inc();
void Dec();
void DoInc();
void DoDec();


void SevenSegment(uint8_t n);
void Print(uint16_t num, uint8_t f);

void main()
{	
	//Initialize.
	init();

    while(1)
	{
		if(!Timer_Started)
		{
			Inc();
			Dec();
		}
		SetTime();
	}
}

void init()
{
    // Prescaler = FCPU/1024
    TCCR0|=(1<<CS02);
    TIMSK|=(1<<TOIE0);
    TCNT0=0;
    
    //Set Ports-----------

    //--Button
    DDRC=0x0f;
    PORTC=0x0f;
    
    //--LED Display
    DDRB=0x0F;
    DDRD=0xFF;
    //---------------------
    
    //Enable Global Interrupts
    sei();
}

void SetTime()
{
	if (PINC & 0x08)
	{
		tEnter = 0;
		
		if(Enter_Flag && !Timer_Started) //If enter button was held and released and clock is not running, start clock.
		{
			Enter_Flag = 0;			
			Enter_pressed = 0;
			Timer_Started = 1;
			
			TCNT1 = 0;
			OCR1A = 15624;
			TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); //Enable Timer 1 with pre-scaler 1024 in CTC mode.
			TIMSK |= (1 << OCIE1A);
		}
		else if(Enter_Flag && Timer_Started) //If enter button was held and released and clock is running, stop clock.
		{
			Enter_Flag = 0;
			Enter_pressed = 0;
			Timer_Started = 0;
			TCCR1B &= ~((1 << CS12) | (1 << CS10)); //Stop Timer 1.
		}
		else if(Enter_pressed && !Timer_Started) //If enter button released and clock is not running.
		{
			//Move to the next two displays.
			if(blink == 2)
				blink = 0;
			else
				blink = 2;
				
			Enter_pressed=0;
		}
	}
	else
	{
		tEnter++;
		Enter_pressed = 1;
		
		if(tEnter>60000)
			Enter_Flag=1;
	}
}

void Inc()
{
	if (PINC & 0x01)
	{
		tInc=0;
		
        if(Inc_Flag)
        {
            Inc_Flag=0;
            stop_blink=0; //Reset blink.
            Inc_pressed=0;
        }            
		if(Inc_pressed)
		{
			Inc_pressed=0;
			stop_blink=0; //Reset blink.
			DoInc(); //Increase value.	
		} 
	}
	else
	{
		tInc++;
        Inc_pressed=1;
        
		if(tInc>60000)
		{
            Inc_Flag=1;
			tInc=20000; //That's the interval between each slide.
			stop_blink=1; //Stop blinking while setting time.
			DoInc(); //Increase value.
       }	
	}
}
void Dec()
{
	if (PINC & 0x02)
	{
    	tDec=0;
        
        if(Dec_Flag)
        {
            Dec_Flag=0;
            stop_blink=0; //Reset blink.
            Dec_pressed=0;
        }     
    	if(Dec_pressed)
    	{
	    	Dec_pressed=0;
	    	stop_blink=0; //Reset blink.
			DoDec(); //Decrease value.
	    }
	}
	else
	{
		tDec++;
        Dec_pressed=1;
        
		if(tDec>60000)
		{
            Dec_Flag=1;
			tDec=20000; //That's the interval between each slide.
			stop_blink=1; //Stop blinking while setting time.
			
			DoDec(); //Decrease value.
		}
	}
}

void DoInc()
{
	if(blink==0)
	{
		if(m<59)
			m++;
		else
			m=0;
		Print(m,0);
	}
	else
	{
		if(h<23)
			h++;
		else
			h=0;
		Print(h,2);
	}
	return;
}

void DoDec()
{
	if(blink==0)
	{
		if(m==0)
			m=59;
		else
			m--;
		Print(m,0);
	}
	else
	{
		if(h==0)
			h=23;
		else
			h--;
		Print(h,2);
	}
	return;
}

void SevenSegment(uint8_t n)
{
	if(n<13)
	{
		switch (n)
		{
			case 0:
			//.gfedcba
			SEVEN_SEGMENT_PORT=0b00111111;
			break;

			case 1:
			SEVEN_SEGMENT_PORT=0b00000110;
			break;

			case 2:
			SEVEN_SEGMENT_PORT=0b01011011;
			break;

			case 3:
			SEVEN_SEGMENT_PORT=0b01001111;
			break;

			case 4:
			SEVEN_SEGMENT_PORT=0b01100110;
			break;

			case 5:
			SEVEN_SEGMENT_PORT=0b01101101;
			break;

			case 6:
			SEVEN_SEGMENT_PORT=0b01111101;
			break;

			case 7:
			SEVEN_SEGMENT_PORT=0b00000111;
			break;

			case 8:
			SEVEN_SEGMENT_PORT=0b01111111;
			break;

			case 9:
			SEVEN_SEGMENT_PORT=0b01101111;
			break;
			
			case 10: //'λ'
			SEVEN_SEGMENT_PORT=0b01010111;
			break;

			case 11: //'Ω'
			SEVEN_SEGMENT_PORT=0b01101011;
			break;
			
			case 12: //dp
			SEVEN_SEGMENT_PORT|=0b10000000;
		}
	}
	else
	{
		//This symbol on display tells that n was greater than 12
		//so display can't handle it
		SEVEN_SEGMENT_PORT=0b11111111;
	}
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t i=0;
    static uint16_t j=0;
  
    if(i==3)
       i=0;
    else
       i++;
    j++;
    
    if(j>=300)
		j=0;
        
    PORTB&=(0b11110000); //Turn off all displays.    
    
	if(j>=200 && !Timer_Started && !stop_blink && (i==blink || i==blink+1)) //Light only the unselected displays.
        return;
        
    PORTB|=(1<<i);
    
    SevenSegment(digits[i]);
	if(i==2)SevenSegment(12);	//Light dot.
}

ISR(TIMER1_COMPA_vect)
{
    PORTC^=0x04;
	s++;
	if(s>=60)
	{
        s=0;
		m++;
		if(m>=60)
		{
            m=0;
			h++;
			if(h>=24)
				h=0;
			Print(h,2);	
		}
		Print(m,0);
	}
}

void Print(uint16_t num, uint8_t f)
{
    uint8_t i=f;
    
    if(num>9999) return;
        
    while(num>=0)
    {
		digits[i]=num%10;
		i++;
		if(i>=f+2)
			break;
		 num=num/10; 
    }
}


