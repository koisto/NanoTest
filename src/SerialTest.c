#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#include "Uart0.h"
#include "CircularBuffer.h"



volatile char rxChar;



/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	Uart0_Init(19200, 1);
	UCSR0B |= (1 << RXCIE0); // enable receiver interrupt
}


int main(void)
{
	SetupHardware();

	sei();	// global interrupt enable

	for (;;)
	{
		if (Uart0_IsSendComplete())
		{
		  Uart0_SendByte(rxChar);

		}
	}
}



/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
 *  for later transmission to the host.
 */
ISR(USART_RX_vect, ISR_BLOCK)
{
	uint8_t ReceivedByte = UDR0;
	rxChar = ReceivedByte;

}

