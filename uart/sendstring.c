#include "uart.h"
#include <stdio.h>
#include <util/delay.h>

int main( void ) {
    // fire up the usart
    uart_init ();
	char str[20];
    // main loop
    while(1) {
		sprintf(str, "%02d:%02d:%02d\n\r", 13,3,5);
            uart_puts(str);
			_delay_ms(1000);
        }
}