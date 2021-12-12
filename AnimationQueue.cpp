#include "AnimationQueue.h"

AnimationQueue::AnimationQueue(LedStrip *ledstrip) :
	_ledstrip(ledstrip),
	_next(0)
{
}

void AnimationQueue::begin()
{
}

void AnimationQueue::loop()
{
	// Only process queue if LED strip has finished fading
	if (!_ledstrip->isFading() && _next < _list.size())
	{
		// Get the next animation
		Item i = _list.get(_next);

		// Start fading animation
		_ledstrip->fadeToColor(i.target_color, i.fade_duration);

		// Prepare queue for upcoming animation
		if (i.repeat)
			_next = (_next + 1) % _list.size();
		else
			_list.remove(_next);
	}
}

void AnimationQueue::abort()
{
	_list.clear();
	_ledstrip->abort();
	_next = 0;
}

void AnimationQueue::addAnimation(uint32_t target_color, uint32_t fade_duration, bool repeat)
{
	Item i = { target_color, fade_duration, repeat };
	_list.add(i);
}
