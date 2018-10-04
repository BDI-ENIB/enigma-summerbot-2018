#include "cube_loading_protocol.hpp"
#include "../../robot.h"
#include "../../ia.hpp"
#include "../../nodes.hpp"
#include <Arduino.h>

void CubeLoadingProtocol::update(IA *ia){ //execute the next action of this protocol
  switch(state){
    case 0:
    ia->mb->moveTo(RoadCubeLoadingZone[loadingZone].getX(), RoadCubeLoadingZone[loadingZone].getY(ia->getFlag("side")));
    Serial.println("LOG Starting_CubeLoadingProtocol_#"+String(loadingZone));
    break;
    case 1:
    ia->mb->moveTo(RealCubeLoadingZone[loadingZone].getX(), RealCubeLoadingZone[loadingZone].getY(ia->getFlag("side")), RealCubeLoadingZone[loadingZone].getA(ia->getFlag("side")));
    Serial.println("LOG Going_to_first_cube");
    break;
    case 2:
    ia->claw->load();
    ia->setFlag("towerLoaded",1);
    Serial.println("LOG loading_cube_1");
    break;
    case 3:
    ia->mb->translate(60);
    Serial.println("LOG going_to_cube_2");
    break;
    case 4:
    ia->claw->load();
    Serial.println("LOG loading_cube_2");
    case 5:
    ia->mb->translate(60);
    Serial.println("LOG going_to_cube_3");
    break;
    case 6:
    ia->claw->load();
    Serial.println("LOG loading_cube_3");
    break;
    case 7:
    ia->mb->translate(-abs((ia->getFlag("side")?610:2390)-ia->mb->getY()));
    if (isRecalibrationRequested){
      ia->setFlag("recalibrationNeeded", 1.0);
      Serial.println("LOG replacing");
      Serial.println("LOG waiting_for_recalibration");
    }else{
      Serial.println("LOG waiting_for_recalibration");
    }
    default:
    // Anomality
    break;
  }
  state++;
}

boolean CubeLoadingProtocol::isCompleted(){ //wether the last action of this protocol have already been executed or not
  return state>7;
}

unsigned short int CubeLoadingProtocol::getPriority(IA *ia){
  return (ia->getFlag("towerLoaded")==0)?priority:0;
}
