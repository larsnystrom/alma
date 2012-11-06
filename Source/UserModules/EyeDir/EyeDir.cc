#include "EyeDir.h"

using namespace ikaros;

void
EyeDir::Init()
{
  le =  GetInputMatrix("left_eye");
  re =  GetInputMatrix("right_eye");
  size_x = GetInputSizeX("left_eye");//50
  size_y = GetInputSizeY("left_eye");//50
  dir = GetOutputArray("Dir");
}

void
EyeDir::Tick()
{
  int dirt[1];

  //Left eye
  float t0=0;
  float t1=0;
  float t2=0;
  for(int i=0; i<size_x/3; i++){
    for(int j=0; j<size_y; j++){
      t0=t0+le[j][i];
    }
  }
  for(int i=0; i<size_x/3; i++){
    for(int j=0; j<size_y; j++){
      t1=t1+le[j][(int)(i+size_x/3)];
    }
  }
  for(int i=0; i<size_x/3; i++){
    for(int j=0; j<size_y; j++){
      t2=t2+le[j][(int)(i+2*size_x/3)];
    }
  }
  if(t0<t1 && t0<t2){
    dirt[0]=1;
  }
  else if(t2<t0 && t2<t1){
    dirt[0]=3;
  }
  else{
    dirt[0]=2;
  }

  //Right eye
  t0=0;
  t1=0;
  t2=0;
  for(int i=0; i<size_x/3; i++){
    for(int j=0; j<size_y; j++){
      t0=t0+re[j][i];
    }
  }
  for(int i=0; i<size_x/3; i++){
    for(int j=0; j<size_y; j++){
      t1=t1+re[j][(int)(i+size_x/3)];
    }
  }
  for(int i=0; i<size_x/3; i++){
    for(int j=0; j<size_y; j++){
      t2=t2+re[j][(int)(i+2*size_x/3)];
    }
  }
  if(t0<t1 && t0<t2){
    dirt[1]=1;
  }
  else if(t2<t0 && t2<t1){
    dirt[1]=3;
  }
  else{
    dirt[1]=2;
  }

  //Combining the eyes
  if(dirt[0]/dirt[1]==1){
    dir[0]=dirt[0];
  }
  else if(dirt[1]==2){
    dir[0]=dirt[0];
  }
  else if(dirt[0]==2){
    dir[0]=dirt[1];
  }
  else{
    dir[0]=2;
  }
}

static InitClass init("EyeDir", &EyeDir::Create, "Source/UserModules/EyeDir/");
