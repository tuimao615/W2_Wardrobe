#include "sensor.h"

//#include "sensor_DHT11.h"
#include "sensor_DHT20.h"
#include "sensor_JW01.h"


unsigned char get_current_temperature(void)
{
	//return DHT11_get_temperature();
	return DHT20_get_temperature();
}

unsigned char get_current_humidity(void)
{
	//return DHT11_get_humidity();
	return DHT20_get_humidity();
}

uint16_t get_TVOC(void)
{
	return JW01_get_TVOC();
}

uint16_t get_CH2O(void)
{
	return JW01_get_CH2O();
}

uint16_t get_CO2(void)
{
	return JW01_get_CO2();
}
