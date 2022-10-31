#ifndef __SENSOR_H
#define __SENSOR_H

#include "sys.h"


unsigned char get_current_temperature(void);
unsigned char get_current_humidity(void);

uint16_t get_TVOC(void);
uint16_t get_CH2O(void);
uint16_t get_CO2(void);

#endif
