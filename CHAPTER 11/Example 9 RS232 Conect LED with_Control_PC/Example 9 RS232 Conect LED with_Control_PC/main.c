/*
 * LED_Control_PC.c
 *
 * Created: 27/4/2022 1:55:56 μμ
 * Author : Γεωργιάδης Κοσμάς
 */   

#define F_CPU		16000000UL
#define BAUD_RATE	19200

#define LED_DDR		DDRC
#define LED_PORT	PORTC
#define LED_PIN		PINC

#include <avr/io.h>
#include <stdio.h>
#include <ctype.h>
//ΣΥΝΑΡΤΗΣΕΙΣ ΠΟΥ ΘΑ ΧΡΗΣΙΜΟΠΟΙΗΣΟΥΜΕ
void init();

void USARTInit(uint16_t ubrr_value);
char USARTReadChar();
void USARTWriteChar(char data);
void USARTWriteString(char *string);

void setLED(uint8_t state);
// Η ΠΡΩΤΗ ΚΑΙ ΚΥΡΙΑ ΣΥΝΑΡΤΗΣΗ ΠΟΥ ΕΚΤΕΛΕΙ ΤΟ ΠΡΟΓΡΑΜΜΑ ΜΑΣ
void main()
{
	char status[16] = {};
	char data;

	uint16_t UBR = F_CPU/BAUD_RATE - 16; //UBR Value Formula => F_CPU/(BAUD_RATE*16) - 1
	UBR = UBR/16;

	init();
	USARTInit(UBR); 

	while(1)
	{
		data = USARTReadChar(); //Wait for request from PC.
		
		//'S' is the signal for the status request.
		if(data == 's' || data == 'S')
		{
			sprintf(status, "Status: 0x%02X\n", LED_PIN);
			USARTWriteString(status); //Send current state of LED port.
		}
		else
			setLED(data); //Set LED port state.

	}
}

void init()
{
	//Set LED port as output.
	LED_DDR = 0xFF;

	//Turn all LEDs off.
	LED_PORT = 0x00;
}

void USARTInit(uint16_t ubrr_value)
{
   //Set Baud rate.
   UBRRL = ubrr_value;
   UBRRH = (ubrr_value >> 8);

   /* Set Frame Format
   >> Asynchronous mode
   >> No Parity
   >> 1 StopBit

   >> char size 8   */
   UCSRC = (1 << URSEL) | (3 << UCSZ0);

   //Enable The receiver and transmitter.
   UCSRB = (1 << RXEN) | (1 << TXEN); //Τέλος της Αρχικοποίησης
}

char USARTReadChar() //Συναρτηση ληψης δεδομενων
{
   //Wait until a data is available.
   while(!(UCSRA & (1 << RXC)))
   {
      //Do nothing
   }
    
   //Now USART has got data from host.
   return UDR;
}

void USARTWriteChar(char data)  //Συναρτηση αποστολης δεδομενων
{
   //Wait until the transmitter is ready.
   while(!(UCSRA & (1 << UDRE)))
   {
      //Do nothing
   }

   //Now write the data to USART buffer.
   UDR = data;
}

void USARTWriteString(char *string)
{	
	char *head = string; //Save string pointer.
	
	while(*string)
	{
		USARTWriteChar(*string);
			
		string++;
	}		
	string = head; //Restore string pointer.
}

void setLED(uint8_t state)
{
	LED_PORT = state;
}


