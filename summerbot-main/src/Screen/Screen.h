#ifndef SCREEN_H
#define SCREEN_H

//---define

#define THICKNESS 10
#define COLORED     0
#define UNCOLORED   1


//---headers

#include <SPI.h>
#include "src/epd/epd4in2.h"
#include "src/epd/epdpaint.h"
#include "Icons.h"


//---Screen class

class Screen {

public :

	// constructor
	Screen(bool simulator);

	// functions
	void init();
	void setScore(const int score,const bool refreshEnabled = true);
	void drawIcon(const int iconId, const bool refreshEnabled = true);
	void clearIcon(const int iconId, const bool refreshEnabled = true);
	void showInitFrame(const int score = 0);
	void refresh();
	void clearScreen();
	bool isBusy() {
		if(simulator_)return false;
		return epd_->isBusy();
	}

private :

	// internal function
	void drawNumber(const int id);

	// variables
	Epd *epd_;
	Paint *numberBuffer_;
	bool simulator_;
	char numbers_[10];
	int table_[7][4];
	char icons_[9][128];
};


#endif

