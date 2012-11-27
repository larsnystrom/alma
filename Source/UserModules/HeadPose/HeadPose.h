#ifndef HeadPose_
#define HeadPose_

#include "IKAROS.h"
#include <XnCppWrapper.h>
#include "CRForestEstimator.h"
#include "rigid_motion.hpp"

class HeadPose: public Module
{
public:
    float* out_head_center;
    float* out_head_front;

    static Module * Create(Parameter * p) { return new HeadPose(p); }

    HeadPose(Parameter * p) : Module(p) 
    {
        hpMaxZ = 0;
        hpTh = 400;
        hpProbTh = 1.0f;
        hpMaxV = 1000.f;
        hpStride = 5;
        hpLargerRadiusRatio = 1.f;
        hpSmallerRadiusRatio = 6.f;
        
        hpImW = 640;
        hpImH = 480;
        hpFps = 30;
    }
    virtual ~HeadPose() {}

    void 		Init();
    void 		Tick();
private:
    XnUInt64            xnFocalLength;
    XnDouble            xnPixelSize;
    xn::Context         xnContext;
    xn::DepthGenerator  xnDepthGenerator;
    xn::DepthMetaData   xnDepthMD;
    XnStatus            xnRetVal;
    
    CRForestEstimator*  hpEstimator;
    
    
    // Path to trees
    std::string         hpTreepath;
    // Number of trees
    int                 hpNTrees;
    // Patch width
    int                 hpPWidth;
    // Patch height
    int                 hpPHeight;
    //maximum distance form the sensor - used to segment the person
    int                 hpMaxZ;
    //head threshold - to classify a cluster of votes as a head
    int                 hpTh;
    //threshold for the probability of a patch to belong to a head
    float               hpProbTh;
    //threshold on the variance of the leaves
    float               hpMaxV;
    //stride (how densely to sample test patches - increase for higher speed)
    int                 hpStride;
    //radius used for clustering votes into possible heads
    float               hpLargerRadiusRatio;
    //radius used for mean shift
    float               hpSmallerRadiusRatio;
    
    
    //input 3D image
    cv::Mat             hpIm3D;
    //input image size
    int                 hpImW;
    int                 hpImH;
    //kinect's frame rate
    int                 hpFps;
    
    std::vector< cv::Vec<float,POSE_SIZE> >     hpMeans; //outputs
    std::vector< std::vector< Vote > >          hpClusters; //full clusters of votes
    std::vector< Vote >                         hpVotes; //all votes returned by the forest
};

#endif

