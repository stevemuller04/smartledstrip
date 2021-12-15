#ifndef INCLUDE_SRC_CONTROL_H
#define INCLUDE_SRC_CONTROL_H

#include "Settings.h"
#include "AnimationQueue.h"
#include <string.h> // required by <ESP_EEPROM.h>
#include <ESP_EEPROM.h>
#include <functional>

using SettingsChangedHandler = std::function<void(Settings)>;

class Control
{
	private:

		Settings _settings;
		AnimationQueue *_animation_queue;
		uint32_t _animation_fastfade_duration;
		LinkedList<SettingsChangedHandler> _handlers;

		void loadSettings();
		void saveSettings();

	public:

		Control(AnimationQueue *animation_queue, uint32_t animation_fastfade_duration);
		void addSettingsChangedHandler(SettingsChangedHandler handler);
		void applySettings(Settings settings);
		Settings getSettings();

		void begin();
};

#endif
