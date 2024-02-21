 #include <avr/io.h>
 #include <util/delay.h>

#define F_CPU 16000000UL

 /**************************MAIN*******************************/
 int main (void)
 {
	DDRE=0xFF;
	while(1){
		PORTE = 0xFF;
		_delay_ms(200);
		PORTE = 0x00;
		_delay_ms(200);
	}
 }