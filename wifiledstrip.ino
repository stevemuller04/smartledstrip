#include <WiFiManager.h>
#include "config.h"

#include "LedStrip.h"
#include "AnimationQueue.h"
#include "Control.h"
#include "HttpUi.h"
#include "Settings.h"

LedStrip ledstrip(LEDSTRIP_NUMPIXELS, LEDSTRIP_PIN, LEDSTRIP_MODE);
AnimationQueue animationQueue(&ledstrip);
Control control(&animationQueue, ANIMATION_FASTFADE_DURATION);
HttpUi http_ui(&control);

void setup()
{
	//Serial.begin(9600);

	control.begin();
	ledstrip.begin();
	animationQueue.begin();

	WiFiManager wifiManager;
	wifiManager.setHostname("LEDSTRIP");
	wifiManager.autoConnect("LED Strip");

	http_ui.begin();

	Settings settings = control.getSettings();
	animationQueue.addAnimation(settings.color1, settings.fade_duration, true);
	animationQueue.addAnimation(settings.color2, settings.fade_duration, true);
}

void loop()
{
	http_ui.loop();
	animationQueue.loop();
	ledstrip.loop();
	yield();
}
