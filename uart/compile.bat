avr-gcc -g -Os -mmcu=atmega256rfr2  -c uart.c sendstring.c
avr-gcc -g -mmcu=atmega256rfr2 -o sendstring.elf sendstring.o uart.o
avr-objcopy -j .text -j .data -O ihex sendstring.elf sendstring.hex