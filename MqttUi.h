#ifndef INCLUDE_SRC_MQTTUI_H
#define INCLUDE_SRC_MQTTUI_H

#include "Control.h"
#include <WiFiClient.h>
#include <PubSubClient.h>

class MqttUi
{
	private:

		Control *_control;
		WiFiClient _client_wifi;
		PubSubClient _client;
		uint32_t _connect_try_interval;
		uint32_t _last_reconnect_time;

		void _handleMqtt(char* topic, byte* payload, unsigned int length);
		void _handleSettingsChanged(Settings settings);
		bool _reconnect();

	public:
		MqttUi(Control *control, uint32_t connect_try_interval);
		void begin();
		void loop();
};

#endif
