#ifndef INCLUDE_SRC_SETTINGS_H
#define INCLUDE_SRC_SETTINGS_H

#include <stdint.h>

struct Settings
{
	uint32_t color1;
	uint32_t color2;
	uint32_t fade_duration;

	static Settings default_settings;
};

#endif
