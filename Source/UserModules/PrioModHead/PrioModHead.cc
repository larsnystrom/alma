#include "PrioModHead.h"
#include <math.h>

using namespace ikaros;

float
PrioModHead::dist(float x1, float y1, float z1, float x2, float y2, float z2){
  float d;

  d = pow((x1-x2),2) + pow((y1-y2),2) + pow((z1-z2),2);
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
PrioModHead::linePlaneIntersect(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
  float num = x1*((z3/x3)+(z3*y3)/(2*x3*x3)+z3/(2*x3*x3))-y1*(z3/(2*x3))-z1-z3/(2*x3);
  float denom = z2-z1-(x2-x1)*((z3/x3)+(z3*y3)/(2*x3*x3)+z3/(2*x3*x3))+(y2-y1)*(z3/(2*x3));
  float dl = dist((x1+(num/denom)*(x2-x1)), (y1+(num/denom)*(y2-y1)), (z1+(num/denom)*(z2-z1)), x3, y3, z3);
  return dl;
}

void
PrioModHead::Init(){
  markers = GetInputMatrix("ATTENDABLES");
  maxMarkers = 10;
//  clocam = GetInputArray("CLOSEST");
  attended = GetOutputArray("ATTENDED");
  
  head_center = GetInputArray("HEAD_CENTER");
  head_front = GetInputArray("HEAD_FRONT");
}

void
PrioModHead::Tick(){
//  float oldClo = 1;
//  for(int i=0; i < maxMarkers; ++i){
//    if(dist(markers[i][0], markers[i][1], 0, clocam[0], clocam[1], 0)<oldClo){
//      attended[0]=markers[i][0];
//      attended[1]=markers[i][1];
//      attended[2]=markers[i][23]/1000;
//      oldClo=dist(markers[i][0], markers[i][1], 0, clocam[0], clocam[1], 0);
//    }
//  }
  
  float curDist = 0.0;
  float minDist = 50000.0;
  int prioMarker = 0;
    for (int i = 0; i < maxMarkers; ++i) {
        curDist = linePlaneIntersect(
            head_center[0], head_center[1], head_center[2], 
            head_front[0], head_front[1], head_front[2],
            markers[i][0], markers[i][1], markers[i][23]/1000
        );
        
        if (curDist < minDist) {
            prioMarker = i;
            minDist = curDist;
        }
    }
  
    attended[0] = markers[prioMarker][0];
    attended[1] = markers[prioMarker][1];
    attended[2] = markers[prioMarker][23]/1000;
  
//    printf(
//        "Marker (x, y, z): (%f, %f, %f)", 
//        markers[prioMarker][0], markers[prioMarker][1], markers[prioMarker][23]/1000
//    );
  
}

static InitClass init("PrioModHead", &PrioModHead::Create, "Source/UserModules/PrioModHead/");
