#include "seg_HT1621.h"
#include "seg_gpio.h"

#include "delay.h"


#define COMMAND_MODE 		0
#define DATA_MODE				1

#define BUFFER_SIZE			10

unsigned char LCD_RAM[BUFFER_SIZE]= 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


static void write(unsigned char bit)
{
	WR_L;
	//delay_us(10);
	
	if(bit == 0)
		DATA_0;
	else
		DATA_1;	
	delay_us(10);
	
	WR_H;
	//delay_us(10);
}

static void write_mode(unsigned char mode)
{
	write(1);
	write(0);
	if(mode == 0)
		write(0);
	else
		write(1);
}

static void write_command(unsigned char command)
{
	unsigned char i = 0;
	for (i = 0; i < 8; i++)
		write((command >> (7 - i)) & 0x01);
	write(0);
}

static void write_address(unsigned char address)
{
	unsigned char i = 0;
	for (i = 0; i < 6; i++)
		write((address >> (5 - i)) & 0x01);
}

static void write_data_8bit(unsigned char bit8)
{
	unsigned char i = 0;
	for (i = 0; i < 8; i++)
		write((bit8 >> i) & 0x01);
}

static void write_data_4bit(unsigned char bit4)
{
	unsigned char i = 0;
	for (i = 0; i < 4; i++)
	{
		unsigned char j = (bit4 >> i);
		//unsigned char jj = j & 0x01;
		write(j & 0x01);	
	}
}


static void SEG_HT1621_Init(void)
{
 	CS_H;
	RD_H;
	WR_H;
	DATA_1;
	delay_ms(500);//100?
	
	CS_L;
	delay_us(10);
	
	write_mode(COMMAND_MODE);
	write_command(0x01);    //Enable System
	write_command(0x03);    //Enable Bias
	write_command(0x04);    //Disable Timer
	write_command(0x05);    //Disable WDT
	write_command(0x08);    //Tone OFF
	write_command(0x18);    //on-chip RC
	write_command(0x29);    //1/4Duty 1/3Bias	//关闭导致COM4无效？
	write_command(0x40);    //Tone Frequency 4kHZ
	write_command(0x80);    //Disable IRQ
	write_command(0xE3);    //Normal Mode
	
	CS_H;
}

static void SEG_HT1621_set_all(u8 data)
{
	unsigned char i;

	CS_L;
	delay_us(10);
	
	write_mode(DATA_MODE);
	write_address(0);
	for(i = 0; i<BUFFER_SIZE; i++)
		write_data_4bit(data);
	
	CS_H;
}

static void SEG_HT1621_update(void)
{
	unsigned char i;

	CS_L;
	delay_us(10);
	
	write_mode(DATA_MODE);
	write_address(0);
	for(i = 0; i<BUFFER_SIZE; i++)
		write_data_4bit(LCD_RAM[i]);
	
	CS_H;
}


static unsigned char get_byte_1(u8 num)
{
	unsigned char byte = 0x00;
	switch(num)
	{
		case 0: byte=0x0F; break;
		case 1: byte=0x00; break;
		case 2: byte=0x0D; break;
		case 3: byte=0x09; break;
		case 4: byte=0x02; break;
		case 5: byte=0x0B; break;
		case 6: byte=0x0F; break;
		case 7: byte=0x01; break;
		case 8: byte=0x0F; break;
		case 9: byte=0x0B; break;
	}
	return byte;
}

static unsigned char get_byte_2(u8 num)
{
	unsigned char byte = 0x00;
	switch(num)
	{
		case 0: byte=0x0A; break;
		case 1: byte=0x0A; break;
		case 2: byte=0x07; break;
		case 3: byte=0x0E; break;
		case 4: byte=0x0E; break;
		case 5: byte=0x0C; break;
		case 6: byte=0x0C; break;
		case 7: byte=0x0A; break;
		case 8: byte=0x0E; break;
		case 9: byte=0x0E; break;
	}
	return byte;
}

/*
	Init
*/

void Lcd_Init(void)
{
	SEG_Config();
	SEG_BL_Config();
	
	SEG_HT1621_Init();
	
	Lcd_clear_all();
	Lcd_Backlight_On();
}

//Backlight

void Lcd_Backlight_On(void)
{
	SEG_BL_ON();
}

void Lcd_Backlight_Off(void)
{
	SEG_BL_OFF();
}

void Lcd_set_brightness(u8 percent)
{
	static u8 current = 16;
	u8 level = (percent-1)*16/100 + 1;
	u8 steps = 0;
	
	if(current == level)
		return;
	
	if(current > level)
		steps = current-level;
	else
		steps = current+16-level;
	
	current = level;
	
	u8 i;
	for(i=0; i<steps; i++)
	{
		SEG_BL_OFF();//Down
		delay_us(1);
		SEG_BL_ON(); //On
		delay_us(1);
	}
}

void Lcd_show_all(void)
{
	SEG_HT1621_set_all(0x0F);
}

void Lcd_clear_all(void)
{
	SEG_HT1621_set_all(0x00);
}

void Lcd_update(void)
{
	SEG_HT1621_update();
}

void Lcd_show_num1(u8 num)
{
	unsigned char b1 = get_byte_1(num);
	unsigned char b2 = get_byte_2(num);
	
	LCD_RAM[1] = b1; 
	LCD_RAM[2] = b2 | (LCD_RAM[2] & 0x01); 
}

void Lcd_show_num2(u8 num)
{
	unsigned char b1 = get_byte_1(num);
	unsigned char b2 = get_byte_2(num);
	
	LCD_RAM[3] = b1; 
	LCD_RAM[4] = b2 | (LCD_RAM[4] & 0x01); 
}

void Lcd_show_num3(u8 num)
{
	unsigned char b1 = get_byte_1(num);
	unsigned char b2 = get_byte_2(num);
	
	LCD_RAM[5] = b1; 
	LCD_RAM[6] = b2 | (LCD_RAM[6] & 0x01); 
}

void Lcd_show_num4(u8 num)
{
	unsigned char b1 = get_byte_1(num);
	unsigned char b2 = get_byte_2(num);
	
	LCD_RAM[7] = b1; 
	LCD_RAM[8] = b2 | (LCD_RAM[8] & 0x01); 
}

void Lcd_Dot_turn(u8 on)
{
	if(on)
		LCD_RAM[4] |= 0x01;
	else
		LCD_RAM[4] &= 0xFE;
}

void Lcd_Temperature_turn(u8 on)
{
	if(on)
		LCD_RAM[6] |= 0x01;
	else
		LCD_RAM[6] &= 0xFE;
}

void Lcd_Humidity_turn(u8 on)
{
	if(on)
		LCD_RAM[8] |= 0x01;
	else
		LCD_RAM[8] &= 0xFE;
}


void Lcd_BT_turn(u8 on)
{
	if(on)
		LCD_RAM[0] |= 0x02;
	else
		LCD_RAM[0] &= 0xFD;
}

void Lcd_Wifi_turn(u8 on)
{
	if(on)
		LCD_RAM[0] |= 0x01;
	else
		LCD_RAM[0] &= 0xFE;
}

void Lcd_Clock_turn(u8 on)
{
	if(on)
		LCD_RAM[0] |= 0x04;
	else
		LCD_RAM[0] &= 0xFB;
}

void Lcd_Alarm_turn(u8 on)
{
	if(on)
		LCD_RAM[0] |= 0x08; 
	else
		LCD_RAM[0] &= 0xF7; 
}


//净化
void Lcd_Jinghua_turn(u8 on)
{
	if(on)
		LCD_RAM[9] |= 0x08; 
	else
		LCD_RAM[9] &= 0xF7; 
}

//香薰
void Lcd_Xiangxun_turn(u8 on)
{
	if(on)
		LCD_RAM[9] |= 0x04; 
	else
		LCD_RAM[9] &= 0xFB; 
}

//杀菌
void Lcd_Shajun_turn(u8 on)
{
	if(on)
		LCD_RAM[9] |= 0x02; 
	else
		LCD_RAM[9] &= 0xFD; 
}

//烘干
void Lcd_Honggan_turn(u8 on)
{
	if(on)
		LCD_RAM[9] |= 0x01; 
	else
		LCD_RAM[9] &= 0xFE; 
}

//自动
void Lcd_Zidong_turn(u8 on)
{
	if(on)
		LCD_RAM[2] |= 0x01; 
	else
		LCD_RAM[2] &= 0xFE; 
}
