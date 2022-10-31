#include "buzzer.h"
#include "device_gpio.h"


#define END 0xff
 
u8 B_Sound_Enable;
u8 on_time,off_time;
u8 Index;
u8 Sound_Page;
 
uc8 Sound_Di[]={1,1,1,1,END};								 
uc8 Sound_Di_Di[]={1,2,1,2,END}; 
uc8 Sound_Di_Di_Di[]={1,3,1,3,END};
uc8 Sound_Di_Di_Di_Di_Di[]={1,4,1,4,END};
uc8 Sound_Di_2s[]={20,5,END};
uc8 Sound_Di_5s[]={50,5,END};
 
uc8 *const SoundMenu[]={Sound_Di,
						Sound_Di_Di,
					  Sound_Di_Di_Di,
					  Sound_Di_Di_Di_Di_Di,
						Sound_Di_2s,
						Sound_Di_5s};
 
void Buzzer_Start(u8 I)
{
	Sound_Page=I;
	Index=0;
	on_time=0;
	off_time=0;
	B_Sound_Enable=1;
}
 

void Buzzer_Output(void)
{
  if(B_Sound_Enable)
	{
		if(on_time)
		{
			on_time--;
			BUZ_ON();
		}
		else if(off_time)
		{
			off_time--;
			BUZ_OFF();
		}
		else
		{
			if(SoundMenu[Sound_Page][Index]==END)
			{
				BUZ_OFF();
				B_Sound_Enable=0;
			}
			else
			{
				on_time=SoundMenu[Sound_Page][Index];
				off_time=SoundMenu[Sound_Page][Index+1];
				Index+=2;
			}
		}
  }
}
