#include <avr/pgmspace.h>
#include "hmi_msg.h"

const char STUD_NAME[] PROGMEM = "Siim Kallari";
const char CLI_ASK_Q[] PROGMEM = "Enter Month Name First Letter>";

const char Jan[] PROGMEM = "January ";
const char Feb[] PROGMEM = "February ";
const char Mar[] PROGMEM = "March ";
const char Apr[] PROGMEM = "April ";
const char May[] PROGMEM = "May ";
const char Jun[] PROGMEM = "June ";

PGM_P const monthNames[] PROGMEM = {
    Jan,
    Feb,
    Mar,
    Apr,
    May,
    Jun
};

/*CLI*/

const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char ascii_cmd[] PROGMEM = "ascii";
const char ascii_help[] PROGMEM = "print ASCII tables";
const char month_cmd[] PROGMEM = "month";
const char month_help[] PROGMEM =
    "Find matching month from lookup list. Usage: month <string>";

