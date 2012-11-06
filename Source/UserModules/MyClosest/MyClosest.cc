#include "MyClosest.h"
#include <math.h>

using namespace ikaros;

void
MyClosest::Init()
{

  depth =  GetInputMatrix("depth");
  size_x = GetInputSizeX("depth");
  size_y = GetInputSizeY("depth"); 
  dist = GetOutputArray("CloseDist");
  point_position = GetOutputArray("ClosePosition");

}

void
MyClosest::Tick()
{
  float sm = depth[0][0];
  for(int j=0; j<size_y; j++){
    for(int i=0; i<size_x; i++){
      if(depth[j][i]<sm){
	sm = depth[j][i];
	point_position[0] = i;
	point_position[1] = j;
      }
    }
  }
  dist[0]=sm;
  point_position[0]=point_position[0]/size_x;
  point_position[1]=point_position[1]/size_y;
}

static InitClass init("MyClosest", &MyClosest::Create, "Source/UserModules/MyClosest/");
