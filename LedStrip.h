#ifndef INCLUDE_SRC_LEDSTRIP_H
#define INCLUDE_SRC_LEDSTRIP_H

#include <Adafruit_NeoPixel.h>

class LedStrip
{
	private:

		Adafruit_NeoPixel _pixels;
		uint16_t _pixels_num;
		uint32_t _pixels_last_color_set; 
		uint32_t _color_initial;
		uint32_t _color_target;
		uint32_t _fade_start_millis;
		uint32_t _fade_duration_millis;
		bool _fade_done;

		float _computeFadeProgress();
		uint32_t _computeCurrentColor(float progress);

	public:

		LedStrip(uint16_t n, int16_t pin, neoPixelType type);
		void begin();
		void loop();
		void fadeToColor(uint32_t color, uint32_t duration_millis);
		bool isFading();
		void abort();
};

#endif