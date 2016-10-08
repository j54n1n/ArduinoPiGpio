/*
 * EEPROM.cpp
 *
 *  Created on: 30 lug 2016
 *      Author: Lorenzo Miori
 *  Modified 8 October 2016 by Julian Sanin
 */

#include <EEPROM.h>

#include <stdio.h>

#define EEPROM_FILE "eeprom.bin"

EEPROMClass EEPROM;

FILE* eeprom_image = NULL;

static void persistent_init() {
    // Open the fake eeprom image.
    if(eeprom_image <= NULL) {
        eeprom_image = fopen(EEPROM_FILE, "r+");
    }
    // Create file if it does not exist.
    if(eeprom_image <= NULL) {
        eeprom_image = fopen(EEPROM_FILE, "a");
	fclose(eeprom_image);
	eeprom_image = NULL;
        persistent_init();
    }
}

uint8_t EEPROMClass::read( int idx ) {
    uint8_t data;
    persistent_init();
    if (eeprom_image > NULL)
    {
        fseek(eeprom_image, idx, SEEK_SET);
        size_t num = fread(&data, 1U, 1U, eeprom_image);
        if(num != 1U) {
            data = 0xFFU;
	}
    }
    else
    {
        /* not available, return unprogrammed byte */
        data = 0xFFU;
    }

    return data;
}

void EEPROMClass::write( int idx, uint8_t val ) {
    persistent_init();
    if (eeprom_image > NULL)
    {
        fseek(eeprom_image, idx, SEEK_SET);
        fwrite(&val, 1U, 1U, eeprom_image);
    }
    else
    {
        /* not available */
    }
}
