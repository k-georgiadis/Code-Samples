/*
 * LCD String.c
 * Created: 20/3/2016 12:21:09 PM
 * Author: Κοσμάς Γεωργιάδης
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

void init_lcd(void);
void lcd_cmd(unsigned char command);
void lcd_data(unsigned char data);
void lcd_send_string(const char* string);
void Scroll_Text(char *string, uint8_t line, uint8_t direction);
void String_Rotate(char *string, uint8_t direction);

uint8_t Length;

volatile uint8_t flag = 0;
	/*
	RS=PB2
       R/W=PB1 //GROUND IT!!!!
       E=PB0
	*/
	
int main(void)
{
    DDRD=0xFF;	//set PORTD as output
    DDRB=0x07;	//Set PB0,PB1 as Output
    PORTB = 0;
    
    init_lcd();
    _delay_ms(3);
	
    lcd_cmd(0x80); 				//Got to first line.
    lcd_send_string("My first string"); 	//Print something.	
    lcd_cmd(0xC0); 				//Go to second line.
    lcd_send_string("G&K Electronics"); 	//Print something.

    while(1) {}
}

void init_lcd()
{
    lcd_cmd(0x38);		//Setup both lines of LCD.
    lcd_cmd(0x0C);		//Set Cursor off - Enable LCD.
    lcd_cmd(0x01);		//Clear Screen.
    lcd_cmd(0x80);		//Line 1, 1st position.
}

void lcd_cmd(unsigned char command)
{
    PORTD = command;		//Put command on the Data Bus
    PORTB = 0x01;		//Enable LCD for command writing
    _delay_ms(2);

    PORTB = 0x00;		//Disable LCD again
    _delay_ms(2);
}

void lcd_data(unsigned char data)
{
    PORTD = data;		//Put data on Data Bus
 
    PORTB = 0x05;           //Set R/S (Register Select) to High,
                            //and Enable to High
    _delay_ms (2);

    PORTB = 0x04;		//Disable LCD again
    _delay_ms (2);
}

void lcd_send_string (const char* string)
{
    while(*string)
    {
        lcd_data(*string);
        string++;
    }
}
