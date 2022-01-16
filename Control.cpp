#include "Control.h"

Control::Control(AnimationQueue *animation_queue, uint32_t animation_fastfade_duration) :
	_animation_queue(animation_queue),
	_animation_fastfade_duration(animation_fastfade_duration)
{
}

void Control::begin()
{
	EEPROM.begin(1 + sizeof(Settings));
	loadSettings();
}

void Control::loadSettings()
{
	uint8_t version = 0xFF;
	EEPROM.get(0, version);

	switch (version)
	{
		case 1:
			EEPROM.get(1, _settings.color1);
			EEPROM.get(5, _settings.color2);
			EEPROM.get(9, _settings.fade_duration);
			EEPROM.get(13, _settings.power);
			break;
		default:
			_settings = Settings::default_settings;
			break;
	}
}

void Control::saveSettings()
{
	EEPROM.put(0, 0x01); // version
	EEPROM.put(1, _settings.color1);
	EEPROM.put(5, _settings.color2);
	EEPROM.put(9, _settings.fade_duration);
	EEPROM.put(13, _settings.power);
	EEPROM.commit();
}

void Control::addSettingsChangedHandler(SettingsChangedHandler handler)
{
	_handlers.add(handler);
}

void Control::applySettings(Settings settings)
{
	Settings old_settings = settings;
	_settings = settings;
	saveSettings();

	_animation_queue->abort();
	_animation_queue->addAnimation(settings.color1, _animation_fastfade_duration, false);
	_animation_queue->addAnimation(settings.color2, settings.fade_duration, true);
	_animation_queue->addAnimation(settings.color1, settings.fade_duration, true);

	for (int i = 0; i < _handlers.size(); ++i)
		_handlers.get(i)(old_settings, settings);
}

Settings Control::getSettings()
{
	return _settings;
}
