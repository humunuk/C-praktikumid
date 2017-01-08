#ifndef _CLI_MICRORL_H_
#define _CLI_MICRORL_H_

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))
#define UART_STATUS_MASK    0x00FF

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

void cli_print(const char * str);
char cli_get_char(void);
void cli_print_help(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_rfid_read(const char *const *argv);
void cli_mem_stat(const char *const *argv);

int cli_execute(int argc, const char *const *argv);

#endif /*END CLI_MICRORL_H*/
