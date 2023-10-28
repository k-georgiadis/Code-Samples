/*
 * RS232 USART Interrupt File.c
 *
 * Created: 5/16/2022 4:43:41 PM
 * Author : GEORGIADIS GEORGE
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);//������������ ��������� ��� �����  */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0)|(1 << UCSZ1); /* ������� ���������� 8-bit */

	UBRRL = BAUD_PRESCALE; /* ������� �� ���������� 8-bit ��� ����� baud rate */
	UBRRH = (BAUD_PRESCALE >> 8); /* ������� �� ��� 8-����� (Load upper 8-bits)*/
}

void USARTWriteChar(char data)  //��������� ��������� ���������
{
	//Wait until the transmitter is ready.
	while(!(UCSRA & (1 << UDRE)))
	{
		//Do nothing
	}

	//Now write the data to USART buffer.
	UDR = data;
}

ISR(USART_RXC_vect) //�������� �������� �������� �����������.(Interrupt Service Routine).
{
	PORTB = UDR;
}

int main()
{
	UART_init(9600);
	sei();   //������������ ��� �������� ��� ����������

	DDRB = 0xFF;
	DDRC = 0x01;
	PORTC = 0x01;

	while(1)
	{
		if(!(PINC & 1))
		{
			USARTWriteChar(PINB);
			_delay_ms(200);
		}
	}
}

