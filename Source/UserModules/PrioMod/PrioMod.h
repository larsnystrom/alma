#ifndef PrioMod_
#define PrioMod_

#include "IKAROS.h"

class PrioMod: public Module
{
public:
    static Module * Create(Parameter * p) { return new PrioMod(p); }
    float dist(float x1, float y1, float z1, float x2, float y2, float z2);
    float linePlaneIntersect(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    int attendedMarker(float* center, float* front, float threshold);

    PrioMod(Parameter * p) : Module(p) {}
    virtual ~PrioMod() {}

    void Init();
    void Tick();

    int maxMarkers;

    float ** markers;

    float * clocam;
    float * attended;
    float * head_center;
    float * head_front;
    float * hand_center;
    float * hand_front;
    float * hand_wave;
    float * attendedView;
private:
    float headPoseAttendMaxDist;
    float handPoseAttendMaxDist;
    
    void lookAtDefault();
    void lookAtHead();
    void lookAtHand();
    void lookAtMarker(int markerId);
};

#endif

