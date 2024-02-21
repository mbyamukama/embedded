#include <stdint.h>
#include <avr/io.h>


#ifndef F_CPU
#define F_CPU 16000000UL
#endif
/* Here we define the i2c address for dev we support */
#define I2C_AT24MAC_ADDR  0xB0 /* EUI64 ADDR */
#define I2C_SHT2X_ADDR    (0x40 << 1) /* SHT2X ADDR */
#define I2C_BME280_ADDR   (0x77 << 1) /* Alternative 0x76 */
#define I2C_DS1307_ADDR   (0xD0) // I2C bus address of DS1307 RTC

#define I2C_READ    1
#define I2C_WRITE   0
#define F_SCL 100000L // I2C clock speed 100 KHz


void i2c_init();
uint8_t i2c_start(uint8_t addr);
void i2c_start_wait(uint8_t addr);
void i2c_stop(void);
void i2c_write(uint8_t u8data);
uint8_t i2c_readAck(void);
uint8_t i2c_readNak(void);
uint8_t i2c_getstatus(void);
void i2c_read_mem(uint8_t addr, uint8_t reg, uint8_t buf[], uint8_t bytes);
uint8_t i2c_read_mem2(uint8_t addr, uint8_t reg);
void i2c_write_mem(uint8_t addr, uint8_t reg, uint8_t value);
