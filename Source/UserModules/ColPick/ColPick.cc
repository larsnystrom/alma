#include "ColPick.h"
#include <math.h>

using namespace ikaros;

void
ColPick::Init()
{
  pict = GetInputMatrix("PICT");
  col = GetOutputArray("COL");
  stdd = GetOutputArray("STDD");

  uLimit = GetIntValue("uLimit");
  lLimit = GetIntValue("lLimit");
  mode = GetIntValue("mode");
}



void
ColPick::Tick()
{
  float teva;
  for(int i=0; i<50; ++i){
    col[i] = 0;
    stdd[i];
  }


  if(mode==0){
    for(int i=0; i<50; ++i){
      for(int j=lLimit; j<uLimit+1; ++j){
	col[i] += pict[i][j];
      }
      col[i]/=(1+uLimit-lLimit);
    }
    for(int i=0; i<50; ++i){
      teva = 0;
      for(int j=lLimit; j<uLimit+1; ++j){
	teva += pow((pict[i][j] - col[i]), 2);
      }
      stdd[i] = sqrt(teva/(1+uLimit-lLimit));
    }
  }


  else if(mode==1){
    for(int i=0; i<50; ++i){
      for(int j=lLimit; j<uLimit+1; ++j){
	col[i] += pict[j][i];
      }
      col[i]/=(1+uLimit-lLimit);
    }
    for(int i=0; i<50; ++i){
      teva = 0;
      for(int j=lLimit; j<uLimit+1; ++j){
	teva += pow((pict[j][i] - col[i]), 2);
      }
      stdd[i] = sqrt(teva/(1+uLimit-lLimit));
    }
  }
}



static InitClass init("ColPick", &ColPick::Create, "Source/UserModules/ColPick/");


