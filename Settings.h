#ifndef INCLUDE_SRC_SETTINGS_H
#define INCLUDE_SRC_SETTINGS_H

#include <string.h> // required by <ESP_EEPROM.h>
#include <ESP_EEPROM.h>

class Settings
{
	public:

		uint32_t color1;
		uint32_t color2;
		uint32_t fade_duration;

		void begin();
		void load();
		void save();
};

#endif