avr-gcc -g -Os -mmcu=atmega128rfa1  -c uart.c i2c.c ds1307.c main.c
avr-gcc -g -mmcu=atmega128rfa1  -o main.elf uart.o i2c.o ds1307.o main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
