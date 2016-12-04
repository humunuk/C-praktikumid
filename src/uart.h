#ifndef _UART_H_
#define _UART_H_

int uart0_putchar(char c, FILE *stream);
int uart0_getchar(FILE *stream);

int uart3_putchar(char c, FILE *stream);

void uart0_initialize(void);
void uart3_initialize(void);

extern FILE uart0_io;
extern FILE uart3_out; 

/* http://www.ermicro.com/blog/?p=325 */
#endif /* _UART_H_ */

