//---headers

#include "Screen.h"
#include "IconsId.h"

Screen *screen;

void setup() {

	Serial.begin(9600);
	delay(100); //for serial

	Serial.println("start");
	delay(100);
	screen = new Screen(true);

	Serial.println("showInitFrame");
	delay(100);
	// show the base frame and set the background
	screen->showInitFrame();
	Serial.println(2);
	delay(100);

	Serial.println("set up image");
	delay(100);
	// write the score in the screen buffer
	screen->setScore(123,false);
	// write the icon in the screen buffer

	Serial.println("draw armed");
	screen->drawIcon(LAUNCHED);

	// display the buffer
	Serial.println("refresh");
	screen->refresh();

	//wait for the screen to refresh (4sec ~)
	while(screen->isBusy()) {
		delay(100);
	}

	Serial.println("set_up_new_image");
	delay(100);
	// write the score in the screen buffer
	screen->setScore(42,false);
	// clear the icon in the screen buffer
	screen->clearIcon(LAUNCHED,false);
	// write the icon in the screen buffer
	screen->drawIcon(VIVE,false);

	// display the buffer
	Serial.println("refresh");
	screen->refresh();

	//wait for the screen to refresh (4sec ~)
	while(screen->isBusy()) {
		delay(100);
	}

	screen->clearIcon(VIVE,false);
	// write the icon in the screen buffer
	screen->drawIcon(TIME_LIMIT);

	Serial.println("end");

}

void loop() {

	Serial.println("nothing");
	delay(1000);

}

