
#include "delay.h"
#include "timer.h"

#include "jw01_usart.h"

#include "config.h"
#include "wifi_usart.h"
//Hanfeng
#include "wifi_LPC330.h"
#include "wifi_buffer.h"
//Tuya
#include "wifi.h"
#include "mode.h"
#include "usart.h"
#include "sensor_DHT20.h"
#include "sensor_JW01.h"

//#include "bt_buffer.h"
//#include "bt_command.h"

//#include "mqtt_api.h"


extern void Device_Init(void);
extern void Sensor_DHT11_Init(void);
extern void Sensor_DHT20_Init(void);
extern void Key_Init(void);
extern void Display_Init(void);
extern void Sensor_JW01_Init(void);

u8 g_init_status = 0;

int g_wifi_status = 0;
int g_network_status = 0;
int g_mqtt_status = 0;


void sytem_init()
{
    delay_init();			//初始化延迟函数
    Timer_Main_Config();	//初始化定时器3
	//Timer_CSB_Config();
		Timer_BUZ_Config();

    Device_Init();			//设备参数初始化
//	Sensor_DHT11_Init();
    Sensor_DHT20_Init();
    Sensor_JW01_Init();
    Key_Init();
//	Display_Init();

    WIFI_Init();
//	BT_USART_Init();
}


// 主函数入口
int main(void)
{
    sytem_init();

    JW01_HW_Poweron();
    WIFI_HW_Poweron();

#ifdef USE_TUYA_MODULE
    wifi_protocol_init();
#endif

    //延迟打开中断，解决串口2不通问题？
    delay_ms(1000);

    JW01_USART_Enable_Rx();
    WIFI_USART_Enable_Rx();


#ifdef USE_HF_LPC330
    //WIFI_HW_Reload_FactorySet();
    //WIFI_HW_SmartBleLink();
    WIFI_SW_SmartBleLink();
    //WIFI_Setup_Network();
#endif

    uart_init(9600);
    g_init_status = 1;
    while(1)
    {

#ifdef USE_TUYA_MODULE
        wifi_uart_service();
#endif
//		printf("\n");
//        printf("温度：%d\n",DHT20_get_temperature());
//        printf("湿度：%d\n",DHT20_get_humidity());
//        delay_ms(1000);
//        printf("TVOC：%d\n",JW01_get_TVOC());
//        delay_ms(1000);
//        printf("CO2：%d\n",JW01_get_CO2());
//        delay_ms(1000);
//        printf("CH2O：%d\n",JW01_get_CH2O());
//        delay_ms(1000);
		delay_ms(100);


    }
}
