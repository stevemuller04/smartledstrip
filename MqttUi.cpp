#include "MqttUi.h"
#include "Helper.h"

MqttUi::MqttUi(
	Control *control,
	char *hostname,
	uint16_t port,
	uint32_t connect_try_interval,
	char *client_id,
	char *username,
	char *password,
	char *topic_prefix) :
	_control(control),
	_client(_client_wifi),
	_connect_try_interval(connect_try_interval),
	_client_id(client_id),
	_username(username),
	_password(password),
	_topic_prefix(topic_prefix),
	_last_reconnect_time(0x80000000UL) // anything far from 0
{
	_client.setServer(hostname, port);
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

	Serial.printf("*mqtt: Connecting with ID '%s', username '%s', password '%s'\n", _client_id, _username, _password);
	bool success = _client.connect(_client_id, _username, _password);
	_last_reconnect_time = millis();

	if (success)
	{
		Serial.println("*mqtt: Connected");
		this->_handleSettingsChanged(_control->getSettings());
		_client.subscribe((_topic_prefix + "color1").c_str());
		_client.subscribe((_topic_prefix + "color2").c_str());
		_client.subscribe((_topic_prefix + "fade_duration").c_str());
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
		_client.publish((_topic_prefix + "COLOR1").c_str(), Helper::serializeColor(settings.color1).c_str(), true);
		_client.publish((_topic_prefix + "COLOR2").c_str(), Helper::serializeColor(settings.color2).c_str(), true);
		_client.publish((_topic_prefix + "FADE_DURATION").c_str(), Helper::serializeColor(settings.fade_duration).c_str(), true);
	}
	else
	{
		Serial.println("*mqtt: settings changed, but is not connected!");
	}
}

void MqttUi::_handleMqtt(char* topic, byte* payload, unsigned int length)
{
	Serial.printf("*mqtt: Got message at %s\n", topic);
	if (_topic_prefix + "color1" == topic)
	{
		Settings settings = _control->getSettings();
		settings.color1 = Helper::deserializeColor(payload, length);
		_control->applySettings(settings);
	}
	else if (_topic_prefix + "color2" == topic)
	{
		Settings settings = _control->getSettings();
		settings.color2 = Helper::deserializeColor(payload, length);
		_control->applySettings(settings);
	}
	else if (_topic_prefix + "fade_duration" == topic)
	{
		Settings settings = _control->getSettings();
		settings.fade_duration = Helper::deserializeInt(payload, length);
		_control->applySettings(settings);
	}
}
