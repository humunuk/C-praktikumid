/******************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
******************************************************************/
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
#include "rfid_access.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

#define UART_BAUD 9600
#define UART_STATUS_MASK 0x00FF
#define DOOR_OPEN PORTA |= _BV(PORTA1)
#define DOOR_CLOSE PORTA &= ~_BV(PORTA1)

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

static inline void init_rfid_reader(void)
{
    /*Init RFID-RC522*/
    MFRC522_init();
    PCD_Init();
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

static inline void handle_door()
{
    time_t cur_time = time(NULL);
    static time_t message_start;
    static time_t door_open_start;
    
    if (PICC_IsNewCardPresent()) {
        Uid uid;
        card_t card;
        PICC_ReadCardSerial(&uid);
        card.size = uid.size;
        memcpy(&card.uid, &uid.uidByte, uid.size);
        card.user = NULL;
        card_t *found_card = find_card(&card);
        if (found_card) {
            lcd_goto(0x40);
            lcd_puts(found_card->user);
            for (int i = 0; i < 16; i++) {
                lcd_putc(' ');
            }
            DOOR_OPEN;
        } else {
            DOOR_CLOSE;
            lcd_goto(LCD_ROW_2_START);
            lcd_puts("Access denied");
            for (int i = 0; i < 16; i++) {
                lcd_putc(' ');
            }
        }
        door_open_start = cur_time;
        message_start = cur_time;
    }
    if ((message_start + 2) < cur_time) {
        // clean the screen once again in 10 secconds so that cli command
        // `month` could show found months for some time
        message_start = cur_time + 2;
        lcd_goto(LCD_ROW_2_START);
        for (int i = 0; i < 16; i++) {
            lcd_putc(' ');
        }
    }
    if ((door_open_start + 2) < cur_time) {
        DOOR_CLOSE;
    }
}

void main (void)
{
    init_hw();
    init_system_clock();
    init_rfid_reader();
    sei();
    print_initial_info();
    init_cli();

    while (1) {
        heartbeat();
        microrl_insert_char(prl, cli_get_char());
        handle_door();
    }
}

/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    system_tick();
}



