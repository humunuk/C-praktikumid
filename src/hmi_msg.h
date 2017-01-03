#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define VER_FW "Version: " GIT_DESCR " built on: " __DATE__" "__TIME__
#define VER_AVR "avr-libc version: " __AVR_LIBC_VERSION_STRING__
#define VER_GCC "gcc version: " __VERSION__
#define UPTIME "Uptime: "
#define CLI_INTRO "Use backspace to delete entry and enter to confirm"
#define CLI_LIMITS "Arrow key's and del do not work currently"
#define MONTH_NOT_FOUND "Month not found"

extern PGM_P const monthNames[];

extern const char STUD_NAME[];
extern const char CLI_ASK_Q[];

extern const char Jan[] PROGMEM;
extern const char Feb[] PROGMEM;
extern const char Mar[] PROGMEM;
extern const char Apr[] PROGMEM;
extern const char May[] PROGMEM;
extern const char Jun[] PROGMEM;

/*CLI*/

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];
#endif /* _HMI_MSG_H_ */
