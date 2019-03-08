#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#include "Uart0.h"
#include "CircularBuffer.h"

CircularBuffer buffer;
char buffer_data[64];

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	CircularBufferInit(&buffer, buffer_data, sizeof(buffer_data));

	/* Hardware Initialization */
	Uart0_Init(19200, 1);
	
	Uart0_RXC_IntEnable();

}


int main(void)
{
	SetupHardware();

	sei();	// global interrupt enable
	Uart0_SendByte('h');
	Uart0_SendByte('e');
	Uart0_SendByte('l');
	Uart0_SendByte('l');
	Uart0_SendByte('o');
	Uart0_SendByte('\n');
	for (;;)
	{
		if (!CircularBufferEmpty(&buffer))
		{
			char c;
			CircularBufferRemove(&buffer, &c);
		  	Uart0_SendByte(c);
		}
	}
}



/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
 *  for later transmission to the host.
 */
ISR(USART_RX_vect, ISR_BLOCK)
{
	uint8_t ReceivedByte = UDR0;

	if(!CircularBufferFull(&buffer))
		CircularBufferInsert(&buffer, ReceivedByte);

}

