#include "mode.h"
#include "device.h"
#include "buzzer.h"
#include "main_timer.h"
#include "mcu_api.h"
#include "protocol.h"

#define TIME_END 0xff

//全局变量避免
static /*volatile*/ WorkMode g_mode = WORK_MODE_ZIDONG;
static /*volatile*/ WorkType g_type = AUTO_CONTROL;
extern int heat_flag;
extern int heat_time;
extern void all_data_update(void);
WorkMode get_work_mode(void)
{
    return g_mode;
}

static void set_work_mode(WorkMode mode)
{
    g_mode = mode;
}
WorkType get_work_type(void)
{
    return g_type;
}

void set_work_type(WorkType type)
{
    g_type = type;
}


u8 jinghua_on_time, jinghua_off_time, jinghua_index;
u8 xiangxun_on_time, xiangxun_off_time, xiangxun_index;
u8 shajun_on_time, shajun_off_time, shajun_index;
u8 honggan_on_time, honggan_off_time, honggan_index;


uc8 jinghua_minutes[]= {5,0,TIME_END}; //净化5分钟
uc8 xiangxun_minutes[]= {10,0,TIME_END}; //香薰10分钟
uc8 shajun_minutes[]= {5,0,TIME_END}; //杀菌5分钟
uc8 honggan_minutes[]= {15,0,TIME_END}; //烘干15分钟


/*Jinghua*/

static void jinghua_on()
{
    FAN_turn(1);
	FLZ_turn(1);
    CY_turn(1);
}

static void jinghua_off()
{
    FAN_turn(0);
	FLZ_turn(0);
    CY_turn(0);
}

void enter_jinhua_mode(void)			//启动净化模式
{
    ALL_turn_off();
    jinghua_on();

    jinghua_on_time = 0;
    jinghua_off_time = 0;
    jinghua_index = 0;

    set_work_mode(WORK_MODE_JINGHUA);
	mcu_dp_enum_update(DPID_MODE,get_work_mode());
    Buzzer_Start(1);
}

void run_jinghua_mode()
{
    if(jinghua_on_time)
    {
        jinghua_on_time--;
        jinghua_on();
    }
    else if(jinghua_off_time)
    {
        jinghua_off_time--;
        jinghua_off();
    }
    else
    {
        u8 on_time = jinghua_minutes[jinghua_index];
        u8 off_time = jinghua_minutes[jinghua_index+1];

        if(on_time==TIME_END || off_time==TIME_END)
        {
            jinghua_off();
            enter_zidong_mode();
        }
        else
        {
            jinghua_on_time = on_time;
            jinghua_off_time = off_time;
            jinghua_index+=2;
        }
    }
}

/*Xiangxun*/

static void xiangxun_on()
{
    XX_turn(1);
}

static void xiangxun_off()
{
    XX_turn(0);
}

void enter_xiangxun_mode(void)			//启动香薰模式
{
    ALL_turn_off();
    xiangxun_on();

    xiangxun_on_time = 0;
    xiangxun_off_time = 0;
    xiangxun_index = 0;

    set_work_mode(WORK_MODE_XIANGXUN);
	mcu_dp_enum_update(DPID_MODE,get_work_mode());
    Buzzer_Start(1);
}

void run_xiangxun_mode()
{
    if(xiangxun_on_time)
    {
        xiangxun_on_time--;
        xiangxun_on();
    }
    else if(xiangxun_off_time)
    {
        xiangxun_off_time--;
        xiangxun_off();
    }
    else
    {
        u8 on_time = xiangxun_minutes[xiangxun_index];
        u8 off_time = xiangxun_minutes[xiangxun_index+1];

        if(on_time==TIME_END || off_time==TIME_END)
        {
            xiangxun_off();
            enter_zidong_mode();
        }
        else
        {
            xiangxun_on_time = on_time;
            xiangxun_off_time = off_time;
            xiangxun_index+=2;
        }
    }
}


/*Shajun*/
static void shajun_on()
{
    UV_turn(1);
}

static void shajun_off()
{
    UV_turn(0);
}

void enter_shajun_mode(void)			//启动杀菌模式
{
    ALL_turn_off();
    shajun_on();

    shajun_on_time = 0;
    shajun_off_time = 0;
    shajun_index = 0;

    set_work_mode(WORK_MODE_SHAJUN);
	mcu_dp_enum_update(DPID_MODE,get_work_mode());
    Buzzer_Start(1);
}

void run_shajun_mode()
{
    if(shajun_on_time)
    {
        shajun_on_time--;
        shajun_on();
    }
    else if(shajun_off_time)
    {
        shajun_off_time--;
        shajun_off();
    }
    else
    {
        u8 on_time = shajun_minutes[shajun_index];
        u8 off_time = shajun_minutes[shajun_index+1];

        if(on_time==TIME_END || off_time==TIME_END)
        {
            shajun_off();
            enter_zidong_mode();
        }
        else
        {
            shajun_on_time = on_time;
            shajun_off_time = off_time;
            shajun_index+=2;
        }
    }
}


/*Honggan*/

static void honggan_on()
{
    FAN_turn(1);
    HEAT_turn(1);
}

static void honggan_off()
{
    FAN_turn(0);
    HEAT_turn(0);
}

void enter_honggan_mode(void)				//启动烘干模式
{
    ALL_turn_off();
    honggan_on();

    honggan_on_time = 0;
    honggan_off_time = 0;
    honggan_index = 0;

    set_work_mode(WORK_MODE_HONGGAN);
	mcu_dp_enum_update(DPID_MODE,get_work_mode());
    Buzzer_Start(1);
}

void run_honggan_mode()
{
    if(honggan_on_time)
    {
        honggan_on_time--;
        //honggan_on();					//不能取消注释，不然会影响加热块分时加热
    }
    else if(honggan_off_time)
    {
        honggan_off_time--;
        honggan_off();
    }
    else
    {
        u8 on_time = honggan_minutes[honggan_index];
        u8 off_time = honggan_minutes[honggan_index+1];

        if(on_time==TIME_END || off_time==TIME_END)
        {
            honggan_off();
            heat_flag = 1;
            heat_time = HEAT_TIME;
            enter_zidong_mode();
        }
        else
        {
            honggan_on_time = on_time;
            honggan_off_time = off_time;
            honggan_index+=2;
        }
    }
}


/*Zidong*/

u8 zidong_off_time;

static void zidong_off()
{
    ALL_turn_off();
}

void enter_zidong_mode(void)			//启动自动模式
{
    ALL_turn_off();
    zidong_off_time = 0;
    set_work_type(AUTO_CONTROL);
    set_work_mode(WORK_MODE_ZIDONG);
	mcu_dp_enum_update(DPID_MODE,get_work_mode());
    Buzzer_Start(1);
}


void run_zidong_mode()
{
    if(zidong_off_time)					//非外力自动模式不改变一直保持运行
    {
        zidong_off_time--;
        zidong_off();
        return;
    }
}

/*Chunjing*/

u8 chunjing_off_time;

static void chunjing_off()
{
	ALL_turn_off();
}

void enter_chunjing_mode(void)
{
	ALL_turn_off();
	
	chunjing_off_time = 0;
	set_work_mode(WORK_MODE_CHUNJING);	
	Buzzer_Start(1);
}

void run_chunjing_mode()
{
	if(chunjing_off_time)
	{
		chunjing_off_time--;
		chunjing_off();
		return;
	}
}

