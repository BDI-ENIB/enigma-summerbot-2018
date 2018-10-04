#include "nodes.hpp"
Node g_roadMain =Node(325,610);
Node g_roadButton =Node(325,1130);
Node g_start=Node(180,200);
Node g_cross1=Node(550,650);
Node g_cross2=Node(990,300);
Node g_cross3=Node(1500,900);

Node unloadingZones[]={
  Node(180+60,490),
  Node(180+60,670),
  Node(180+60,850)
};
Node RoadCubeLoadingZone[]={
  Node(540,610),
  Node(1190,610),
  Node(1500,610)
};
Node RealCubeLoadingZone[]={
  Node(540,850-ROBOT_1_W/2,PI/2),
  Node(1190,280+44+ROBOT_1_W/2,-PI/2),
  Node(1500,1100-ROBOT_1_W/2,PI/2)
};
Node RoadBuldozerCubeLoadingZone[]={
Node(840,1130,-PI/2),
Node(1540,610,-PI/2)
};
Node RealBuldozerCubeLoadingZone[]={
Node(840,850,PI),
Node(1540,200,9*PI/10)
};
