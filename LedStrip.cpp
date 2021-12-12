#include "LedStrip.h"

LedStrip::LedStrip(uint16_t n, int16_t pin, neoPixelType type) :
	_pixels(n, pin, type),
	_pixels_num(n),
	_pixels_last_color_set(0),
	_color_initial(0),
	_color_target(0),
	_fade_start_millis(0),
	_fade_duration_millis(0),
	_fade_done(true)
{
}

void LedStrip::begin()
{
	_pixels.begin();
	_pixels.clear();
	_pixels.show();
}

void LedStrip::loop()
{
	if (!_fade_done)
	{
		float progress = _computeFadeProgress();
		if (progress >= 1.0f)
		{
			_fade_done = true;
		}

		uint32_t c = _computeCurrentColor(progress);
		if (c != _pixels_last_color_set)
		{
			_pixels_last_color_set = c;
			_pixels.fill(c, 0, _pixels_num);
			_pixels.show();
		}
	}
}

void LedStrip::fadeToColor(uint32_t color, uint32_t duration_millis)
{
	_color_initial = _computeCurrentColor(_computeFadeProgress());
	_color_target = color;
	_fade_start_millis = millis();
	_fade_duration_millis = duration_millis;
	_fade_done = false;
}

bool LedStrip::isFading()
{
	return !_fade_done;
}

void LedStrip::abort()
{
	_fade_done = true;
}

float LedStrip::_computeFadeProgress()
{
	uint32_t dt = millis() - _fade_start_millis;

	if (dt >= _fade_duration_millis)
		return 1.0f;

	float dt_rel = float(dt) / float(_fade_duration_millis);
	float dt_rel_sqr = dt_rel * dt_rel;
	return dt_rel_sqr / (2.0f * (dt_rel_sqr - dt_rel) + 1.0f);
}

uint32_t LedStrip::_computeCurrentColor(float progress)
{
	float c3 = ((_color_target & 0xFF000000) >> 24) * progress + ((_color_initial & 0xFF000000) >> 24) * (1.0f - progress);
	float c2 = ((_color_target & 0xFF0000) >> 16) * progress + ((_color_initial & 0xFF0000) >> 16) * (1.0f - progress);
	float c1 = ((_color_target & 0xFF00) >> 8) * progress + ((_color_initial & 0xFF00) >> 8) * (1.0f - progress);
	float c0 = (_color_target & 0xFF) * progress + (_color_initial & 0xFF) * (1.0f - progress);
	return uint32_t(c3) << 24 | uint32_t(c2) << 16 | uint32_t(c1) << 8 | uint32_t(c0);
}
