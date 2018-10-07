#include "bee_activation_protocol.h"
#include "../../robot.h"
#include "../../ia.hpp"
#include "../../nodes.hpp"
#include "../../pinout.h"
#include "../Bee/Bee.h"
#include <Arduino.h>

void BeeActivationProtocol::update(IA *ia){ //execute the next action of this protocol
  Servo *servo;
  switch(state){
    case 0:
    ia->setFlag("ignore_IR", 1);
    ia->mb->moveTo(RoadCubeLoadingZone[2].getX(), RoadCubeLoadingZone[2].getY(ia->getFlag("side")),  (ia->getFlag("side")?1:-1)*(-PI/4));
    break;
    case 1:
    ia->mb->moveTo(2000-ROBOT_1_W_DEPLOYED/2-27,(ia->getFlag("side")?(ROBOT_1_W_DEPLOYED+30):3000-(ROBOT_1_W_DEPLOYED+30)), (ia->getFlag("side")?1:-1)*PI/4);
    Serial.println("LOG Starting_BeeSplatProtocol_(bugsplat...understood?)");
    break;
    case 2:
    ia->bee->deploy();
    break;
    case 3:
    ia->mb->rotate((ia->getFlag("side")?1:-1)*PI/4);
    break;
    case 4:
    ia->mb->translate(500-ROBOT_1_W-100);
    Serial.println("LOG splating_the_bee..._hopefully_xD");
    break;
    case 5:
    ia->mb->rotate(PI/2);
    ia->bee->retract();
    break;
    case 6:
    ia->setFlag("ignore_IR", 0);
    break;
    default:
    // Abnormal bee-havior
    break;
  }
  state++;
}

boolean BeeActivationProtocol::isCompleted(){ //wether the last action of this protocol have already been executed or not
  return state>6;
}

unsigned short int BeeActivationProtocol::getPriority(IA *ia){
  return priority;
}

