#include "defines.h"
#include "uart.h"
#include <util/delay.h>


inline uint8_t uartRead() {
	while (!(UCSRA & (1<<RXC))) { }
	return UDR;
}


/* init the uart */
inline void uartInit() {
   // Set RXEN for receiving
   // Set TXEN for sending
	UCSRB |= (1<<TXEN) | (1<<RXEN);

   // Set UART settings (8 Bit, 1 Stopbits, no Parity).
   //  see http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART
   UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);

	//UCSRB |= (1<<RXCIE);
	UBRRH = UBRR_VAL >> 8;
   	UBRRL = UBRR_VAL & 0xFF;
}
