#ifndef CUBE_LOAD_PROTOCOL
#define CUBE_LOAD_PROTOCOL 1

#include "../../protocol.h"
class IA;

class CubeLoadingProtocol: public Protocol {
  public:
    CubeLoadingProtocol(unsigned char lz,unsigned short p, bool irr = false):loadingZone{lz}, priority{p}, isRecalibrationRequested{irr} {};
    void update(IA *ia) override; //execute the next action of this protocol
    bool isCompleted() override; //wether the last action of this protocol have already been executed or not
    unsigned short int getPriority(IA *ia) override;
  private:
    unsigned char loadingZone = 0;
    unsigned char state = 0;
    unsigned short priority;
    bool isRecalibrationRequested;
};

#endif

