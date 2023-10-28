
/*
 * Inc-Dec_Counter.c
 * Created: 12/4/2020 7:04:54 μμ
 * Author Κοσμάς Γεωργιάδης
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

#define SEVEN_SEGMENT_PORT	PORTD
#define SEVEN_SEGMENT_DDR	DDRD

volatile uint8_t digits[4];

void init();
void Inc(uint16_t *a,uint16_t *b,uint16_t *count);
void Dec(uint16_t *a,uint16_t *b,uint16_t *count);
uint16_t DoInc(uint16_t c);
uint16_t DoDec(uint16_t c);
void SevenSegment(uint8_t n);
void Print(uint16_t num);

 void main()
{
    uint16_t i,j,i2,j2,c;
    init();	//Initialize.
	
    //Initialize Port C.
    DDRC=0b00000011; //Set PC0,PC1 as output.
    PORTC=0b00000011; //Set PC0,PC1 high.

    //Initialize counter and flags.
    c=0; 		//Counter.
    i=0; j=0; 	//Flags for increase button. 
    i2=0; j2=0; 	//Flags for decrease button. 

    while (1){ 	//Loop.
        Inc(&i, &j, &c);
        Dec(&i2, &j2, &c);	
    }
}

void init()
{
   TCCR0|=(1<<CS02);		// Prescaler = FCPU/1024
   TIMSK|=(1<<TOIE0);	// Enable Overflow Interrupt Enable
   TCNT0=0;			// Initialize Counter
   DDRB|=0b00001111;		// Port B[3,2,1,0] as out put
   PORTB=0b00000001;
   SEVEN_SEGMENT_DDR=0XFF;	// Port D
   SEVEN_SEGMENT_PORT=0X00;	// Turn off all segments
   sei();			// Enable Global Interrupts
}
void Inc(uint16_t *a,uint16_t *b,uint16_t *count)
{
    uint16_t i,j,c;
    i=*a; 			// Flag if button pressed.
    c=*count; 		// Get Counter.
    if (PINC & 0x01){ 	// Inc Button Released.
        if(i==1){ 		// If button pressed before, increment.
            c=DoInc(c); 	// Increment.
            i=0; 		// Stop increment.
        }				
    }
    else {			// Inc Button Pressed.
        i=1; 			// Set i=1 to increment.
    }
    *a=i; *b=j; *count=c; 	// Pass values to main.
}

void Dec(uint16_t *a,uint16_t *b,uint16_t *count)
{
    uint16_t i,j,c;
    i=*a; //Flag if button pressed.
    c=*count; //Get Counter.
    if (PINC & 0x02 && c>0){ //Dec Button Released.
        if(i==1){  //If button pressed before, decrement.
            c=DoDec(c); //Decrement. 
            i=0; //Stop decrement. Σταμάτησε την μείωση 
        }
    }
    else if(c>0){ //Dec Button Pressed.
        i=1; //Set i=1 to decrement. Ορίστε το i=1 σε μείωση.
    }			
    *a=i; *b=j; *count=c; //Pass values to main.
}
uint16_t DoInc(uint16_t c)
{
    if(c<9999)
        c++;
    else
        c=0; //Reset Counter.
    Print(c);
    return c;
}
uint16_t DoDec(uint16_t c)
{
    c--; //Decrement.
    Print(c);
    return c;
}
void SevenSegment(uint8_t n)
{
    /*This function writes a digits given by n to the display
      Note:n must be less than 9*/
    if(n<10) {
        switch (n){
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
    else{
        //This symbol on display tells that n was greater than 9
        //so display can't handle it
        SEVEN_SEGMENT_PORT=0b11111101;
    }
}
ISR(TIMER0_OVF_vect)
{
    /* This interrupt service routine (ISR) Updates the displays */
    static uint8_t i=0;
    if (i==3) { 	// If on last display then come back to first.
      i=0;
    }
    else { 		// Goto Next display
      i++;
    }
    PORTB&=(0b11110000);	//Activate a display according to i
    PORTB|=(1<<i);
    SevenSegment(digits[i]); //Write the digit[i] in the ith display.
}
void Print(uint16_t num)
{
    /* This function breaks apart a given integer into separate digits
    and writes them to the display array i.e. digits[] */
    volatile uint8_t i=0;
    uint8_t j;
    if(num>9999) return;
    while(num) {
        digits[i]=num%10;
        i++;	
        num=num/10;
    }
    for(j=i;j<4;j++) digits[j]=0;
}
