#ifndef MQTT_API_H
#define MQTT_API_H

int mqtt_connect(void);
void mqtt_disconnect(void);

void mqtt_heartbeat(void);

void mqtt_publish(void);

void mqtt_subscribe(void);
void mqtt_unsubscribe(void);
	
#endif
