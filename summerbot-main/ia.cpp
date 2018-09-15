#include "ia.hpp"
#define LEFT_SIDE true


IA::IA(MotionBase *mb, Claw *claw){
	this->mb = mb;
	this->claw = claw;
}

void IA::addCommands(Command commandList[], short listSize){
	for(char i = 0; i<listSize; i++){
		protocol[i+protocolLenght]=commandList[i];
	}
  protocolLenght+=listSize;
}

void IA::update(){
  //Serial.println(this->toString());
	if(!mb->isBusy()/*&&!claw->isBusy()*/){
		if(currentCommandIndex+1>=protocolLenght){
			return;		
		}		
		currentCommandIndex++;
		executeCommand(protocol[currentCommandIndex].commandType, protocol[currentCommandIndex].args);
    Serial.println(this->toString());
	}
}

String IA::toString(){
  return "Current ActionType: "+String(protocol[currentCommandIndex].commandType)
        +" | arg[0]: "+String(protocol[currentCommandIndex].args[0])
        +" | currentCommandIndex: "+String(currentCommandIndex)
        +" | maxCommandIndex: "+String(protocolLenght)
        +" | mb->isBusy(): "+(mb->isBusy()?"true":"false")
        +" | claw->isBusy(): "+(claw->isBusy()?"true":"false");
}

void IA::executeCommand(CommandType command, double args[3]){
		// {forward, rotate, load, unload, stack}
		if(command==CommandType::forward){
			mb->translate(args[0]);
		}else if(command==CommandType::rotate){
			mb->rotate(args[0]);
		}else if(command==CommandType::moveTo){
			mb->moveTo(args[0], args[1], args[2]); //X, Y, TETA
		}else if(command==CommandType::load){
			claw->load();
		}else if(command==CommandType::unload){
			claw->unload();
		}else if(command==CommandType::stack){
			claw->stack();
		}else if(command==CommandType::buldozer){
			//claw.openWide();
		}else if(command==CommandType::recalibrate){
      mb->translate(args[0]);
		}
}

