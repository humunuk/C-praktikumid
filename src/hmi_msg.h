#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define VER_FW "Version: " GIT_DESCR " built on: " __DATE__" "__TIME__
#define VER_AVR "avr-libc version: " __AVR_LIBC_VERSION_STRING__
#define VER_GCC "gcc version: " __VERSION__
#define UPTIME "Uptime: "

const char STUD_NAME[] PROGMEM = "Siim Kallari";
const char CLI_ASK_Q[] PROGMEM = "Enter Month Name First Letter>";

const char Jan[] PROGMEM = "January ";
const char Feb[] PROGMEM = "February ";
const char Mar[] PROGMEM = "March ";
const char Apr[] PROGMEM = "April ";
const char May[] PROGMEM = "May ";
const char Jun[] PROGMEM = "June ";

PGM_P const monthNames[] PROGMEM =
{
    Jan,
    Feb,
    Mar,
    Apr,
    May,
    Jun
};

#endif /* _HMI_MSG_H_ */
