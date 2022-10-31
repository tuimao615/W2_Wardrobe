#ifndef __SENSOR_DHT11_H
#define __SENSOR_DHT11_H

#include "sys.h"


void Sensor_DHT11_Init(void);

void DHT11_update(void);
unsigned char DHT11_get_temperature(void);
unsigned char DHT11_get_humidity(void);

#endif
