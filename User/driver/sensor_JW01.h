#ifndef __SENSOR_JW01_H
#define __SENSOR_JW01_H

#include "sys.h"


void Sensor_JW01_Init(void);

void JW01_update(void);

uint16_t JW01_get_TVOC(void);
uint16_t JW01_get_CH2O(void);
uint16_t JW01_get_CO2(void);

#endif
