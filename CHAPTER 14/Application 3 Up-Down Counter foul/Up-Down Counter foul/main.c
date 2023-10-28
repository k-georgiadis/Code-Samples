
/*
 * EEPROM_Counter_below_20000_.c
 * Author: Κοσμάς Γεωργιάδης
 */ 
//****************************WARNING!!!*****************************
//     THIS PROGRAM WILL WORK IF ALL PORT C PINS ARE LOW(0).
//****************************WARNING!!!*****************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include <avr/eeprom.h>

#define SEVEN_SEGMENT_PORT	PORTD
#define SEVEN_SEGMENT_DDR	DDRD

volatile uint8_t digits[4];
void init();
void Load();
void PReset(uint16_t *c);

void Inc(uint16_t *a,uint16_t *b,uint16_t *count);
void Dec(uint16_t *a,uint16_t *b,uint16_t *count);
uint16_t DoInc(uint16_t c);
uint16_t DoDec(uint16_t c);
uint16_t CheckIfMax(uint16_t c);
uint16_t CheckIfMin(uint16_t c);
void CheckLED();
void SevenSegment(uint8_t n);
void Print(uint16_t num);

volatile uint16_t tInc=0;
volatile uint16_t tDec=0;
uint16_t t; //Real Counter.

void main()
{
    uint16_t i,j,i2,j2,c;

    init(); 	//Initialize hardware stuff and Display Port....
    Load(); 	//Set Real Counter.
	
    //Initialize Port C.
    DDRC=0b00001111; 	//Set PC0,PC1,PC2,PC3 as output.
    PORTC=0b00001011; 	//Set PC0,PC1,PC3 high.
	
    //Initialize counter and flags.
    c=t-(t/10000)*10000; //Set Temp Counter.
    i=0; j=0; //Flags for increase button.
    i2=0; j2=0; //Flags for decrease button.
    Print(c); //Print Temp Counter.

    while(1){		//Loop.
        CheckLED();
        Inc(&i,&j,&c);
        Dec(&i2,&j2,&c);
        PReset(&c);
    }
}

void init()
{
    TCCR0|=(1<<CS02);		//Prescaler = FCPU/1024.
    TIMSK|=(1<<TOIE0);		//Enable Overflow Interrupt Enable.
    TCNT0=0;				//Initialize Counter.
    DDRB|=0b00001111;		//Port B[3,2,1,0] as out put.
    PORTB=0b00000001;
    SEVEN_SEGMENT_DDR=0XFF;		//Port D.
    SEVEN_SEGMENT_PORT=0X00;	//Turn off all segments.
    sei();				//Enable Global Interrupts.
}

void Load()
{
    eeprom_busy_wait(); 		//Check if EEPROM is ready.
    t=eeprom_read_word(0); 		//Reload Real Counter from EEPROM.
}
void PReset(uint16_t *c)
{
    if(!(PINC & 0b00001000)) { 	//Check if Preset button pressed.
        uint16_t count; 		//Create a new temp Counter.
        eeprom_update_word(0,0); 	//Reset Real Counter in EEPROM.
        Load(); 			//Reload Counter.
        count=0; 			//Set the new Temp Counter.
        *c=count; 			//Reset Temp Counter.
    }						
}
void Inc(uint16_t *a,uint16_t *b,uint16_t *count)
{
    uint16_t i,j,c;
    i=*a; 			//Flag if button pressed.
    j=*b; 			//Slide flag.
    c=*count; 		//Get Temp Counter.
    if (PINC & 0x01) { //Inc Button Released.
        tInc=0; 		//Reset slide timer.
        j=0; 			//Reset slide flag.
        if(i==1) { 		//If there was no slide, increment.
            c=DoInc(c); 	//Increment.
            i=0; 		//Stop increment.
        }				
    }
    else {			//Button Pressed.
        tInc++; 		//Timer for slide.
        if(tInc>25000) {	//Begin slide.
            c=DoInc(c);	//25000-18000=7000.
            tInc=18000; 	//That's the interval between each slide.
            j=1; 		//Slide began.
            i=0; 		//Set i=0 so that it won't increment again, 
 				//when button released.
        }
        if(j==0) i=1; //If there was no slide, set i=1 to increment.
    }
    *a=i; *b=j; *count=c; 	//Pass values to main.
}
void Dec(uint16_t *a,uint16_t *b,uint16_t *count)
{
    uint16_t i, j, c;
    i=*a; 			//Flag if button pressed.
    j=*b;			//Slide flag.
    c=*count; 		//Get Temp Counter.
    if ((PINC & 0x02) && t>0) { //Dec Button Released.
        tDec=0;
        j=0; 			//Reset slide flag.
        if(i==1) {		//If there was no slide, decrement.
            c=DoDec(c); 	//Decrement.
            i=0; 		//Stop decrement.
        }
    }
    else if (t>0) {		//Dec button pressed.
        tDec++;
        if(tDec>25000) {
            c=DoDec(c);	// 25000-18000=7000.
            tDec=18000; 	// That's the interval between each slide.


            j=1; 		//Slide began.
            i=0; 		//Set i=0 so that it won't decrement again, 
//when button released.
        }
        if(j==0) i=1; //If there was no slide, set i=1 to decrement.
    }	
    *a=i; *b=j; *count=c; 	//Pass values to main.
}

uint16_t DoInc(uint16_t c)
{
    uint16_t temp; 			//Create a new counter.
    c++; 				//Increment Temp Counter.
    t++; 				//Increment Real Counter.
    c=CheckIfMax(c); 		//Check if c>9999 or t>19999.
    eeprom_busy_wait(); 		//Check if EEPROM ready.
    eeprom_update_word(0,t); 	//Update the EEPROM Counter.
    temp=eeprom_read_word(0); 	//Set new counter.
    while(temp!=t) 			//Check if Update was successful.
	eeprom_update_word(0,t); 	//Update the EEPROM Counter again.
    Print(c);	
    return c;
}	

uint16_t DoDec(uint16_t c)
{
    uint16_t temp; 			//Create a new counter.
    c=CheckIfMin(c); 		//Check if Counters are zero.
    eeprom_busy_wait(); 		//Check if EEPROM ready.
    eeprom_update_word(0,t); 	//Update the EEPROM Counter.	
    temp=eeprom_read_word(0); 	//Set new counter.
    while(temp!=t) 			//Check if Update was successful.
        eeprom_update_word(0,t); 	//Update the EEPROM Counter again.
    Print(c);
    return c;
}

uint16_t CheckIfMax(uint16_t c)
{
    if(c>9999)
        c=0; //Reset Temp Counter.
    if (t>19999)
        t=0; //Reset Real Counter.
    return c;
}



uint16_t CheckIfMin(uint16_t c)
{
    if(c==0 && t>0) {
        t--;		//Decrement real counter.
        c=t;		//Set Temp Counter to Real Counter.
    }
    else if(t>0) {
        t--;		//Decrement Real counter.
        c--;		//Decrement Temp Counter.
    }
    return c;
}

void CheckLED()
{
    if(t>9999)
        PORTC|=(1<<2); 	//Light LED.
    else
        PORTC&= ~(1<<2); 	//Turn LED off.
}

void SevenSegment(uint8_t n)
{
    /* This function writes a digits given by n to the display
	Note: n must be less than 9 */
    if(n<10){
        switch (n) {
            case 0:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b00111111;
                break;
            case 1:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b00000110;
                break;
            case 2:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b01011011;
                break;
            case 3:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b01001111;
                break;
            case 4:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b01100110;
                break;

            case 5:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b01101101;
                break;
            case 6:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b01111101;
                break;
            case 7:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b00000111;
                break;
            case 8:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b01111111;
                break;
            case 9:
                //.gfedcba
                SEVEN_SEGMENT_PORT=0b01101111;
                break;
        }
    }
    else {
        //This symbol on display tells that n was greater than 9
        //so display can't handle it
        SEVEN_SEGMENT_PORT=0b11111101;
    }
}

ISR(TIMER0_OVF_vect)
{
    /* This interrupt service routine (ISR) Updates the displays */
    static uint8_t i=0;
    if(i==3) {	//If on last display then come back to first.
        i=0;
    }
    else {		// Goto Next display
        i++;
   }

   //Activate a display according to i
   PORTB&=(0b11110000);
   PORTB|=(1<<i);

   //Write the digit[i] in the ith display.
   SevenSegment(digits[i]);
}

void Print(uint16_t num)
{
    /* This function breaks apart a given integer into separate digits
       and writes them to the display array i.e. digits[] */
    uint8_t i=0;
    uint8_t j;

    if(num>9999) return;
    while(num) {
        digits[i]=num%10;
        i++;
        num=num/10;
    }
    for(j=i;j<4;j++) digits[j]=0;
}
