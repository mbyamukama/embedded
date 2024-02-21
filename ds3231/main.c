#include "uart.h"
#include "ds3231.h"
#include <util/delay.h>

void main()
{	 
	char str[25], year[4];
	uart_init();
	ds3231_init();
	
	datetime_t datetime;
    datetime.year=17;	
	datetime.month=4;
	datetime.day=25;
	datetime.hours=15;
	datetime.mins=46;
	datetime.secs=55;
	
	ds3231_set_datetime(&datetime);
	while(1){
		ds3231_get_datetime(&datetime);
		
		sprintf(year,"20%d",datetime.year); //get 2017 from 17
		sprintf(str, "%s-%02d-%02d %02d:%02d:%02d\n\r",year,datetime.month,datetime.day, 
				datetime.hours,datetime.mins,datetime.secs);
		uart_puts(str);
		_delay_ms(2000);
	}
}