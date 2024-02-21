#ifndef UART_H
#define UART_H

#define F_CPU 16000000
void uart_init();
void uart_puts(char *send);
char uart_get();
void uart_putc(char data);
void uart_puts_p(const char *str);
#endif