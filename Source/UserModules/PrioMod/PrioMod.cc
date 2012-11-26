#include "PrioMod.h"
#include <math.h>

using namespace ikaros;

float
PrioMod::dist(float x1, float y1, float z1, float x2, float y2, float z2){
  float d;

  d = pow((x1-x2),2) + pow((y1-y2),2) + pow((z1-z2),2);
  d = sqrt(d);

  return d;
}

void
PrioMod::Init(){
  markers = GetInputMatrix("ATTENDABLES");
  maxMarkers = 10;
  clocam = GetInputArray("CLOSEST");
  attended = GetOutputArray("ATTENDED");
}

void
PrioMod::Tick(){
  float oldClo = 1;
  for(int i=0; i < maxMarkers; ++i){
    if(dist(markers[i][0], markers[i][1], 0, clocam[0], clocam[1], 0)<oldClo){
      attended[0]=markers[i][0];
      attended[1]=markers[i][1];
      attended[2]=markers[i][23]/1000;
      oldClo=dist(markers[i][0], markers[i][1], 0, clocam[0], clocam[1], 0);
    }
  }
}

static InitClass init("PrioMod", &PrioMod::Create, "Source/UserModules/PrioMod/");
