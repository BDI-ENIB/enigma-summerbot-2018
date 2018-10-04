#ifndef NODES_HPP
#define NODES_HPP 1

#include "src/summerbot-pathfinder/pathfinder.h"
#include "terrain.h"
#include "robot.h"

class Node {
  private:
    int x, y;
    double a;
  public:
    int getX(){
      return x;
    }
    int getY(bool isGreenSide){
      return isGreenSide?y:3000-y;
    }
    double getA(bool isGreenSide){
      return isGreenSide?a:-a;
    }
    Node(int x, int y): x{x}, y{y}, a{0} {}
    Node(int x, int y,double a): x{x}, y{y}, a{a} {}
};

extern Node g_roadMain;
extern Node g_roadButton;
extern Node g_start;
extern Node g_cross1;
extern Node g_cross2;
extern Node g_cross3;

extern Node unloadingZones[];
extern Node RoadCubeLoadingZone[];
extern Node RealCubeLoadingZone[];
extern Node RoadBuldozerCubeLoadingZone[];
extern Node RealBuldozerCubeLoadingZone[];
#endif
