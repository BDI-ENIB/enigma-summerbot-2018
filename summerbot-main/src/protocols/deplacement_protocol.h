#ifndef DEPLACEMENT_PROTOCOL
#define DEPLACEMENT_PROTOCOL 1

#include "../../protocol.h"
class IA;

class DeplacementProtocol: public Protocol {
  public:
    DeplacementProtocol(unsigned short p): priority{p} {};
    void update(IA *ia) override; //execute the next action of this protocol
    bool isCompleted() override; //wether the last action of this protocol have already been executed or not
    unsigned short int getPriority(IA *ia) override;
  private:
    unsigned short priority;
};

#endif
