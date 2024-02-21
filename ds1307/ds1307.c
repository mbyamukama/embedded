#include "ds1307.h"

uint8_t bcd_to_dec(uint8_t val)
{
  return( (val/16*10) + (val%16) );
}
uint8_t dec_to_bcd(uint8_t val)
{
  return( (val/10*16) + (val%10) );
}

void ds1307_init()
{
	i2c_init();
}
void ds1307_get_date_time(uint8_t *months, uint8_t *days, uint8_t *years, uint8_t *hours, uint8_t *minutes, uint8_t *seconds)
// returns months, days, and years in BCD format
{
	*days = bcd_to_dec(i2c_read_mem2(I2C_DS1307_ADDR,REG_DAYS)); 
	*months = bcd_to_dec(i2c_read_mem2(I2C_DS1307_ADDR,REG_MONTH));
	*years = bcd_to_dec(i2c_read_mem2(I2C_DS1307_ADDR,REG_YRS));	
	*hours = bcd_to_dec(i2c_read_mem2(I2C_DS1307_ADDR,REG_HRS));
	*minutes = bcd_to_dec(i2c_read_mem2(I2C_DS1307_ADDR,REG_MINS));
	*seconds = bcd_to_dec(i2c_read_mem2(I2C_DS1307_ADDR,REG_SECS));
	if (*hours & 0x40) // 12hr mode:
	*hours &= 0x1F; // use bottom 5 bits (pm bit = temp & 0x20)
	else *hours &= 0x3F; // 24hr mode: use bottom 6 bits	
}
void set_date_time(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint8_t year)
// simple, hard-coded way to set the date.
{
    i2c_write_mem(I2C_DS1307_ADDR,REG_MINS, dec_to_bcd(minute));
	i2c_write_mem(I2C_DS1307_ADDR,REG_SECS, dec_to_bcd(second));
    i2c_write_mem(I2C_DS1307_ADDR,REG_HRS, dec_to_bcd(hour)); // add 0x40 for PM
    i2c_write_mem(I2C_DS1307_ADDR,REG_DAYS, dec_to_bcd(day));
	i2c_write_mem(I2C_DS1307_ADDR,REG_MONTH, dec_to_bcd(month));
	i2c_write_mem(I2C_DS1307_ADDR,REG_YRS, dec_to_bcd(year));
}
