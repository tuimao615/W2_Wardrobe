#include "mqtt_api.h"
#include "MQTTPacket.h"

#include "wifi_buffer.h"


int mqtt_connect(void)
{
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	unsigned char buf[200];
	int buflen = sizeof(buf);
	int len = 0;
	
	data.clientID.cstring = "id_3alc03lsldkfad";
	data.keepAliveInterval = 60;
	data.cleansession = 1;
	data.username.cstring = "user";
	data.password.cstring = "password";

	len = MQTTSerialize_connect(buf, buflen, &data);
	rc = WIFI_SendData(buf, len);
	
	return rc;
//	
//	if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
//	{
//		unsigned char sessionPresent, connack_rc;

//		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) == 1 && connack_rc == 0)
//		{
//			return true;
//		}
//		
//		printf("Unable to connect, return code %d\n", connack_rc);
//	}
//	
//	return false;
}

void mqtt_disconnect(void)
{
}

void mqtt_heartbeat(void)
{
	unsigned char buf[100] = {0};
	u8 len = MQTTSerialize_pingreq(buf, 100);
	WIFI_SendData((unsigned char*)buf, len);
}

void mqtt_publish(void)
{
}

void mqtt_subscribe(void)
{
}

void mqtt_unsubscribe(void)
{
}
