
/* BUTTON ON-OFF-LED.c
 * Created: 4/22/2022 1:41:04 ЛЛ
 * Author: цеыяциадгс цеыяциос
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
    DDRC=0b00000001;
    DDRD=0b00000001;
    //PORTD |= (1<<PIND0)??
    PORTC |= (1<<PINC0);

    char button_pressed = 0;

    while (1) //infinite loop
    {
        if (PINC & (1<<PINC0)) //If switch is released
        {
            if(button_pressed)
                PORTD ^= (1<<PIND0); //Toggle LED
	
            button_pressed = 0;
        }
        else //If switch is pressed.
            button_pressed = 1;
    }
}

