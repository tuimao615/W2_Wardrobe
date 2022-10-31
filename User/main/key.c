#include "key.h"

#define USE_GPIO_KEY
//#define USE_I2C_KEY

#ifdef USE_GPIO_KEY							//—°‘Ògpio
	#include "key_gpio.h"
#endif

#ifdef USE_I2C_KEY
	#include "key_i2c.h"
	#include "key_XW05A.h"
	
	MmiKey Key_I2C_Get_Key(void);
#endif


void Key_Init(void)
{
#ifdef USE_GPIO_KEY
	Key_GPIO_Config();						//∞¥º¸≈‰÷√gpio
#endif

#ifdef USE_I2C_KEY
	Key_I2C_Config();
#endif
}

MmiKey Key_Scan(void)
{
	static MmiKey last_key = KEY_NONE;
	MmiKey key = KEY_NONE;
	
	//GPIO
#ifdef USE_GPIO_KEY
	if(key == KEY_NONE)
		key = Key_GPIO_Get_Key();
#endif

	//I2C
#ifdef USE_I2C_KEY
	if(key == KEY_NONE)
		key = Key_I2C_Get_Key();
#endif
	
	if(key == KEY_RESET)
	{
		last_key = key;
		return KEY_RESET;
	}
	else if(last_key != key)
	{
		last_key = key;
		return key;
	}
	else
		return KEY_NONE;
}


#ifdef USE_I2C_KEY

MmiKey Key_I2C_Get_Key(void)
{
	unsigned char keys = Key_I2C_Get_Keys();
	
	if(0 == keys)
		return KEY_NONE;
	
	MmiKey mmikey = KEY_NONE;

	switch(keys)
	{
		case 0x01: mmikey = KEY_MODE_JINGHUA; break;
		case 0x02: mmikey = KEY_MODE_XIANGXUN; break;
		case 0x04: mmikey = KEY_MODE_SHAJUN; break;
		case 0x08: mmikey = KEY_MODE_HONGGAN; break;
		case 0x10: mmikey = KEY_MODE_ZIDONG; break;
		case 0x18: mmikey = KEY_ALT_CONFIG; break;
		case 0x00: mmikey = KEY_NONE; break;
		default: break;
	}
	
	return mmikey;
}

#endif
