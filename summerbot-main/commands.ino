#include "src/SerialCommand/SerialCommand.h"

SerialCommand sCmd;
long t = millis();
bool sendPos = false;
bool simStart=false;

void commands_init() {
	sCmd.addCommand("START", start_command);
	sCmd.addCommand("GOTO", goto_command);
	sCmd.addCommand("SPOS", setpos_command);
	sCmd.addCommand("+POS",   startpos_command);
	sCmd.addCommand("-POS",   stoppos_command);
	sCmd.addCommand("GMOV",   getmoves_command);
	sCmd.addCommand("GSIDE",   getside_command);
	sCmd.addCommand("SSIDE",   setside_command);

	sCmd.addCommand("WHOIS", whois_command);       // who are you?
	sCmd.addCommand("AYR", ready_command);       // are you ready?
}

void commands_update() {
	sCmd.readSerial();
	if (millis() - t > 300) {
		if (sendPos) {
			double x=0,y=0,a=0;
			mb->getRealPos(&x,&y,&a);
			Serial.print("POS ");
			Serial.print(x);
			Serial.print(" ");
			Serial.print(y);
			Serial.print(" ");
			Serial.println(a);
		}
		t = millis();
	}
}

void remoteStarter(){
	while(!simStart){
		commands_update();
		vive_update();
		delay(10);
	}
	delay(1000);
}

void start_command() {
	simStart = true;
}

void goto_command() {
	mb->moveTo(String(sCmd.next()).toFloat(), String(sCmd.next()).toFloat(), String(sCmd.next()).toFloat());
}

void setpos_command() {
	mb->setPosition(String(sCmd.next()).toFloat(), String(sCmd.next()).toFloat(), String(sCmd.next()).toFloat());
}

void whois_command() {
	Serial.println("IAM summerbot");
}

void ready_command() {
	Serial.println("READY");
}

void startpos_command() {
	sendPos = true;
}

void stoppos_command() {
	sendPos = false;
}

void getmoves_command() {
	Serial.println("MOV " + mb->movesString());
}

void getside_command() {
	Serial.print("SIDE ");Serial.println(ia->getFlag("side"));
}

void setside_command(){
	forcedSide=true;
	globalSide = (bool)String(sCmd.next()).toInt();
	ia->setFlag("side", globalSide);
	mb->setPosition(START_1_X,START_1_Y,START_1_A);
	getside_command();
}

