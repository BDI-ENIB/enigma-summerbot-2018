#include "panel_activation_protocol.hpp"
#include "../../robot.h"
#include "../../ia.hpp"
#include "../../pinout.h"
#include "../../nodes.hpp"
#include <Arduino.h>

void PanelActivationProtocol::update(IA *ia){ //execute the next action of this protocol
  switch(state){
    case 0:
    ia->mb->moveTo(g_roadMain.getX(), g_roadMain.getY(ia->getFlag("side")),(ia->getFlag("side")?1:-1)*PI/2);
    Serial.println("LOG Starting_PanelActivationProtocol");
    break;
    case 1:
    ia->mb->moveTo(g_roadButton.getX(), g_roadButton.getY(ia->getFlag("side")),0);
    break;
    case 2:
    ia->mb->translate(ROBOT_1_W/2-g_roadButton.getX()+20);
    Serial.println("LOG going_to_the_button");
    break;
    case 3:
    ia->mb->translateRPM(-60,60);
    Serial.println("LOG smashing_the_button");
    if(doesRecalibrate==0){
      state+=4;
    }
    break;
    case 4:
    ia->mb->setPosition(ROBOT_1_W/2+20, ia->getFlag("side"!=0?1130:1870), 0);
    ia->mb->translate(60);
    break;
    case 5:
    ia->mb->moveTo(unloadingZones[2].getX(), unloadingZones[2].getY(ia->getFlag("side")), 0);
    break;
    case 6:
    ia->mb->translate(-7);
    break;
    case 7:
    if((!digitalRead(DIST_BACK_LEFT) || !digitalRead(DIST_BACK_RIGHT))
  /*&& ia->getFlag("simulator")==0*/){
      state-=2;
      if(digitalRead(DIST_BACK_LEFT) || digitalRead(DIST_BACK_RIGHT)){
        ia->mb->translate(10);
        ia->mb->rotate(digitalRead(DIST_BACK_LEFT)==0?PI/16:-PI/16);
      }
    }else{
      Serial.println("LOG X-Axis_recalibrated");
      ia->mb->setPosition(ROBOT_1_W/2+60, unloadingZones[2].getY(ia->getFlag("side")), 0);
    }
    break;
    case 8:
    ia->mb->translate(60);
    Serial.println("LOG backing_away");
    break;
    case 9:
    ia->mb->moveTo(350, ia->getFlag("side")?1130:1870, 0);
    break;
    default:
    // Anomality
    break;
  }
  state++;
}

bool PanelActivationProtocol::isCompleted(){ //wether the last action of this protocol have already been executed or not
    return doesReposition?(state>9):(state>8);
}

unsigned short int PanelActivationProtocol::getPriority(IA *ia){
  return priority;
}
