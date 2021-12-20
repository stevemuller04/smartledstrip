#include "MqttUi.h"
#include "Helper.h"
#include "config.h"

MqttUi::MqttUi(Control *control, uint32_t connect_try_interval) :
	_control(control),
	_client(_client_wifi),
	_connect_try_interval(connect_try_interval),
	_last_reconnect_time(0x80000000UL) // anything far from 0
{
	_client.setServer(MQTT_HOSTNAME, MQTT_PORT);
	_client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->_handleMqtt(topic, payload, length); });
	_control->addSettingsChangedHandler([this] (Settings s) { this->_handleSettingsChanged(s); });
}

void MqttUi::begin()
{
}

void MqttUi::loop()
{
	if (_client.connected() || _reconnect())
		_client.loop();
}

bool MqttUi::_reconnect()
{
	if (millis() - _last_reconnect_time < _connect_try_interval)
		return false;

	Serial.printf("*mqtt: Connecting with ID '%s', username '%s', password '%s'\n", MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
	bool success = _client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
	_last_reconnect_time = millis();

	if (success)
	{
		Serial.println("*mqtt: Connected");
		this->_handleSettingsChanged(_control->getSettings());
		_client.subscribe(MQTT_TOPIC_SUB_COLOR1);
		_client.subscribe(MQTT_TOPIC_SUB_COLOR2);
		_client.subscribe(MQTT_TOPIC_SUB_FADE_DURATION);
		return true;
	}
	else
	{
		Serial.println("*mqtt: Connection failed!");
		return false;
	}
}

void MqttUi::_handleSettingsChanged(Settings s)
{
	if (_client.connected())
	{
		Serial.println("*mqtt: publishing changed settings");
		Settings settings = _control->getSettings();
		_client.publish(MQTT_TOPIC_PUB_COLOR1, Helper::serializeColor(settings.color1).c_str(), true);
		_client.publish(MQTT_TOPIC_PUB_COLOR2, Helper::serializeColor(settings.color2).c_str(), true);
		_client.publish(MQTT_TOPIC_PUB_FADE_DURATION, Helper::serializeColor(settings.fade_duration).c_str(), true);
	}
	else
	{
		Serial.println("*mqtt: settings changed, but is not connected!");
	}
}

void MqttUi::_handleMqtt(char* topic, byte* payload, unsigned int length)
{
	Serial.printf("*mqtt: Got message at %s\n", topic);
	if (strcmp(MQTT_TOPIC_SUB_COLOR1, topic) == 0)
	{
		Settings settings = _control->getSettings();
		settings.color1 = Helper::deserializeColor(payload, length);
		_control->applySettings(settings);
	}
	else if (strcmp(MQTT_TOPIC_SUB_COLOR2, topic) == 0)
	{
		Settings settings = _control->getSettings();
		settings.color2 = Helper::deserializeColor(payload, length);
		_control->applySettings(settings);
	}
	else if (strcmp(MQTT_TOPIC_SUB_FADE_DURATION, topic) == 0)
	{
		Settings settings = _control->getSettings();
		settings.fade_duration = Helper::deserializeInt(payload, length);
		_control->applySettings(settings);
	}
}
