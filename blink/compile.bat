avr-gcc -g -Os -std=c99 -mmcu=atmega256rfr2  -c ledflash.c
avr-gcc -g -Os  -mmcu=atmega256rfr2 -o ledflash.elf ledflash.o
avr-objcopy -j .text -j .data -O ihex ledflash.elf ledflash.hex