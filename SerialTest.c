#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#include <stdint.h>
#include <stdbool.h>

#define SERIAL_UBBRVAL(Baud)    ((((F_CPU / 16) + (Baud / 2)) / (Baud)) - 1)
#define SERIAL_2X_UBBRVAL(Baud) ((((F_CPU / 8) + (Baud / 2)) / (Baud)) - 1)

#define UART_BUFFER_SIZE	256

volatile char rxChar;

struct UART_Buffer {
	unsigned int in_idx;
	unsigned int out_idx;
	char buff[UART_BUFFER_SIZE];
};

#define IS_BUFFER_EMPTY(buff)	((buff.in_idx - buff.out_idx) == 0)
#define IS_BUFFER_FULL(buff)	((buff.in_idx - buff.out_idx) >= UART_BUFFER_SIZE)

static inline void Serial_Init(const uint32_t BaudRate, const bool DoubleSpeed)
{
	UBRR0  = (DoubleSpeed ? SERIAL_2X_UBBRVAL(BaudRate) : SERIAL_UBBRVAL(BaudRate));

	UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00));
	UCSR0A = (DoubleSpeed ? (1 << U2X0) : 0);
	UCSR0B = ((1 << TXEN0)  | (1 << RXEN0));
}


static inline void Serial_Disable(void)
{
	UCSR0B = 0;
	UCSR0A = 0;
	UCSR0C = 0;

	UBRR0  = 0;
}

static inline bool Serial_IsCharReceived(void)
{
	return ((UCSR0A & (1 << RXC0)) ? true : false);
}

static inline bool Serial_IsSendReady(void)
{
	return ((UCSR0A & (1 << UDRE0)) ? true : false);
}

static inline bool Serial_IsSendComplete(void)
{
	return ((UCSR0A & (1 << TXC0)) ? true : false);
}

static inline void Serial_SendByte(const char DataByte)
{
	while (!(Serial_IsSendReady()));
	UDR0 = DataByte;
}

static inline int16_t Serial_ReceiveByte(void)
{
	if (!(Serial_IsCharReceived()))
		return -1;

	return UDR0;
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	Serial_Init(19200, 1);
	UCSR0B |= (1 << RXCIE0); // enable receiver interrupt
}


int main(void)
{
	SetupHardware();

	sei();	// global interrupt enable

	for (;;)
	{
		if (Serial_IsSendReady())
		{
		  Serial_SendByte(rxChar);

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

