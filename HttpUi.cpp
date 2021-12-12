#include "HttpUi.h"

const char HTTPUI_HTML[] PROGMEM =
#include "HttpUi.html.h"
;

HttpUi::HttpUi(Settings *settings, AnimationQueue *animationQueue, uint32_t animation_fastfade_duration) :
	_settings(settings),
	_animationQueue(animationQueue),
	_server(80),
	_animation_fastfade_duration(animation_fastfade_duration)
{
}

void HttpUi::begin()
{
	_server.on("/", HTTP_GET, [this] () { this->_handleRoot(); });
	_server.on("/api/config", HTTP_GET, [this] () { this->_handleApiGetConfig(); });
	_server.on("/api/config", HTTP_POST, [this] () { this->_handleApiSetConfig(); });
	_server.begin();
}

void HttpUi::loop()
{
	_server.handleClient();
}

void HttpUi::_handleRoot()
{
	_server.send_P(200, "text/html", HTTPUI_HTML);
}

void HttpUi::_handleApiGetConfig()
{
	String color1 = _colorToString(_settings->color1);
	String color2 = _colorToString(_settings->color2);
	String fade_duration = String(_settings->fade_duration, DEC);
	_server.send(200, "text/json", "{\"color1\":\"#" + color1 + "\",\"color2\":\"#" + color2 + "\",\"fade_duration\":" + fade_duration + "}");
}

void HttpUi::_handleApiSetConfig()
{
	bool has_changes = false;

	if (_server.hasArg("color1"))
	{
		_settings->color1 = _parseColor(_server.arg("color1"));
		has_changes = true;
	}

	if (_server.hasArg("color2"))
	{
		_settings->color2 = _parseColor(_server.arg("color2"));
		has_changes = true;
	}

	if (_server.hasArg("fade_duration"))
	{
		_settings->fade_duration = _parseInt(_server.arg("fade_duration"));
		has_changes = true;
	}

	if (has_changes)
	{
		_settings->save();
		_animationQueue->abort();
		_animationQueue->addAnimation(_settings->color1, _animation_fastfade_duration, false);
		_animationQueue->addAnimation(_settings->color2, _settings->fade_duration, true);
		_animationQueue->addAnimation(_settings->color1, _settings->fade_duration, true);
	}

	_server.send(200, "text/json", "{}");
}

uint32_t HttpUi::_parseColor(String s)
{
	s.replace("#", "");
	return strtoul(s.c_str(), NULL, 16);
}

uint32_t HttpUi::_parseInt(String s)
{
	return strtoul(s.c_str(), NULL, 10);
}

String HttpUi::_colorToString(uint32_t color)
{
	String s = "000000000" + String(color, HEX);
	return s.substring(s.length() - 6);
}

