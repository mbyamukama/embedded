avr-gcc -g -Os -mmcu=atmega128rfa1 -c uart.c sendstring.c
avr-gcc -g -mmcu=atmega128rfa1 -o sendstring.elf sendstring.o uart.o
avr-objcopy -j .text -j .data -O ihex sendstring.elf sendstring.hex