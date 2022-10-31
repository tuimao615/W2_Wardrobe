#include "DHT11_swp.h"
#include "config.h"

void DHT11_SWP_Config(void)
{
	RCC_GPIOA_Enable();
	DHT11_SWP_Output();
	
	DHT11_H;
}

void DHT11_SWP_Output(void)
{
	GPIO_Out_PP(DHT11_SWP_GPIO_PORT, DHT11_SWP_PIN);
}
 
void DHT11_SWP_Input(void)
{
	GPIO_In_FLOATING(DHT11_SWP_GPIO_PORT, DHT11_SWP_PIN);
}
