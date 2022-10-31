#include "type_def.h"
#include "event.h"

void MQTT_Parse_Message(const char* msg)
{
	MmiEvent event = EVENT_NONE;
	
	if(strcmp(msg, "on") == 0)
		event = EVENT_MODE_JINGHUA;
}


void MQTT_Send_Message(MmiEvent event)
{
	
}
