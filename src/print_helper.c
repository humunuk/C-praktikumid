#include <stdio.h>
#include <stdlib.h>
#include "print_helper.h"
#include "../lib/andygock_avr-uart/uart.h"

void print_ascii_table()
{
    for (char c = ' '; c <= '~'; c++) {
        uart0_putc(c);
    }

    uart0_puts("\r\n");
}

void print_for_human(const unsigned char *array, const size_t len)
{
    char c[3];

    for (size_t i = 0; i < len; i++) {
        if (array[i] >= ' ' && array[i] <= '~') {
            uart0_putc(array[i]);
        } else {
            sprintf(c, "\"0x%02x\"", array[i]);
            uart0_puts(c);
        }
    }

    uart0_puts("\r\n");
}


