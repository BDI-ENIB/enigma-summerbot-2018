#ifndef TERRAIN_H
#define TERRAIN_H
bool globalSide=false;
//cubes
#define PLUS_1_X 1190
#define PLUS_1_Y (globalSide?300:2700)

#define PLUS_2_X 540
#define PLUS_2_Y (globalSide?850:2150)

#define PLUS_3_X 1500
#define PLUS_3_Y (globalSide?1100:1900)

#define PLUS_4_X 1500
#define PLUS_4_Y (globalSide?1900:1100)

#define PLUS_5_X 540
#define PLUS_5_Y (globalSide?2150:850)

#define PLUS_6_X 1190
#define PLUS_6_Y (globalSide?2700:300)

#define TOWER_AREA_X 0
#define TOWER_AREA_Y (globalSide?765:2235)
#define TOWER_AREA_W 730

#define CUBE_W 60
//water
#define SOURCE_CLEAR_X 840
#define SOURCE_CLEAR_Y (globalSide?0:3000)

#define DRAIN_CLEAR_X 0
#define DRAIN_CLEAR_Y (globalSide?0:3000)

#define SOURCE_WASTE_X 2000
#define SOURCE_WASE_Y (globalSide?610:2390)

#define DRAIN_WASTE_X 1750
#define DRAIN_WASTE_Y (globalSide?1197:1803)

#endif