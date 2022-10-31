#include "device_gpio.h"
#include "config.h"

void Device_GPIO_Config(void)
{
	//ʹ��ʱ��	
	RCC_GPIOA_Enable();
	RCC_GPIOB_Enable();

//	GPIO_Out_PP(LIGHT_GPIO, LIGHT_GPIO_PIN);//�ƴ�
	GPIO_Out_PP(HEAT_GPIO, HEATONE_GPIO_PIN);//����˿1
	GPIO_Out_PP(HEAT_GPIO, HEATTWO_GPIO_PIN);//����˿2
	GPIO_Out_PP(FAN_GPIO, FAN_GPIO_PIN);//����
	GPIO_Out_PP(FLZ_GPIO, FLZ_GPIO_PIN);//������
	GPIO_Out_PP(CY_GPIO, CY_GPIO_PIN);//����
	GPIO_Out_PP(XX_GPIO, XX_GPIO_PIN);//��޹
	GPIO_Out_PP(UV_GPIO, UV_GPIO_PIN);//������
	GPIO_Out_PP(UV_LED_GPIO, UV_LED_GPIO_PIN);//������
	GPIO_Out_PP(BUZ_GPIO, BUZ_GPIO_PIN);//������
	GPIO_In_FLOATING(PIR_GPIO, PIR_GPIO_PIN);//�����Ӧ
	
	UV_LED_OFF();
}
