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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "../lib/andygock_avr-uart/uart.h"
#include "rfid_access.h"
#include "hmi_msg.h"

card_t *head = NULL;

void print_card(const card_t *card)
{
    uart0_puts("Info: ");
    for (uint8_t i = 0; i < card->size; i++) {
        char *serial;
            serial = (char *) malloc(sizeof(sizeof(card->uid[i]) / sizeof(card->uid[0])));
            if (serial == NULL) {
                uart0_puts_p(OUT_OF_MEM);
                exit(1);
            }
            sprintf(serial, "%02X", card->uid[i]);
            uart0_puts(serial);
            free(serial);
    }
    
    char *user;
    user = (char *) malloc(strlen(card->user) + 1);
    sprintf(user, " - %s", card->user);
    uart0_puts(user);
    free(user);
    uart0_puts("\r\n");
}

void print_card_list(void)
{
    if (head == NULL) {
        uart0_puts_P("No cards in list");
        uart0_puts_P("\r\n");
    } else {
        card_t *current = head;
        while (current->next != NULL) {
            print_card(current);
            current = current->next;
        }
        print_card(current);
    }
}

card_t* find_card(const card_t *card) 
{
    if (head != NULL) {
        card_t *current;
        current = head;
        while (current != NULL) {
            if (current->size != card->size || !memcmp(current->uid, card->uid, card->size) || ((card->user != NULL) && !strcmp(current->user, card->user))) {
                return current;
            }
            current = current->next;
        }
    }
    
    return NULL;
}

void add_card(const card_t *card)
{
    card_t *in_list = find_card(card);
    
    if (in_list) {
        uart0_puts_P("Already in list!");
        uart0_puts_P("\r\n");
        print_card(in_list);
        return;
    }

    if (head == NULL) {
        head = malloc(sizeof(card_t));
        if (head == NULL) {
            uart0_puts_p(OUT_OF_MEM);
            exit(1);
        }
        head->size = card->size;
        memmove(&head->uid, &card->uid, card->size);
        head->user = malloc(strlen(card->user) + 1);
        strcpy(head->user, card->user);
        head->next = NULL;
        print_card(head);
    } else {
        card_t *current = head;
        
        while(current->next != NULL) {
            current = current->next;
        }
        
        current->next = malloc(sizeof(card_t));
        if (current->next == NULL) {
            uart0_puts(OUT_OF_MEM);
            exit(1);
        }
        
        current->next->size = card->size;
        memmove(&current->next->uid, &card->uid, card->size);
        current->next->user = malloc(strlen(card->user) + 1);
        strcpy(current->next->user, card->user);
        current->next->next = NULL;
        print_card(current->next);
    }
}

void remove_card(const card_t *card) 
{
    card_t *current = head;
    card_t *prev = NULL;
    
    if (head == NULL) {
        uart0_puts_P("No cards in list");
    }
    
    while (current != NULL) {
        if (memcmp(current->uid, card->uid, card->size)) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            
            free(current);
            return;
        }
        
        prev = current;
        current = current->next;
    }
    
    uart0_puts_P("Did not find card");
}
