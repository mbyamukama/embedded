#ifndef _DS1307_H
#define _ds1307_H 

#include <avr/io.h> 
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "i2c.h"

// I2C (TWI) ROUTINES

#define REG_SECS 0x00
#define REG_MINS 0x01
#define REG_HRS 0x02
#define DAYOFWK_REGISTER 0x03
#define REG_DAYS 0x04
#define REG_MONTH 0x05
#define REG_YRS 0x06
#define CONTROL_REGISTER 0x07
#define RAM_BEGIN 0x08
#define RAM_END 0x3F



uint8_t bcd_to_dec(uint8_t val);
uint8_t dec_to_bcd(uint8_t val);

void ds1307_init();
void ds1307_get_date_time(uint8_t *months, uint8_t *days, uint8_t *years, uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
void set_date_time(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint8_t year);
#endif