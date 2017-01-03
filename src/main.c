#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <time.h>
#include "hmi_msg.h"
#include "cli_microrl.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"

#define UART_BAUD 9600
#define UART_STATUS_MASK 0x00FF

//Microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;

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

static inline void print_initial_info(void)
{
    /* Send uart3 information */
    uart3_puts_p(PSTR("Console started\r\n"));
    uart3_puts_p(PSTR(VER_FW "\r\n"));
    uart3_puts_p(PSTR(VER_AVR " " VER_GCC "\r\n"));
    /* Send uart0 information */
    uart0_puts_p(STUD_NAME);
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(PSTR(CLI_INTRO));
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(PSTR(CLI_LIMITS));
    uart0_puts_p(PSTR("\r\n"));
    /*Send LCD information*/
    lcd_puts_P(STUD_NAME);
}


static inline void heartbeat(void)
{
    static time_t time_y2k_prev;
    time_t time_y2k_cpy;
    time_y2k_cpy = time(NULL);
    char uptime[32];

    if (time_y2k_cpy != time_y2k_prev) {
        PORTA ^= _BV(PORTA3);
        sprintf_P(uptime, PSTR(UPTIME "%lus\r\n"), time_y2k_cpy);
        uart3_puts(uptime);
        time_y2k_prev = time_y2k_cpy;
    }
}

static inline void init_cli(void)
{
    microrl_init(prl, uart0_puts);
    microrl_set_execute_callback(prl, cli_execute);
}

void main (void)
{
    init_hw();
    init_system_clock();
    sei();
    print_initial_info();
    init_cli();

    while (1) {
        heartbeat();
        microrl_insert_char(prl, cli_get_char());
    }
}

/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    system_tick();
}



