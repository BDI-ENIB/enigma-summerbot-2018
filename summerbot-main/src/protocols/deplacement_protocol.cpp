#include "deplacement_protocol.h"
#include "../../robot.h"
#include "../../ai.h"

void DeplacementProtocol::update(IA*ia){
  switch (state) {
    case 1:
    ia->mb->translate(10);
    break;
    case 2:
    ia->mb->rotate(PI/2);
    break;
    case 3:
    ia->mb->translate(10);
    break;
    case 4:
    ia->mb->rotate(PI/2);
    break;
    case 5:
    ia->mb->translate(10);
    break;
    case 6:
    ia->mb->rotate(PI/2);
    break;
  }
  state++;
}

boolean DeplacementProtocol::isCompleted(){ //wether the last action of this protocol have already been executed or not
  return state >6;
}

unsigned short int DeplacementProtocol::getPriority(IA *ia){
  return priority;
}
