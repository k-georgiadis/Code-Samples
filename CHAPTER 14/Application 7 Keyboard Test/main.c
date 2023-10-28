/*
 * Keyboard Test.c
 *
 * Created: 11/5/2020 12:24:39 μμ
 * Author : Κοσμάς Γεωργιάδης
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

//Define LCD enable and register pins.
#define E			PB0
#define RW			PB1
#define RS			PB2
						
void Port_Init();
void LCD_Init();

void Keypad();
void MoveCursor(uint8_t direction);

void lcd_cmd(uint8_t command);
void lcd_data(uint8_t data);
void lcd_send_string(char* string, uint8_t IsDelayed);

//4x3 Keyboard Matrix.
char row_col [4][3] = { {'1','2','3'},
						{'4','5','6'},
						{'7','8','9'},
						{'*','0','#'} };
						
uint8_t CursorAddress;

int main(void)
{
	Port_Init();
	LCD_Init();
	
    while (1)
    {
		Keypad();
    }
}

void Port_Init()
{
	//LCD/Keypad init.
	DDRD = 0xFF; //Set PORTD as Output.
	DDRB = 0x3F; //Set E, RW, RS and columns as output.
	PORTB = 0x38; //Set keypad columns HIGH (1).
	
	//Keypad init.
	DDRC = 0x00; //Rows as input.
	PORTC = 0x0F; //Enable pull-ups on rows.
	
	ADCSRA &= ~(1 << ADEN); //Disable ADC.
	ACSR = (1 << ACD); //Disable the analog comparator.
}
void LCD_Init()
{
	//Setup both lines of LCD.
	lcd_cmd(0x38);
	
	//Set blinking cursor on - Enable LCD.
	lcd_cmd(0x0F);
	
	//Clear Screen.
	lcd_cmd(0x01);
	
	//Line 1, 1st position.
	lcd_cmd(0x80);
	CursorAddress = 0x80;
}

void Keypad()
{
	uint8_t i, j;
	char input;

	for(j = 0; j < 3; j++) //COL_1 = PB3
	{
		PORTB &= ~(8 << j); //Check each column by feeding it with 0.

		for(i = 0; i < 4; i++) //ROW_4 = PC3
		{
			if((PINC & (1 << i)) == 0)
			{
				input = row_col[i][j];

				while((PINC & (1 << i)) == 0){};

				lcd_data(input);
				break;
			}
		}
		PORTB |= (8 << j); //Set keypad column high again.
	}
	
}

void MoveCursor(uint8_t direction)
{
	if(direction == 1)
	{
		CursorAddress++; //Increment address.
		
		if(CursorAddress > 0x8F && CursorAddress < 0xC0)
			CursorAddress = 0xC0;
		else if(CursorAddress > 0xCF)
			CursorAddress = 0x80;
		else
			return;
	}
	else
	{
		CursorAddress--; //Decrement address.
		
		if(CursorAddress < 0x80)
			CursorAddress = 0xCF;
		else if(CursorAddress < 0xC0 && CursorAddress > 0x8F)
			CursorAddress = 0x8F;
		else
			return;	
	}

	lcd_cmd(CursorAddress);
	return;
}

void lcd_cmd(uint8_t command)
{
	//Put command on the Data Bus.
	PORTD = command;

	//Enable LDC for command.
	PORTB &= ~(1 << RS);
	PORTB |= (1 << E);
	_delay_ms(2);

	//Disable LCD.
	PORTB &= ~(1 << E);
	_delay_ms(2);
}
void lcd_data(uint8_t data)
{
	//Put data on Data Bus.
	PORTD = data;
	
	//Enable LDC for data.
	PORTB |= (1 << RS) | (1 << E);
	
	_delay_ms(2);

	//Disable LCD again.
	PORTB &= ~(1 << E);
	_delay_ms(2);

	MoveCursor(1); //Move cursor forward.
}
void lcd_send_string(char* string, uint8_t IsDelayed)
{
	while(*string)
	{
		lcd_data(*string);
		
		if(IsDelayed)
			_delay_ms(56.0);
		string++;
	}
}
