#include "Pathfinder.h"

PathFinderNode *nodes[MAX_NODES]={
	(new PathFinderNode(0,0)),
	(new PathFinderNode(9,1)),
	(new PathFinderNode(10,10)),
	(new PathFinderNode(0,10))
};

void setup(){
	Serial.begin(9600);
	delay(5000);
	Serial.println("Starting");
	nodes[0]->addLink(nodes[1]);
	nodes[0]->addLink(nodes[3]);
	nodes[2]->addLink(nodes[1]);
	nodes[2]->addLink(nodes[3]);
	PathFinder pf;
	pf.addNode(nodes[0]);
	pf.addNode(nodes[1]);
	pf.addNode(nodes[2]);
	pf.addNode(nodes[3]);
	Serial.println("from node to node");
	long d=micros();
	PathFinderNode *n=pf.findPath(nodes[0],nodes[2]);
	long a=micros();
	Serial.println("done.");
	while(n){
		Serial.print("|");
		Serial.print(n->x);
		Serial.print(",");
		Serial.print(n->y);
		Serial.print(">");
		n=n->next;
	}
	Serial.println();
	Serial.print("temps de calcul: ");
	Serial.print(a-d);
	Serial.println(" us");
}

void loop(){
	//has to be there even if empty to allow compilation  
}

