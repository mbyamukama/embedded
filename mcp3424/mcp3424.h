/*
MCP3424.h 
Contiki Driver for the MCP3424 ADC to I2C converter
Original Version by Jeroen Cappaert (c)  August 2012
- gain values 0-3 represent {x1,x2,x4,x8}
- resolution 0-3 represents {12bits, 14bits, 16bits, 18bits}
User functions:
- setChannel(channelnr.) - for channel 1-4, enter a value 0-1
- getMvDivisor() - get the conversion value between the ADC value and mV
- readData() - read data from the I2C channel
- getChannelmV(channelnr.) - get data in mV directly from the I2C channel chosen by channelnr.
*/
#include "i2c.h"
#ifndef MCP3424_H
#define MCP3424_H

#define MCP3424_ADDRESS 0X69
#define MCP342X_START      0X80 // write: start a conversion
#define MCP342X_BUSY 0X80 // read: output not ready
typedef enum { false, true } bool;


//address (input) I2C bus address of the device
//gain (input) Measurement gain multiplier (1,2,4,8)
//resolution (input) ADC resolution (0,1,2,4)
void mcp3424_init_config(uint8_t address, uint8_t channel, uint8_t gain, uint8_t resolution);

// Start a one-shot channel measurement
// @param channel (input) Analog input to read (1-4)
void mcp3424_start_measure();

// Test if a measurement is ready. Use getMeasurement() to return
// the value of the result.
// @return True if measurement was ready, false otherwise
bool is_measurement_ready();

// Get the result of the last channel measurement
// @param value (output) Measured value of channel, in mV
int32_t get_measurement_uv();

// Get the divisor for converting a measurement into mV
int get_mv_divisor();


#endif
