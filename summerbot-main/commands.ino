#include "src/SerialCommand/SerialCommand.h"
SerialCommand sCmd;
long t=millis();
bool sendPos=false;
void commands_init(){
  sCmd.addCommand("GOTO", goto_command);
  sCmd.addCommand("SPOS", setpos_command);
  sCmd.addCommand("+POS",   startpos_command);
  sCmd.addCommand("-POS",   stoppos_command);
  sCmd.addCommand("GMOV",   getmoves_command);

  sCmd.addCommand("WHOIS", whois_command);       // who are you?
  sCmd.addCommand("AYR", ready_command);       // are you ready?
}
void commands_update(){
  sCmd.readSerial();
  if(millis()-t>100){
    if(sendPos){
      Serial.print("POS ");Serial.print(mb->getX());Serial.print(" ");Serial.print(mb->getY());Serial.print(" ");Serial.println(mb->getA());
    }
    t=millis();
  }
}
void goto_command() {
  mb->moveTo(String(sCmd.next()).toFloat(),String(sCmd.next()).toFloat(),String(sCmd.next()).toFloat());
}
void setpos_command() {
  mb->setPosition(String(sCmd.next()).toFloat(),String(sCmd.next()).toFloat(),String(sCmd.next()).toFloat());
}
void whois_command() {
  Serial.println("IAM summerbot");
}
void ready_command() {
  Serial.println("READY");
}
void startpos_command(){
  sendPos=true;
}
void stoppos_command(){
  sendPos=false;
}
void getmoves_command(){
  Serial.println("MOV "+mb->movesString());
}
