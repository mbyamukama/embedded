avr-gcc -g -Os -mmcu=atmega256rfr2  -c main.c ff.c diskio.c  uart.c
avr-gcc -g -mmcu=atmega256rfr2 -o main.elf main.o ff.o diskio.o uart.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex