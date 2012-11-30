#include "PrioModHead.h"
#include <math.h>

using namespace ikaros;

float
PrioModHead::dist(float x1, float y1, float z1, float x2, float y2, float z2){
  float d;

  d = pow((x1-x2), 2) + pow((y1-y2), 2) + pow((z1-z2), 2);
  d = sqrt(d);

  return d;
}

/**
 *Ex:
 *1=Head center
 *2=Face center
 *3=AR-tagg location
 **/
float
PrioModHead::linePlaneIntersect(
    float x1, float y1, float z1, 
    float x2, float y2, float z2, 
    float x3, float y3, float z3)
{
    float num = x1 * ( (z3/x3) + (z3*y3)/(2*x3*x3) + z3/(2*x3*x3) ) - y1*( z3/(2*x3) ) - z1 - z3/(2*x3);
    float denom = z2 - z1 - (x2-x1)*( (z3/x3) + (z3*y3)/(2*x3*x3) + z3/(2*x3*x3) ) + (y2-y1)*( z3/(2*x3) );
    float dl = dist( 
        (x1 + (num/denom)*(x2-x1)), 
        (y1 + (num/denom)*(y2-y1)), 
        (z1 + (num/denom)*(z2-z1)), 
        x3, 
        y3, 
        z3
    );
  return dl;
}

void
PrioModHead::Init(){
    maxMarkers = GetIntValue("max_markers");
    headPoseAttendMaxDist = GetFloatValue("HEAD_ATTEND_MAX_DIST");
    
    printf("PrioModHead: HEAD_ATTEND_MAX_DIST=%f\n", headPoseAttendMaxDist);
    
    markers = GetInputMatrix("ATTENDABLES");
    head_center = GetInputArray("HEAD_CENTER");
    head_front = GetInputArray("HEAD_FRONT");
    
    attended = GetOutputArray("ATTENDED");
    attendedView = GetOutputArray("ATTENDED_VIEW");
}

void
PrioModHead::Tick(){
    float curDist = 0.0f;
    float minDist = headPoseAttendMaxDist;
    int prioMarker = -1;
    for (int i = 0; i < maxMarkers; ++i) {
        
        if (markers[i][2] > 0.1f) {
            curDist = linePlaneIntersect(
                head_center[0], head_center[1], head_center[2], 
                head_front[0], head_front[1], head_front[2],
                markers[i][0], markers[i][1], markers[i][23] / 1000.f
            );
            
            if (curDist < minDist) {
                prioMarker = i;
                minDist = curDist;
            }
        }
    }
    if (prioMarker != -1) {
        printf(
            "MarkerAttended[%f] (x, y, z): (%f, %f, %f)\n", 
            markers[prioMarker][2], markers[prioMarker][0], 
            markers[prioMarker][1], markers[prioMarker][23] / 1000.f
        );
        
        attended[0] = markers[prioMarker][0];
        attended[1] = markers[prioMarker][1];
        attended[2] = markers[prioMarker][23] / 1000.f;
    } else {
        attended[0] = head_center[0];
        attended[1] = head_center[1];
        attended[2] = head_center[2];
        printf(
            "MarkerNotAttended: Looking at head(%f, %f, %f)\n",
            attended[0], attended[1], attended[2]
        );
        
//        attended[0] = 0.5f;
//        attended[1] = 0.8f;
//        attended[2] = 2.0f;
//        printf(
//            "MarkerNotAttended: Looking forward(%f, %f, %f)\n",
//            attended[0], attended[1], attended[2]
//        );
    }
    
    attendedView[0] = attended[0];
    attendedView[1] = attended[1];
  
}

static InitClass init("PrioModHead", &PrioModHead::Create, "Source/UserModules/PrioModHead/");
