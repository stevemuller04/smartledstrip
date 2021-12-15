#ifndef INCLUDE_SRC_ANIMATIONQUEUE_H
#define INCLUDE_SRC_ANIMATIONQUEUE_H

#include "LedStrip.h"
#include <LinkedList.h>

class AnimationQueue
{
	struct Item
	{
		uint32_t target_color;
		uint32_t fade_duration;
		bool repeat;
	};

	private:

		LedStrip *_ledstrip;
		LinkedList<Item> _list;
		int _next;

	public:

		AnimationQueue(LedStrip *ledstrip);
		void begin();
		void loop();
		void abort();
		void addAnimation(uint32_t target_color, uint32_t fade_duration, bool repeat);
};

#endif
