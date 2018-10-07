//---headers

#include "Screen.h"


//---constructor

Screen::Screen(bool s):
	simulator_{s},

	numbers_{0b00111111,
			 0b00110000,
		     0b01101101,
			 0b01111001,
			 0b01110010,
			 0b01011011,
			 0b01011111,
			 0b00110001,
			 0b01111111,
			 0b01111011 },

	table_{ {20-THICKNESS,20-THICKNESS,80+2*THICKNESS,2*THICKNESS},
			{20-THICKNESS,20-THICKNESS,2*THICKNESS,80+THICKNESS},
			{20-THICKNESS,100,2*THICKNESS,80+THICKNESS},
			{20-THICKNESS,180-THICKNESS,80+2*THICKNESS,2*THICKNESS},
			{100-THICKNESS,100,2*THICKNESS,80+THICKNESS},
			{100-THICKNESS,20-THICKNESS,2*THICKNESS,80+THICKNESS},
			{20-THICKNESS,100-THICKNESS,80+2*THICKNESS,2*THICKNESS} }
	{

	if(simulator_){
		Serial.println("LOG Inited_Screen");
	}
	else {
		epd_ = new Epd;
		if (epd_->Init() != 0) {
			// Serial.print("e-Paper init failed");
		}
		// Serial.println("post");
	}

	unsigned char internalNumberBuffer[4000]; // > 120*200/8
	numberBuffer_ = new Paint(internalNumberBuffer,120,200);

}

void
Screen::drawNumber(const int number) {

	// Serial.println("before clear");
	numberBuffer_->Clear(UNCOLORED);
	// Serial.println("after clear");

	for(volatile int j=0; j<7; j++) { //volatile otherwise it shit itself

		if(numbers_[number] & 1<<j) {

			const int tx = table_[j][0];
			const int ty = table_[j][1];
			const int dtx = table_[j][2];
			const int dty = table_[j][3];

			// Serial.println("random serial");
			if(j%2 == 0) { //vertical lines
				// Serial.println(j);
				for(volatile int k = 0; k < dtx; k++) { //same thing as earlyer
					numberBuffer_->DrawVerticalLine(tx+k,ty,dty,COLORED);
					// Serial.println("DrawVerticalLine");
				}
			}
			else { //horizontal lines
				// Serial.println(j);
				for(volatile int k = 0; k < dty; k++) { //same thing as earlyer
					numberBuffer_->DrawHorizontalLine(tx,ty+k,dtx,COLORED);
					// Serial.print("DrawHorizontalLine");
				}
			}

		}

	}
	// Serial.println("end");
}

void
Screen::setScore(const int score, const bool refreshEnabled) {
	if(simulator_) {
		Serial.println("LOG screen_score_"+String(score));
	} 
	else {
		
		if(score > 999) { return; } //score is too high

		const char hundreeds = int(score/100);
		const char decades = int((score-hundreeds*100)/10);
		const char units = int(score%10);


		// Serial.println("drawHundreeds");
		drawNumber(hundreeds);
		epd_->SetPartialWindow(numberBuffer_->GetImage(), 16, 40,
												 numberBuffer_->GetWidth(), numberBuffer_->GetHeight());

		// Serial.println("drawDecades");
		drawNumber(decades);
		epd_->SetPartialWindow(numberBuffer_->GetImage(), 144, 40,
												 numberBuffer_->GetWidth(), numberBuffer_->GetHeight());

		// Serial.println("drawUnits");
		drawNumber(units);
		epd_->SetPartialWindow(numberBuffer_->GetImage(), 272, 40,
												 numberBuffer_->GetWidth(), numberBuffer_->GetHeight());
	}
	
	if(refreshEnabled) {
		refresh();
	}

	// Serial.println("after drawNumbers");

}

void
Screen::drawIcon(const int iconId, const bool refreshEnabled) {

	if(simulator_) {
		Serial.println("LOG screen_drawn_icon_"+String(iconId));
	}
	else {
		
		if(iconId < 0 || iconId > 8) { //id does not correspond to any icon
			return;
		}

		epd_->SetPartialWindow(iconsTable[iconId],(3+iconId*5)*8,264,32,32);
		
	}
	
	if(refreshEnabled) {
		refresh();
	}
	
}

void
Screen::clearIcon(const int iconId, const bool refreshEnabled) {

	if(simulator_) {
		Serial.println("LOG screen_cleared_icon_"+String(iconId));
	}
	else {
		
		if(iconId < 0 || iconId > 8) { //id does not correspond to any icon
			return;
		}

		epd_->fillPartialWindow((3+iconId*5)*8,264,32,32,UNCOLORED);

	}
	
	if(refreshEnabled) {
			refresh();
	}
	
}

void
Screen::showInitFrame(const int score) {

	if(simulator_) {
		Serial.println("LOG screen_show_init_frame");
	}
	else {
		
		/*unsigned char internalTmpBuffer[2000]; // > 400*20/8
		Paint *tmpBuffer = new Paint(internalTmpBuffer,400,20);
		Serial.println(tmpBuffer->GetWidth());
			Serial.println(tmpBuffer->GetHeight());
		Serial.println(tmpBuffer->GetWidth()*tmpBuffer->GetHeight()/8);
		tmpBuffer->Clear(COLORED);

		tmpBuffer->DrawStringAt(0,0,"Enigma",&Font16,UNCOLORED);
		epd_->SetPartialWindow(tmpBuffer->GetImage(), 0, 0,
												 tmpBuffer->GetWidth(), tmpBuffer->GetHeight());*/

		epd_->ClearFrame();
		// Serial.println("back in showInitFrame");

		epd_->fillPartialWindow(0,0,400,20,COLORED); //top of the screen
		epd_->fillPartialWindow(0,260,400,1,COLORED); //bottom of the screen
	}
	
	setScore(score);

	// sSerial.println("end");

}

void
Screen::refresh() {

	if(isBusy()) { return; } //screen is not ready

	if(simulator_) {
		Serial.println("LOG screen_refresh");
	}
	else {
		epd_->DisplayFrame();
	}
}

void 
Screen::clearScreen() {
	
	if(simulator_) {
		Serial.println("LOG screen_clear_frame");
	}
	else {
		epd_->ClearFrame();
		epd_->DisplayFrame();
	}
}

