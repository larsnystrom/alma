#ifndef PrioMod_
#define PrioMod_

#include "IKAROS.h"

class PrioMod: public Module
{
public:
    static Module * Create(Parameter * p) { return new PrioMod(p); }
    float dist(float x1, float y1, float z1, float x2, float y2, float z2);

    PrioMod(Parameter * p) : Module(p) {}
    virtual ~PrioMod() {}

    void Init();
    void Tick();

    int maxMarkers;

    float ** markers;

    float * clocam;
    float * attended;
};

#endif

