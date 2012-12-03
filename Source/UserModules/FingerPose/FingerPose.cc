#include "FingerPose.h"
#include <math.h>
using namespace ikaros;

void
FingerPose::Init()
{
  depth = GetInputMatrix("DEPTH");
  pvh = GetOutputArray("POINT_VECTOR_HAND");
  pvf = GetOutputArray("POINT_VECTOR_FINGER");
  hp = GetInputArray("HAND_POSITION");
  vp1 = GetOutputArray("VP1");
  vp2 = GetOutputArray("VP2");
}

void
FingerPose::Tick()
{
    if (hp[0] < 0.1f) {
        pvh[0] = 0.0f;
        pvh[1] = 0.0f;
        pvh[2] = 0.0f;
        
        pvf[0] = 0.0f;
        pvf[1] = 0.0f;
        pvf[2] = 0.0f;
    } else {
        pvh[0] = hp[0];
        pvh[1] = hp[1];
        pvh[2] = hp[2];
        //pvh[2] = hp[23]/1000;

        float fl = 0.10;
        float r = fl/((hp[2])*tan(0.4974));
        //float r = fl/((hp[23]/1000)*tan(0.4974));

        int x1 = max(0, (int)((hp[0]-r)*640+0.5));
        int x2 = min(640, (int)((hp[0]+r)*640+0.5));
        int y1 = max(0, (int)((hp[1]-r)*480+0.5));
        int y2 = min(480, (int)((hp[1]+r)*480+0.5));

        pvf[2]=5.f;
        for(int j=y1; j<y2; j++){
            for(int i=x1; i<x2; i++){
                if((depth[j][i])/1000<pvf[2] && (depth[j][i])/1000>0.0){
                    pvf[0] = (float)(i)/640.f;
                    pvf[1] = (float)(j)/480.f;
                    pvf[2] = (depth[j][i])/1000;
                }
            }
        }
    }
    
    vp1[0] = pvh[0];
    vp1[1] = pvh[1];
    vp2[0] = pvf[0];
    vp2[1] = pvf[1];
}



static InitClass init("FingerPose", &FingerPose::Create, "Source/UserModules/FingerPose/");


