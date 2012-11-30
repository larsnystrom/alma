#include "FingerPose.h"
#include <math.h>
using namespace ikaros;

void
FingerPose::Init()
{
  depth = GetInputMatrix("DEPTH");
  pv = GetOutputMatrix("POINT_VECTOR");
  hp = GetInputArray("HAND_POSITION");
  vp1 = GetOutputArray("VP1");
  vp2 = GetOutputArray("VP2");
}

void
FingerPose::Tick()
{
  pv[0][0] = hp[0];
  pv[0][1] = hp[1];
  pv[0][2] = hp[2];
  //pv[0][2] = hp[23]/1000;

  float fl = 0.15;
  float r = fl/((hp[2])*tan(0.4974));
  //float r = fl/((hp[23]/1000)*tan(0.4974));

  int x1 = max(0, (int)((hp[0]-r)*640+0.5));
  int x2 = min(640, (int)((hp[0]+r)*640+0.5));
  int y1 = max(0, (int)((hp[1]-r)*480+0.5));
  int y2 = min(480, (int)((hp[1]+r)*480+0.5));

  pv[1][2]=5.0;
  for(int j=y1; j<y2; j++){
    for(int i=x1; i<x2; i++){
      if((depth[j][i])/1000<pv[1][2] && (depth[j][i])/1000>0.0){
	pv[1][0] = (float)(i)/640.f;
	pv[1][1] = (float)(j)/480.f;
	pv[1][2] = (depth[j][i])/1000;
      }
    }
  }
  vp1[0] = pv[0][0];
  vp1[1] = pv[0][1];
  vp2[0] = pv[1][0];
  vp2[1] = pv[1][1];
}



static InitClass init("FingerPose", &FingerPose::Create, "Source/UserModules/FingerPose/");


