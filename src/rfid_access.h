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
#ifndef _RFID_ACCESS_H
#define _RFID_ACCESS_H

typedef struct card {
    uint8_t size;
    uint8_t uid[10];
    char *user;
    struct card *next;
} card_t;

extern card_t *head;
extern card_t* find_card(const card_t *card);
extern void add_card(const card_t *card);
extern void print_card_list(void);
extern void remove_card(const card_t *card);

#endif /*End RFID_ACCESS*/
