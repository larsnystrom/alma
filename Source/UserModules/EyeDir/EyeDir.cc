#include "EyeDir.h"
using namespace ikaros;

void
EyeDir::Init()
{
  le =  GetInputMatrix("left_eye");
  re =  GetInputMatrix("right_eye");

  leo =  GetOutputMatrix("left_eye_out");
  reo =  GetOutputMatrix("right_eye_out");

  coll1 = GetOutputArray("COLL1");
  coll2 = GetOutputArray("COLL2");
  coll3 = GetOutputArray("COLL3");
  stdl1 = GetOutputArray("STDL1");
  stdl3 = GetOutputArray("STDL3");

  colr1 = GetOutputArray("COLR1");
  colr2 = GetOutputArray("COLR2");
  colr3 = GetOutputArray("COLR3");
  stdr1 = GetOutputArray("STDR1");
  stdr3 = GetOutputArray("STDR3");

  dir = GetOutputArray("Dir");

  lem =  GetOutputMatrix("left_eye_marked");
  rem =  GetOutputMatrix("right_eye_marked");
}

void
EyeDir::Tick()
{
  /**
   *Initiating and reseting stuff
   **/
  dir[1] = 0.75f;
  int llc = 0;
  int lrc = 0;
  int rlc = 0;
  int rrc = 0;
  float tempstoll = 0;
  float tempstorr = 0;
  int tempraf = 0;
  int tempras = 0;
  int cli1 = 0;
  int cri1 = 0;
  float clt1 = 0;
  float crt1 = 0;
  float teval = 0;
  float tevar = 0;
  for(int i=0; i<50; ++i){
    coll1[i] = 0;
    coll2[i] = 0;

    colr1[i] = 0;
    colr2[i] = 0;
  }
  for(int i=0; i<50; ++i){
    for(int j=0; j<20; ++j){
      leo[j][i]=0;
      reo[j][i]=0;
    }
  }

  /**
   *Averaging the middle of the image
   **/
  for(int i=0; i<50; ++i){
    for(int j=20; j<31; ++j){
      coll1[i] += le[i][j];
      colr1[i] += re[i][j];
    }
    coll1[i]/=11;
    colr1[i]/=11;
  }
  for(int i=0; i<50; ++i){
    teval = 0;
    tevar = 0;
    for(int j=20; j<31; ++j){
      teval += pow((le[i][j] - coll1[i]), 2);
      tevar += pow((re[i][j] - colr1[i]), 2);
    }
    stdl1[i] = sqrt(teval/11);
    stdr1[i] = sqrt(tevar/11);
  }

  /**
   *Finding the pupil
   **/
  for(int i=15; i<50; ++i){
    if(stdl1[i]>clt1){
      cli1 = i;
      clt1 = stdl1[i];
    }
    if(stdr1[i]>crt1){
      cri1 = i;
      crt1 = stdr1[i];
    }
  }

  /**
   *Draws output markers on new images (redundant)
   **/
  for(int i=0; i<50; ++i){
    for(int j=0; j<50; ++j){
      lem[i][j] = le[i][j];
      rem[i][j] = re[i][j];
    }
  }
  for(int i=0; i<50; ++i){
    if(cli1 < 50 && cli1 > 0){
      lem[cli1][i] = 1;
    }
    if(cri1 < 50 && cri1 > 0){
      rem[cri1][i] = 1;
    }
  }

  /**
   *Cropping the image
   **/
  if(cli1 < 40 && cli1 > 10){
    for(int i=0; i<50; ++i){
      for(int j=(cli1-10); j<(cli1+10); ++j){
	leo[j+10-cli1][i] = le[j][i];
      }
    }
  }
  if(cri1 < 40 && cri1 > 10){
    for(int i=0; i<50; ++i){
      for(int j=(cri1-10); j<(cri1+10); ++j){
	reo[j+10-cri1][i] = re[j][i];
      }
    }
  }

  /**
   *Averaging the new picture
   **/
  for(int i=0; i<50; ++i){
    for(int j=0; j<20; ++j){
      coll3[i] += leo[j][i];
      colr3[i] += reo[j][i];
    }
    coll3[i]/=20;
    colr3[i]/=20;
  }
  for(int i=0; i<50; ++i){
    teval = 0;
    tevar = 0;
    for(int j=0; j<20; ++j){
      teval += pow((leo[j][i] - coll3[i]), 2);
      tevar += pow((reo[j][i] - colr3[i]), 2);
    }
    stdl3[i] = sqrt(teval/20);
    stdr3[i] = sqrt(tevar/20);
  }

  /**
   *Finding the corners of the eyes
   **/
  for(int i=0; i<50; ++i){
    for(int j=20; j<31; ++j){
      coll2[i] += le[j][i];
      colr2[i] += re[j][i];
    }
    coll2[i]/=11;
    colr2[i]/=11;
  }
  for(int i=3; i<49; ++i){
    if(coll2[i]>0 && coll2[i]<coll2[i+1]){
      llc = i;
      break;
    }
  }
  for(int i=3; i<49; ++i){
    if(colr2[i]>0 && colr2[i]<colr2[i+1]){
      rlc = i;
      break;
    }
  }
  for(int i=47; i>1; --i){
    if(coll2[i]>0 && coll2[i]<coll2[i-1]){
      lrc = i;
      break;
    }
  }
  for(int i=47; i>1; --i){
    if(colr2[i]>0 && colr2[i]<colr2[i-1]){
      rrc = i;
      break;
    }
  }

  /**
   *Finding the gaze-direction
   **/
  for(int i=llc; i<lrc; ++i){
    if(i-llc<(lrc+1-llc)/2){
      tempstoll += coll2[i];
      if(i-llc+0.6>(lrc-llc)/2){
	tempstoll -= coll2[i];
	tempraf = i;
      }
    }
    else{
      tempstoll -= coll2[i];
    }
  }
  for(int i=rlc; i<rrc; ++i){
    if(i-rlc<(rrc+1-rlc)/2){
      tempstorr += colr2[i];
      if(i-rlc+0.6>(rrc-rlc)/2){
	tempstorr -= colr2[i];
	tempras = i;
      }
    }
    else{
      tempstorr -= colr2[i];
    }
  }
  if(tempstoll>0 && tempstorr>0){
    dir[0] = 1.0f;
    printf("Right\n");
  }
  else if(tempstoll<0 && tempstorr<0){
    dir[0] = 0.0f;
    printf("Left\n");
  }
  else{
    dir[0] = 0.5f;
    printf("Center\n");
  }
}

static InitClass init("EyeDir", &EyeDir::Create, "Source/UserModules/EyeDir/");


