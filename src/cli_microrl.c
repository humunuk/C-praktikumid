#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1},
    {rfid_read_cmd, rfid_read_help, cli_rfid_read, 0},
    {mem_cmd, mem_help, cli_mem_stat, 0}
};


void cli_print(const char *str)
{
    uart0_puts(str);
}


char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA) {
        return uart0_getc() & UART_STATUS_MASK;
    } else {
        return 0x00;
    }
}


void cli_print_help(const char *const *argv)
{
    (void) argv;
    uart0_puts_P("\r\n");
    uart0_puts_P("Implemented commands:\r\n");

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        uart0_puts_p(cli_cmds[i].cmd);
        uart0_puts_P(" : ");
        uart0_puts_p(cli_cmds[i].help);
        uart0_puts_P("\r\n");
    }
}


void cli_print_ver(const char *const *argv)
{
    (void) argv;
    uart0_puts_P("\r\n");
    uart0_puts_P(VER_FW "\r\n");
    uart0_puts_P(VER_AVR " " VER_GCC "\r\n");
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    uart0_puts_P("\r\n");
    print_ascii_table();
    unsigned char ascii[128] = {0};

    for (int i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(ascii, 128);
}


void cli_handle_month(const char *const *argv)
{
    uart0_puts_p(PSTR("\r\n"));
    /*Prepare for LCD display*/
    char match = 0;
    lcd_goto(LCD_ROW_2_START);

    for (unsigned int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&monthNames[i]),
                       strlen(argv[1]))) {
            match = 1;
            lcd_puts_P((PGM_P) pgm_read_word(&monthNames[i]));
            uart0_puts_p((PGM_P) pgm_read_word(&monthNames[i]));
            uart0_puts_P("\r\n");
        }
    }

    if (!match) {
        lcd_goto(LCD_ROW_2_START);
        uart0_puts_P(MONTH_NOT_FOUND);
        uart0_puts_P("\r\n");
    }

    /*Clear rest of the screen*/
    for (int i = 0; i < 16; i++) {
        lcd_putc(' ');
    }
}


void cli_rfid_read(const char *const *argv)
{
    (void) argv;
    Uid uid;
    Uid *uid_ptr = &uid;
    uart0_puts_P("\r\n");
    
    if (PICC_IsNewCardPresent()) {
        uart0_puts("Card selected!\r\n");
        PICC_ReadCardSerial(uid_ptr);
        
        char size[sizeof(uid.size)];
        sprintf(size, "Uid size: 0x%02X\r\n", uid.size);
        uart0_puts(size);
        free(size);
        
        char sak[sizeof(uid.sak)];
        sprintf(sak, "Uid sak: 0x%02X\r\n", uid.sak);
        uart0_puts(sak);
        free(sak);
        
        uart0_puts("Card UID: ");
        
        for (byte i = 0; i < uid.size; i++) {
            char serial[sizeof(uid.uidByte[i]) / sizeof(uid.uidByte[0])];
            sprintf(serial, "%02X", uid.uidByte[i]);
            uart0_puts(serial);
            free(serial);
        }
        
        uart0_puts_P("\r\n");
    } else {
        uart0_puts_P("Unable to select card.\r\n");
    }
}

void cli_mem_stat(const char *const *argv)
{
    (void) argv;
    uart0_puts("\r\n");
    extern int __heap_start, *__brkval;
    int v;
    int space;
    static int prev_space;
    space = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    uart0_puts_P("Heap statistics\r\n");
    char *stat;
    stat = (char *) malloc(sizeof(size_t));
    sprintf(stat, "Used %d\r\n", getMemoryUsed());
    uart0_puts(stat);
    (void) space;
    sprintf(stat, "Free: %d\r\n", getFreeMemory());
    uart0_puts(stat);
    uart0_puts_P("\r\nSpace between stack and heap: \r\n");
    sprintf(stat, "Current %d\r\n", space);
    uart0_puts(stat);
    sprintf(stat, "Previous %d\r\n", prev_space);
    uart0_puts(stat);
    sprintf(stat, "Change %d\r\n", space - prev_space);
    uart0_puts(stat);
    uart0_puts_P("\r\nFreelist\r\n");
    sprintf(stat, "Freelist size: %d\r\n", getFreeListSize());
    uart0_puts(stat);
    sprintf(stat, "Blocks in freelist: %d\r\n", getNumberOfBlocksInFreeList());
    uart0_puts(stat);
    sprintf(stat, "Largest block in freelist %d\r\n", 
            getLargestBlockInFreeList());
    uart0_puts(stat);
    sprintf(stat, "Largest freelist block: %d\r\n", 
            getLargestAvailableMemoryBlock());
    uart0_puts(stat);
    sprintf(stat, "Largest allocable block: %d\r\n", 
            getLargestNonFreeListBlock());
    uart0_puts(stat);
    free(stat);
    prev_space = space;
}


void cli_print_cmd_error(void)
{
    uart0_puts_P("\r\n");
    uart0_puts_P("Command not implemented.\r\n Use <help> to get help.\r\n");
}


void cli_print_cmd_arg_error(void)
{
    uart0_puts_P("\r\n");
    uart0_puts_P("To few or too many arguments for this command\r\nUse <help>\r\n");
}


int cli_execute(int argc, const char * const * argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}
