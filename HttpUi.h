#ifndef INCLUDE_SRC_HTTPUI_H
#define INCLUDE_SRC_HTTPUI_H

#include "Settings.h"
#include "AnimationQueue.h"
#include <ESP8266WebServer.h>

class HttpUi
{
	private:

		Settings *_settings;
		AnimationQueue *_animationQueue;
		ESP8266WebServer _server;
		uint32_t _animation_fastfade_duration;

		void _handleRoot();
		void _handleApiSetConfig();
		void _handleApiGetConfig();
		static uint32_t _parseColor(String s);
		static uint32_t _parseInt(String s);
		static String _colorToString(uint32_t color);

	public:

		HttpUi(Settings *settings, AnimationQueue *animationQueue, uint32_t animation_fastfade_duration);
		void begin();
		void loop();
};

#endif
