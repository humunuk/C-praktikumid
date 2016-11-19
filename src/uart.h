int uart4_putchar(char c, FILE *stream);
int uart4_getchar(FILE *stream);

void myuart_initialize(void);
int myuart_putchar(char c, FILE *stream);

void uart4_init(void);

/* http://www.ermicro.com/blog/?p=325 */

FILE uart4_io = FDEV_SETUP_STREAM(uart4_putchar, uart4_getchar, _FDEV_SETUP_RW);
FILE myuart_out = FDEV_SETUP_STREAM(myuart_putchar, NULL, _FDEV_SETUP_WRITE);
