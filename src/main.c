#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"

void main (void)
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    
    /* Init error console as stderr in UART3 and print user code info */
    uart3_initialize();
    stderr = &uart3_out;
    fprintf_P(stderr, PSTR(VER_FW "\n"));
    fprintf_P(stderr, PSTR(VER_AVR " " VER_GCC "\n"));
    /* End UART3 init and info print */
    
    /* Init CLI (stdin and stdout) print student name*/
    uart0_initialize();
    stdin = stdout = &uart0_io;
    fprintf_P(stdout, STUD_NAME, "\n");
    /* End UART0 init and info print */
    
    /* Init LCD print student name */
    lcd_init();
    lcd_puts_P(STUD_NAME);
    /* End init LCT and print info */
    
    /* Print ASCII maps */
    print_ascii_table(stdout);
    unsigned char ascii[128] = {0};

    for (int i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);
    /* End print ASCII maps */
    
    char match = 0;

    while (1) {
        /* Set pin 3 high to turn LED on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        
        /*Prepare for LCD display*/
        char inBuf = ' ';
        lcd_goto(LCD_ROW_2_START);
        fprintf_P(stdout, CLI_ASK_Q, "\n");
        if (!match) {
        lcd_puts_P(CLI_ASK_Q);
        lcd_goto(LCD_ROW_2_START);
        }
        fscanf(stdin, "%c", &inBuf);
        fprintf(stdout, "%c\n", inBuf);
        
        for (unsigned int i = 0; i < 6; i++) 
        {
            if (!strncmp_P(&inBuf, (PGM_P) pgm_read_word(&monthNames[i]), 1)) {
                match = 1;
                lcd_puts_P((PGM_P) pgm_read_word(&monthNames[i]));
                fprintf_P(stdout, PSTR("%S\n"),(PGM_P) pgm_read_word(&monthNames[i]));
            }
        }
        
        /*Clear rest of the screen*/
        for (int i = 0; i < 16; i++) {
            lcd_putc(' ');
        }

        /* Set pin 3 low to turn LED off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}



