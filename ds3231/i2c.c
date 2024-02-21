#include "i2c.h"
#include <util/twi.h>

void i2c_init()
{
	/* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */

	TWSR = 0;                         /* no prescaler */
	TWBR = ((F_CPU / F_SCL) - 16) / 2;  /* must be > 10 for stable operation */
}

/*put i2c into start condition with device address*/
uint8_t i2c_start(uint8_t addr)
{
	uint8_t twst;
	uint32_t n;

	/* Send START condition */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	/* Wait until transmission completed */
	for(n = 0; n < 100000 && !(TWCR & (1 << TWINT)); n++) {
	}
	if(n >= 100000) {
		return 1;
	}

	/* check value of TWI Status Register. Mask prescaler bits. */
	twst = TW_STATUS & 0xF8;
	if((twst != TW_START) && (twst != TW_REP_START)) {
		return 1;
	}

	/* send device address */
	TWDR = addr;
	TWCR = (1 << TWINT) | (1 << TWEN);

	/* wail until transmission completed and ACK/NACK has been received */
	for(n = 0; n < 100000 && !(TWCR & (1 << TWINT)); n++) {
	}
	if(n >= 100000) {
		return 1;
	}

	/* check value of TWI Status Register. Mask prescaler bits. */
	twst = TW_STATUS & 0xF8;
	if((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK)) {
		return 1;
	}

	return 0;
}


void i2c_start_wait(uint8_t addr)
{
	uint8_t   twst;
	while ( 1 )
	{
		// send START condition
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
		// wait until transmission completed
		while(!(TWCR & (1<<TWINT)));
		// check value of TWI Status Register. Mask prescaler bits.
		twst = TW_STATUS & 0xF8;
		if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
		// send device address
		TWDR = addr;
		TWCR = (1<<TWINT) | (1<<TWEN);
		// wail until transmission completed
		while(!(TWCR & (1<<TWINT)));
		// check value of TWI Status Register. Mask prescaler bits.
		twst = TW_STATUS & 0xF8;
		if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) )
		{
			/* device busy, send stop condition to terminate write operation */
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
			// wait until stop condition is executed and bus released
			while(TWCR & (1<<TWSTO));
			continue;
		}
		//if( twst != TW_MT_SLA_ACK) return 1;
		break;
	}
}

void
i2c_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	/* wait until ready */
	while(TWCR & (1<<TWSTO));
}
void
i2c_write(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while((TWCR & (1 << TWINT)) == 0) ;
}

uint8_t
i2c_readAck(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while((TWCR & (1 << TWINT)) == 0) ;
	return TWDR;
}

uint8_t
i2c_readNak(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	while((TWCR & (1 << TWINT)) == 0) ;
	return TWDR;
}

void
i2c_write_mem(uint8_t addr, uint8_t reg, uint8_t value)
{
	i2c_start(addr | I2C_WRITE);
	i2c_write(reg);
	i2c_write(value);
	i2c_stop();
}

uint8_t
i2c_read_mem2(uint8_t addr, uint8_t reg)
{
	uint8_t data = 0;
	i2c_start(addr); // send device address
	i2c_write(reg); // set register pointer
	i2c_start(addr | I2C_READ); // restart as a read operation
	data = i2c_readNak(); // read the register data
	i2c_stop(); // stop
	return data;
}

void
i2c_read_mem(uint8_t addr, uint8_t reg, uint8_t buf[], uint8_t bytes)
{
	uint8_t i = 0;
	i2c_start(addr | I2C_WRITE);
	i2c_write(reg);
	i2c_start(addr | I2C_READ);
	for(i = 0; i < bytes; i++) {
		if(i == bytes - 1) {
			buf[i] = i2c_readNak();
		} else {
			buf[i] = i2c_readAck();
		}
	}
	i2c_stop();
}

