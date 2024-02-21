avr-gcc -g -Os -std=c99 -mmcu=atmega256rfr2  -c uart.c i2c.c mcp3424.c main.c
avr-gcc -g -mmcu=atmega256rfr2  -o main.elf uart.o i2c.o mcp3424.o main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
