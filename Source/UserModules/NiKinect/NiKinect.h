#ifndef NiKinect_
#define NiKinect_

#include "IKAROS.h"
#include 

class NiKinect: public Module {
public:
    
    float **		depth;
    cv::Mat         im3D;

    static Module * Create(Parameter * p) {return new NiKinect(p);};
    
    NiKinect(Parameter * p) : Module(p) {};
    virtual ~NiKinect();

    void    Init();
    void    Tick();
};

#endif
