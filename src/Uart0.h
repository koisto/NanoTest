#ifndef __UART0_H__
#define __UART0_H__

#include <stdint.h>
#include <stdbool.h>


void Uart0_Init(const uint32_t BaudRate, const bool DoubleSpeed);

void Uart0_Disable(void);

bool Uart0_IsCharReceived(void);

bool Uart0_IsSendComplete(void);

void Uart0_SendByte(const char DataByte);

int16_t Uart0_ReceiveByte(void);

#endif