#ifndef HeadPose_
#define HeadPose_

#include "IKAROS.h"

class HeadPose: public Module
{
public:
    float* out_head_center;
    float* out_head_front;
    float* out_head_center2;
    float* out_head_front2;

    static Module * Create(Parameter * p) { return new HeadPose(p); }

    HeadPose(Parameter * p) : Module(p) {}
    virtual ~HeadPose() {}

    void 		Init();
    void 		Tick();
};

#endif

