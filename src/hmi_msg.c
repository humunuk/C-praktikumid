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
#include <avr/pgmspace.h>
#include "hmi_msg.h"

const char STUD_NAME[] PROGMEM = "Siim Kallari";
const char CLI_ASK_Q[] PROGMEM = "Enter Month Name First Letter>";
const char OUT_OF_MEM[] PROGMEM = "Can not allocate memory";

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
const char rfid_read_cmd[] PROGMEM = "read";
const char rfid_read_help[] PROGMEM =
    "Read RFID card thats close to RFID reader";
const char mem_cmd[] PROGMEM = "mem";
const char mem_help[] PROGMEM = "Get stats of memory usage";
const char rfid_add_cmd[] PROGMEM = "add";
const char rfid_add_help[] PROGMEM = 
    "Add RFID card to access list. Card must be readable by reader while adding. Usage: add <name>";
const char rfid_list_cmd[] PROGMEM = "list";
const char rfid_list_help[] PROGMEM = 
    "List all RFID cards in access list";
const char rfid_remove_cmd[] PROGMEM = "remove";
const char rfid_remove_help[] PROGMEM = 
    "Place card on reader and type remove";
