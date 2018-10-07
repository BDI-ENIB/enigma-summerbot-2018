#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <Arduino.h>

#define MAX_NEIGHBORS 5
#define NEIGHBOR_LOOP unsigned short i=0;i<MAX_NEIGHBORS;i++

#define MAX_NODES 30
#define NODE_LOOP unsigned short i=0;i<MAX_NODES;i++

class PathFinderNode{
	public:
	//constants
	PathFinderNode *neighbors[MAX_NEIGHBORS];
	double neighborDists[MAX_NEIGHBORS];
	double x;
	double y;
	bool addArc(PathFinderNode *neighbor);
	bool addLink(PathFinderNode *neighbor);
	//Dijkstra
	PathFinderNode *prev;
	PathFinderNode *next;
	double dist;
	bool done;
	void reset();
	void computeNeighbors();
	
	//constructor
	PathFinderNode(double x,double y):x{x},y{y} {
		for(unsigned short i=0;i<MAX_NEIGHBORS;i++){
		neighbors[i]=0;
		neighborDists[i]=0;
		//Serial.println(i);
		}
		reset();
	}
};

class PathFinder{
	private:
		PathFinderNode *nodes_[MAX_NODES];
	public:
		PathFinder() {
		for(NODE_LOOP) nodes_[i] = 0;
		};
		//mainly external
		bool addNode(PathFinderNode *n);
		PathFinderNode* findPath(PathFinderNode *startNode,PathFinderNode *endNode);//node to node [exact,exact]
		PathFinderNode* findPath(double sx,double sy,PathFinderNode *endNode);//closest node to node [nearest,exact]
		PathFinderNode* findPath(double sx,double sy,double ex,double ey);//closest node to closest node [nearest,nearest] (need a motion from the end node to the actual destination)
		PathFinderNode* findClosestNode(double x,double y);
		//mainly internal
		PathFinderNode* findMinDistNodeNotDone();
};
#endif

