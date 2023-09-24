#include "Control.h"
#include <Preferences.h>

Control::Control(AnimationQueue *animation_queue, uint32_t animation_fastfade_duration) :
	_animation_queue(animation_queue),
	_animation_fastfade_duration(animation_fastfade_duration)
{
}

void Control::begin()
{
	loadSettings();
}

void Control::loadSettings()
{
	Preferences pref;
	pref.begin("main", true);

	_settings.color1 = pref.getUInt("col1", Settings::default_settings.color1);
	_settings.color2 = pref.getUInt("col2", Settings::default_settings.color2);
	_settings.fade_duration = pref.getUInt("fdur", Settings::default_settings.fade_duration);
	_settings.power = pref.getBool("power", Settings::default_settings.power);

	pref.end();
}

void Control::saveSettings()
{
	Preferences pref;
	pref.begin("main", false);

	pref.putUInt("col1", _settings.color1);
	pref.putUInt("col2", _settings.color2);
	pref.putUInt("fdur", _settings.fade_duration);
	pref.putBool("power", _settings.power);

	pref.end();
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
	if (settings.power)
	{
		_animation_queue->addAnimation(settings.color1, _animation_fastfade_duration, false);
		_animation_queue->addAnimation(settings.color2, settings.fade_duration, true);
		_animation_queue->addAnimation(settings.color1, settings.fade_duration, true);
	}
	else
	{
		_animation_queue->addAnimation(0, _animation_fastfade_duration, false);
	}

	for (int i = 0; i < _handlers.size(); ++i)
		_handlers.get(i)(old_settings, settings);
}

Settings Control::getSettings()
{
	return _settings;
}
