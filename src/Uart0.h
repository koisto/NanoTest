#ifndef __UART0_H__
#define __UART0_H__

#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>

void Uart0_Init(const uint32_t BaudRate, const bool DoubleSpeed);

void Uart0_Disable(void);

#define Uart0_IsCharReceived() 		(UCSR0A & (1 << RXC0))
#define Uart0_IsSendReady()			(UCSR0A & (1 << UDRE0))
#define Uart0_IsSendComplete()		(UCSR0A & (1 << TXC0))
#define Uart0_RXC_IntEnable()		UCSR0B |= (1 << RXCIE0)
#define Uart0_RXC_IntDisable()		UCSR0B &= ~(1 << RXCIE0)
#define Uart0_TXC_IntEnable()		UCSR0B |= (1 << TXCIE0)
#define Uart0_TXC_IntDisable()		UCSR0B &= ~(1 << TXCIE0)
#define Uart0_UDR_IntEnable() 		UCSR0B |= (1 << UDRIE0)
#define Uart0_UDR_IntDisable()		UCSR0B &= ~(1 << UDRIE0)

void Uart0_SendByte(const char DataByte);

int16_t Uart0_ReceiveByte(void);

#endif