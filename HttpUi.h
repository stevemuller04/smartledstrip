#ifndef INCLUDE_SRC_HTTPUI_H
#define INCLUDE_SRC_HTTPUI_H

#include "Control.h"
#include <ESP8266WebServer.h>

class HttpUi
{
	private:

		Control *_control;
		ESP8266WebServer _server;

		void _handleRoot();
		void _handleApiSetConfig();
		void _handleApiGetConfig();
		static uint32_t _parseColor(String s);
		static uint32_t _parseInt(String s);
		static String _colorToString(uint32_t color);

	public:

		HttpUi(Control *control);
		void begin();
		void loop();
};

#endif
