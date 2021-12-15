#include "HttpUi.h"
#include "Helper.h"

const char HTTPUI_HTML[] PROGMEM =
#include "HttpUi.html.h"
;

HttpUi::HttpUi(Control *control) :
	_control(control),
	_server(80)
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
	Settings settings = _control->getSettings();
	String color1 = Helper::serializeColor(settings.color1);
	String color2 = Helper::serializeColor(settings.color2);
	String fade_duration = Helper::serializeInt(settings.fade_duration);
	_server.send(200, "text/json", "{\"color1\":\"" + color1 + "\",\"color2\":\"" + color2 + "\",\"fade_duration\":" + fade_duration + "}");
}

void HttpUi::_handleApiSetConfig()
{
	bool has_changes = false;
	Settings settings = _control->getSettings();

	if (_server.hasArg("color1"))
	{
		settings.color1 = Helper::deserializeColor(_server.arg("color1"));
		has_changes = true;
	}

	if (_server.hasArg("color2"))
	{
		settings.color2 = Helper::deserializeColor(_server.arg("color2"));
		has_changes = true;
	}

	if (_server.hasArg("fade_duration"))
	{
		settings.fade_duration = Helper::deserializeInt(_server.arg("fade_duration"));
		has_changes = true;
	}

	if (has_changes)
	{
		_control->applySettings(settings);
	}

	_server.send(200, "text/json", "{}");
}
