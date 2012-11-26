#ifndef HeadPose_
#define HeadPose_

#include "IKAROS.h"

class HeadPose: public Module
{
public:
    static Module * Create(Parameter * p) { return new HeadPose(p); }

    HeadPose(Parameter * p) : Module(p) {}
    virtual ~HeadPose() {}

    void 		Init();
    void 		Tick();
};

#endif

