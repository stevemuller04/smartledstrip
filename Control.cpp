#include "Control.h"

Control::Control(AnimationQueue *animation_queue, uint32_t animation_fastfade_duration) :
	_animation_queue(animation_queue),
	_animation_fastfade_duration(animation_fastfade_duration)
{
}

void Control::begin()
{
	EEPROM.begin(12);
	loadSettings();
}

void Control::loadSettings()
{
	EEPROM.get(0, _settings.color1);
	EEPROM.get(4, _settings.color2);
	EEPROM.get(8, _settings.fade_duration);
}

void Control::saveSettings()
{
	EEPROM.put(0, _settings.color1);
	EEPROM.put(4, _settings.color2);
	EEPROM.put(8, _settings.fade_duration);
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
