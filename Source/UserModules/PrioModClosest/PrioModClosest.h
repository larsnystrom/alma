#ifndef PrioModClosest_
#define PrioModClosest_

#include "IKAROS.h"

class PrioModClosest: public Module
{
public:
    static Module * Create(Parameter * p) { return new PrioModClosest(p); }
    float dist(float x1, float y1, float z1, float x2, float y2, float z2);

    PrioModClosest(Parameter * p) : Module(p) {}
    virtual ~PrioModClosest() {}

    void Init();
    void Tick();

    int maxMarkers;

    float ** markers;

    float * clocam;
    float * attended;
};

#endif

