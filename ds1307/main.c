#include "uart.h"
#include "ds1307.h"
#include <util/delay.h>

void main()
{	
	unsigned char hours, minutes, seconds, month, day, year;
	char str[20];	 //char *str;
	uart_init();
	ds1307_init();
	set_date_time(00, 10, 10, 24, 3, 17); //s,m,hr,d,m,y
	while(1){
		ds1307_get_date_time(&month, &day, &year, &hours, &minutes, &seconds);
		sprintf(str, "%02d-%02d-%02d %02d:%02d:%02d\n\r",day,month,year,hours,minutes,seconds);
		uart_puts(str);
		_delay_ms(3000);
	}
}