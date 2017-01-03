#include <stdio.h>
#include "print_helper.h"

int print_ascii_table()
{
    for (char c = ' '; c <= '~'; c++) {
        if (!printf("%c ", c)) {
            return 0;
        }
    }

    return printf("\n");
}

int print_for_human(const unsigned char *array, const size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (array[i] >= ' ' && array[i] <= '~') {
            if (!printf("%c", array[i])) {
                return 0;
            }
        } else {
            if (!printf("\"0x%02x\"", array[i])) {
                return 0;
            }
        }
    }

    return printf("\n");
}


