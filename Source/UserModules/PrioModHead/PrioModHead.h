#ifndef PrioModHead_
#define PrioModHead_

#include "IKAROS.h"

class PrioModHead: public Module
{
public:
    static Module * Create(Parameter * p) { return new PrioModHead(p); }
    float dist(float x1, float y1, float z1, float x2, float y2, float z2);
    float linePlaneIntersect(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

    PrioModHead(Parameter * p) : Module(p) {}
    virtual ~PrioModHead() {}

    void Init();
    void Tick();

    int maxMarkers;

    float ** markers;

    float * clocam;
    float * attended;
    float * head_center;
    float * head_front;
};

#endif

