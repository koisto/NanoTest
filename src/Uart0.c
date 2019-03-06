#include "Uart0.h"

#include <avr/io.h>

#define SERIAL_UBBRVAL(Baud)    ((((F_CPU / 16) + (Baud / 2)) / (Baud)) - 1)
#define SERIAL_2X_UBBRVAL(Baud) ((((F_CPU / 8) + (Baud / 2)) / (Baud)) - 1)

 void Uart0_Init(const uint32_t BaudRate, const bool DoubleSpeed)
{
	UBRR0  = (DoubleSpeed ? SERIAL_2X_UBBRVAL(BaudRate) : SERIAL_UBBRVAL(BaudRate));

	UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00));
	UCSR0A = (DoubleSpeed ? (1 << U2X0) : 0);
	UCSR0B = ((1 << TXEN0)  | (1 << RXEN0));
}


 void Uart0_Disable(void)
{
	UCSR0B = 0;
	UCSR0A = 0;
	UCSR0C = 0;

	UBRR0  = 0;
}

 bool Uart0_IsCharReceived(void)
{
	return ((UCSR0A & (1 << RXC0)) ? true : false);
}

 bool Uart0_IsSendReady(void)
{
	return ((UCSR0A & (1 << UDRE0)) ? true : false);
}

 bool Uart0_IsSendComplete(void)
{
	return ((UCSR0A & (1 << TXC0)) ? true : false);
}

 void Uart0_SendByte(const char DataByte)
{
	while (!(Uart0_IsSendReady()));
	UDR0 = DataByte;
}

 int16_t Uart0_ReceiveByte(void)
{
	if (!(Uart0_IsCharReceived()))
		return -1;

	return UDR0;
}