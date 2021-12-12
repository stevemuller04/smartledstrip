#include "Settings.h"

void Settings::begin()
{
	EEPROM.begin(12);
	load();
}

void Settings::load()
{
	EEPROM.get(0, color1);
	EEPROM.get(4, color2);
	EEPROM.get(8, fade_duration);
}

void Settings::save()
{
	EEPROM.put(0, color1);
	EEPROM.put(4, color2);
	EEPROM.put(8, fade_duration);
	EEPROM.commit();
}
