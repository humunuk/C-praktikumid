#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <time.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"

#define UART_BAUD 9600
#define UART_STATUS_MASK 0x00FF

int uart0_getchar(FILE *stream)
{
    (void)  stream;
    return  (uart0_getc() & UART_STATUS_MASK);
}

int uart0_putchar(char c, FILE *stream)
{
    (void)  stream;

    if  (c  ==  '\n') {
        uart0_putc('\r');
        uart0_putc('\n');
        return  0;
    }

    uart0_putc((uint8_t) c);
    return  0;
}


FILE uart0_io = FDEV_SETUP_STREAM(uart0_putchar, uart0_getchar, _FDEV_SETUP_RW);

int uart3_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putc('\r');
        uart3_putc('\n');
        return 0;
    }

    uart3_putc((uint8_t) c);
    return 0;
}
FILE uart3_out = FDEV_SETUP_STREAM(uart3_putchar, NULL, _FDEV_SETUP_WRITE);

static inline void init_system_clock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}

static inline void init_hw()
{
    /* Set pin 3 of PORTA (ARDUINO mega pin 25) for output and set low */
    DDRA |= _BV(DDA3);
    PORTA &= ~_BV(PORTA3);
    /*Init uart3*/
    uart3_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    /*Init uart0*/
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    /*Init LCD*/
    lcd_init();
}

static inline void print_required_ascii(void)
{
    print_ascii_table(&uart0_io);
    unsigned char ascii[128] = {0};

    for (int i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(&uart0_io, ascii, 128);
}

static inline void print_initial_info(void)
{
    /* Send uart3 information */
    uart3_puts_p(PSTR("Console started\r\n"));
    uart3_puts_p(PSTR(VER_FW "\r\n"));
    uart3_puts_p(PSTR(VER_AVR " " VER_GCC "\r\n"));
    /* Send uart0 information */
    uart0_puts_p(PSTR("Console started\r\n"));
    uart0_puts_p(STUD_NAME);
    /*Send LCD information*/
    lcd_puts_P(STUD_NAME);
    /*Send the required ASCIIs*/
    print_required_ascii();
    uart0_puts_p(CLI_ASK_Q);
}

static inline void search_month(uint8_t month_letter)
{
    /*Prepare for LCD display*/
    char match = 0;
    char inBuf = (char) month_letter;
    lcd_goto(LCD_ROW_2_START);
    uart0_putc(inBuf);
    uart0_putc('\r');
    uart0_putc('\n');

    if (!match) {
        lcd_puts_P(CLI_ASK_Q);
        lcd_goto(LCD_ROW_2_START);
    }

    for (unsigned int i = 0; i < 6; i++) {
        if (!strncmp_P(&inBuf, (PGM_P) pgm_read_word(&monthNames[i]), 1)) {
            match = 1;
            lcd_puts_P((PGM_P) pgm_read_word(&monthNames[i]));
            uart0_puts_p((PGM_P) pgm_read_word(&monthNames[i]));
            uart0_puts("\r\n");
        }
    }

    uart0_puts_p(CLI_ASK_Q);

    /*Clear rest of the screen*/
    for (int i = 0; i < 16; i++) {
        lcd_putc(' ');
    }
}

static inline void heartbeat(void)
{
    static time_t time_y2k_prev;
    time_t time_y2k_cpy;
    time_y2k_cpy = time(NULL);

    if (time_y2k_cpy != time_y2k_prev) {
        PORTA ^= _BV(PORTA3);
        fprintf_P(&uart3_out, PSTR(UPTIME"%lu s\n"), time_y2k_cpy);
        time_y2k_prev = time_y2k_cpy;
    }
}

void main (void)
{
    init_hw();
    init_system_clock();
    sei();
    print_initial_info();
    uint8_t input;

    while (1) {
        heartbeat();

        if (uart0_peek() != UART_NO_DATA) {
            input = uart0_getc() & UART_STATUS_MASK;
            search_month(input);
        }
    }
}

/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    system_tick();
}



