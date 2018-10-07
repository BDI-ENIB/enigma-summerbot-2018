#include "buldozer_cube_loading_protocol.h"
#include "../../robot.h"
#include "../../ia.hpp"
#include "../../nodes.hpp"
#include <Arduino.h>

void BuldozerCubeLoadingProtocol::update(IA *ia){ //execute the next action of this protocol
  short unloadingZone=0; // I still don't understand why the fu** does creating a var in a switch throw an error.
  switch(state){
    case 0:
    if(initialPlacement == true){
      ia->mb->moveTo(RoadBuldozerCubeLoadingZone[loadingZone].getX(),
              RoadBuldozerCubeLoadingZone[loadingZone].getY(ia->getFlag("side")),
              RoadBuldozerCubeLoadingZone[loadingZone].getA(ia->getFlag("side")));
    }
    Serial.println("LOG Starting_BuldozerCubeLoadingProtocol_#"+String(loadingZone));
    break;
    case 1:
    ia->mb->moveTo(RealBuldozerCubeLoadingZone[loadingZone].getX(),
              RealBuldozerCubeLoadingZone[loadingZone].getY(ia->getFlag("side")),
              RealBuldozerCubeLoadingZone[loadingZone].getA(ia->getFlag("side")));
    Serial.println("LOG Moving_Toward_LoadingZone_#"+String(loadingZone));
    break;
    case 2:
    if(predefinedUnloadingPos<0){
      for(;ia->getFlag("cross"+(String)unloadingZone+"SuccessfullyCleared");unloadingZone++);
    }else{
      unloadingZone = predefinedUnloadingPos;
    }
    ia->mb->moveTo(unloadingZones[unloadingZone].getX()+25+20,
              unloadingZones[unloadingZone].getY(ia->getFlag("side")),
              PI);

    ia->setFlag("cross"+(String)unloadingZone+"SuccessfullyCleared", 1);
    Serial.println("LOG PushingCubesViolently.lol._Direction_UnloadingZone_#"+(String)unloadingZone);
    break;
    case 3:
    ia->mb->translate(-80);
    break;
    case 4:
    ia->mb->moveTo(g_roadMain.getX(), g_roadMain.getY(ia->getFlag("side")),0);
    break;
    default:
    // Abnormal comportment
    break;
  }
  state++;
}

boolean BuldozerCubeLoadingProtocol::isCompleted(){ //wether the last action of this protocol have already been executed or not
  return doesReposition?(state>4):(state>3);
}

unsigned short int BuldozerCubeLoadingProtocol::getPriority(IA *ia){
  return priority;
}

