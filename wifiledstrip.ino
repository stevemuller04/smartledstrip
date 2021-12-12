#include <WiFiManager.h>
#include "config.h"

#include "Settings.h"
#include "LedStrip.h"
#include "AnimationQueue.h"
#include "HttpUi.h"

Settings settings;
LedStrip ledstrip(LEDSTRIP_NUMPIXELS, LEDSTRIP_PIN, LEDSTRIP_MODE);
AnimationQueue anim(&ledstrip);
HttpUi ui(&settings, &anim, ANIMATION_FASTFADE_DURATION);

void setup() {
	//Serial.begin(9600);

	settings.begin();
	ledstrip.begin();
	anim.begin();

	WiFiManager wifiManager;
	wifiManager.setHostname("LEDSTRIP");
	wifiManager.autoConnect("LED Strip");

	ui.begin();

	anim.addAnimation(settings.color1, settings.fade_duration, true);
	anim.addAnimation(settings.color2, settings.fade_duration, true);
}

void loop() {
	ui.loop();
	anim.loop();
	ledstrip.loop();
}
