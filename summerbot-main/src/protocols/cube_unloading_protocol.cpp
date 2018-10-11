#include "cube_unloading_protocol.h"
#include "../../robot.h"
#include "../../ai.h"
#include "../../nodes.hpp"
#include <Arduino.h>

void CubeUnloadingProtocol::update(IA *ia){ //execute the next action of this protocol
  switch(state){
    case 0:
    ia->mb->moveTo(g_roadMain.getX(), g_roadMain.getY(ia->getFlag("side")));
    Serial.println("LOG Starting_CubeUnloadingProtocol");
    break;
    case 1:
    ia->mb->moveTo(unloadingZones[unloadingZone].getX(), unloadingZones[unloadingZone].getY(ia->getFlag("side")), PI);
    Serial.println("LOG going_to_unload_zonel_#"+String(unloadingZone));
    break;
    case 2:
    ia->mb->translate(60); // a cube should be 58±
    Serial.println("LOG forward");
    break;
    case 3:
    ia->claw->unload();
    ia->setFlag("towerLoaded",0);
    Serial.println("LOG empty");
    break;
    case 4:
    ia->mb->translate(-60);
    Serial.println("LOG back");
    break;
    case 5:
    ia->mb->moveTo(g_roadMain.getX(),g_roadMain.getY(ia->getFlag("side")));
    Serial.println("LOG back_to_base");
    ia->setFlag("cross"+(String)unloadingZone+"SuccessfullyCleared", 1);
    break;
    default:
    // Anomality
    break;
  }
  state++;
}

boolean CubeUnloadingProtocol::isCompleted(){ //wether the last action of this protocol have already been executed or not
  return state >5;
}

unsigned short int CubeUnloadingProtocol::getPriority(IA *ia){
  if(ia->getFlag("cross"+(String)unloadingZone+"SuccessfullyCleared")){
    return PRIORITY_NULL;
  }
  return (ia->getFlag("towerLoaded")==1)?priority:PRIORITY_NULL;
}

