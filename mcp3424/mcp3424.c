/*
MCP3424.c
Library for using the mcp3424 adc to I2C converter
*/

#include "MCP3424.h"
#include "uart.h"
#include <stdio.h> //temp

int address;      // I2C bus address
uint8_t resolution;   // ADC resolution
uint8_t gain;         // Gain
int32_t value;                  // Last measurement result, in uV
char str[15]; //temp


void mcp3424_init_config(uint8_t _address, uint8_t channel, uint8_t _gain, uint8_t _resolution) 
{
	// Configure the device for one-shot mode. Resolution and gain don't matter here.
	i2c_init();
	address = _address; gain=_gain; resolution=_resolution;
	

	
	unsigned char PGAgain = 0;
	unsigned char sampleRate = 0;
	unsigned char conversionModeBit = 0; //1=Continuous, 0=One Shot
	unsigned char channelBits = 0; //0 = Channel 1
	channelBits = channel - 1; //zero based

	switch(gain) {
		case('8'):
		PGAgain = 0x03;
		break;
		case('4'):
		PGAgain = 0x02;
		break;
		case('2'):
		PGAgain = 0x01;
		break;
		case('1'):
		PGAgain = 0x00;
		break;
	default:
		PGAgain = 0x0;
	}
	
	switch(resolution) {
		case(18):
		sampleRate = 0x03; //3.75 sps (18 bits), 3 bytes of data
		break;
		case(16):
		sampleRate = 0x02; //2 bytes of data,
		break;
		case(14):
		sampleRate = 0x01; //2 bytes of data
		break;
		case(12):
		sampleRate = 0x00; //240 SPS (12 bits), 2 bytes of data
		break;
	default:
		sampleRate = 0x00;
	}

	unsigned char config = PGAgain;
	config = config | (sampleRate << 2);
	config = config | (conversionModeBit << 4);
	config = config | (channelBits << 5);
	config = config | (1 << 7); //write a 1 here to initiate a new conversion in One-shot mode
	
	sprintf(str, "addr-%d config - %d ", address,config);
	uart_puts(str);
	
	i2c_start(address | I2C_WRITE);
	uart_puts("i2c started");
	i2c_write(config);        /*ATTN: WE HAVE AN ERROR HERE*/
	uart_puts("i2c write complete");
	i2c_stop();
	
	uart_puts("config has been set");
}

// calculate and return mV divisor from gain and resolution.
int get_mv_divisor()
{
	int mvdivisor = 1 << (gain + 2*resolution);
	return mvdivisor;
}


void mcp3424_start_measure() {
	i2c_start_wait(address | I2C_WRITE);
	i2c_write(MCP342X_START);
	i2c_stop();
}


bool is_measurement_ready() {
	// Assume <18-bit mode. Note that this needs to handle 
	// 4-byte responses to work with 18-bit mode.
	int16_t rawvalue = 0;
	// Ask for 3 bytes from the sensor
	i2c_start(address | I2C_READ);
	// Read the first two bytes into the correct positions
	// for an int16_t
	for (uint8_t i = 0; i < 2; i++) {
		rawvalue = (rawvalue << 8) | i2c_readNak();
	}
	// read config/status byte
	uint8_t status = i2c_readAck();
	i2c_stop();

	if (status & MCP342X_BUSY) {
		return false;
	}

	// If we're out of range, set the output to INT_MAX as a signal
	if(rawvalue == 0x7FFF) {
		value = 0x7FFFFFFF;
	}
	else if(rawvalue == -0x8000) {
		value = -0x80000000;
	}
	else {
		value = (((int32_t)rawvalue)*1000)/get_mv_divisor();
	}
	return true;
}

int32_t get_measurement_uv() {
	return value;
}