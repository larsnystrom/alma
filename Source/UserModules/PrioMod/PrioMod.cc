#include "PrioMod.h"
#include <math.h>

using namespace ikaros;

float
dist(float x1, float y1, float x2, float y2){
  float d;

  d = pow((x1-x2),2) + pow((y1-y2),2);
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
    if(dist(markers[i][0], markers[i][1], clocam[0], clocam[1])<oldClo){
      attended[0]=markers[i][0];
      attended[1]=markers[i][1];
      attended[2]=markers[i][23];
      oldClo=dist(markers[i][0], markers[i][1], clocam[0], clocam[1]);
    }
  }
}

static InitClass init("PrioMod", &PrioMod::Create, "Source/UserModules/PrioMod/");
