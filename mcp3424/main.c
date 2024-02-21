#include "uart.h"
#include "mcp3424.h"
#include <stdio.h>
#include <util/delay.h>

void main()
{	 
	char str[150];
	int len=0,chan=1;
	int32_t result=0;
	uart_init();
	mcp3424_init_config(MCP3424_ADDRESS,0,8,16);
    uart_puts("config complete in main\n");

	/*while(1){
		len=0;
		if(chan>4) chan=1;
		while(chan <=4) 
		{
			/*mcp3424_start_measure(chan);
			while(!is_measurement_ready());
			result = get_measurement_uv();		
		    len += sprintf(&str[len],"V on CHAN %d is %ld\n",chan,result); 
			uart_puts("chan incremented\n");
			++chan;
		}
		uart_puts(str);
		_delay_ms(1000);
	}*/
}