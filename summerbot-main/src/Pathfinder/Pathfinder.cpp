#include "Pathfinder.h"

//PATHFINDERNODE
bool PathFinderNode::addArc(PathFinderNode *neighbor){
	for(NEIGHBOR_LOOP){
		if(neighbors[i]==0){
		neighbors[i]=neighbor;
		neighborDists[i]=(x-neighbor->x)*(x-neighbor->x)+(y-neighbor->y)*(y-neighbor->y);
		return true;
		}
	}
	return false;
}

bool PathFinderNode::addLink(PathFinderNode *neighbor){
	for(NEIGHBOR_LOOP){
		if(neighbors[i]==0){
		if(neighbor->addArc(this)){
			neighbors[i]=neighbor;
			neighborDists[i]=(x-neighbor->x)*(x-neighbor->x)+(y-neighbor->y)*(y-neighbor->y);
			return true;
		}else return false;
		}
	}
	return false;
}

void PathFinderNode::reset(){
	dist = 999999;
	done = false;
	prev = 0;
	next = 0;
}

//https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
void PathFinderNode::computeNeighbors(){
	for(NEIGHBOR_LOOP){
		if(neighbors[i]){
		double alt = dist + neighborDists[i];
		if(alt<neighbors[i]->dist){
			neighbors[i]->dist=alt;
			neighbors[i]->prev=this;
		}
		}
	}
}

//PATHFINDER
bool PathFinder::addNode(PathFinderNode *n){
	for(NODE_LOOP){
		if(nodes_[i]==0){
		nodes_[i]=n;
		return true;
		}
	}
	return false;
}

PathFinderNode* PathFinder::findMinDistNodeNotDone(){
	PathFinderNode* minNode = 0;
	double minDist = 999999;
	for(NODE_LOOP){
		if(nodes_[i] && !nodes_[i]->done && nodes_[i]->dist<minDist){
		minDist = nodes_[i]->dist;
		minNode = nodes_[i];
		}
	}
	return minNode;
}

PathFinderNode* PathFinder::findClosestNode(double x,double y){
	PathFinderNode* minNode = 0;
	double minDist = 99999999;
	for(NODE_LOOP){
		if(nodes_[i] && (x-nodes_[i]->x)*(x-nodes_[i]->x)+(y-nodes_[i]->y)*(y-nodes_[i]->y)<minDist){
		minDist = nodes_[i]->dist;
		minNode = nodes_[i];
		}
	}
	return minNode;
}

//https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
PathFinderNode* PathFinder::findPath(PathFinderNode *startNode,PathFinderNode *endNode){
	//variables reset
	for(NODE_LOOP){
		if(nodes_[i])nodes_[i]->reset();
	}
	//1 node path exception
	if(startNode==endNode) return startNode;
	//initialisation
	startNode->dist=0;
	PathFinderNode* curNode = findMinDistNodeNotDone();
	//creation of the Dijkstra map
	while(curNode){
		curNode->done = true;
		curNode->computeNeighbors();
		curNode = findMinDistNodeNotDone();
	}
	//no backward path found
	if(!endNode->prev) return (PathFinderNode*)0;
	//creation of the forward path from the backward path
	curNode=endNode;
	while(curNode->prev){
		curNode->prev->next=curNode;
		curNode = curNode->prev;
	}
	//to iter on return data while(n){something;n=n->next;}
	return startNode;
}

PathFinderNode* PathFinder::findPath(double sx,double sy,PathFinderNode *endNode){
	PathFinderNode* startNode = findClosestNode(sx,sy);//finding closest node from start point
	return findPath(startNode,endNode);
}

PathFinderNode* PathFinder::findPath(double sx,double sy,double ex,double ey){
	PathFinderNode* endNode = findClosestNode(ex,ey);//finding closest node from endpoint
	return findPath(sx,sy,endNode);
}

