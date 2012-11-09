#include "EyeDir.h"

using namespace ikaros;

void
EyeDir::Init()
{
  le =  GetInputMatrix("left_eye");
  re =  GetInputMatrix("right_eye");

  coll1 = GetOutputArray("COLL1");
  coll2 = GetOutputArray("COLL2");
  stdl1 = GetOutputArray("STDL1");
  stdl2 = GetOutputArray("STDL2");

  colr1 = GetOutputArray("COLR1");
  colr2 = GetOutputArray("COLR2");
  stdr1 = GetOutputArray("STDR1");
  stdr2 = GetOutputArray("STDR2");
  //dir = GetOutputArray("Dir");
}

void
EyeDir::Tick()
{
  int inter = 7;
  int cli1;
  int cli2;
  int cri1;
  int cri2;
  float clt1 = 0;
  float clt2 = 1;
  float crt1 = 0;
  float crt2 = 1;
  float teval;
  float tevar;
  for(int i=0; i<50; ++i){
    coll1[i] = 0;
    coll2[i] = 0;

    colr1[i] = 0;
    colr2[i] = 0;
  }

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

  for(int i=0; i<50; ++i){
    if(stdl1[i]>clt1){
      cli1 = i;
      clt1 = stdl1[i];
    }
    if(stdr1[i]>crt1){
      cri1 = i;
      crt1 = stdr1[i];
    }
  }
  /*
  for(int i=(cli1-3); i<(cli1+3); ++i){
    if(coll[i]<clt2){
      cli2 = i;
      clt2 = coll[i];
    }
  }
  */
  if(cli1>=inter && cri1>=inter && cli1<=(50-inter) && cri1<=(50-inter)){
    for(int i=0; i<50; ++i){
      for(int j=(cli1-inter); j<(cli1+inter); ++j){
	coll2[i] += le[j][i];
      }
      coll2[i]/=10;
      for(int j=(cri1-inter); j<(cri1+inter); ++j){
	colr2[i] += re[j][i];
      }
      colr2[i]/=10;
    }
    for(int i=0; i<50; ++i){
      teval = 0;
      tevar = 0;
      for(int j=(cli1-inter); j<(cli1+inter); ++j){
	teval += pow((le[j][i] - coll2[i]), 2);
      }
      stdl2[i] = sqrt(teval/10);
      for(int j=(cri1-inter); j<(cri1+inter); ++j){
	tevar += pow((re[j][i] - colr2[i]), 2);
      }
      stdr2[i] = sqrt(tevar/10);
    }
  }

  /*
  int cli;
  int cri;
  float clt = 0;
  float crt = 0;
  dir[1] = 0.8;
  dir[0] = 0.5;

  for(int i=0; i<30; ++i){
    coll[i] = 0;
    colr[i] = 0;
  }

  for(int i=10; i<40; ++i){
    for(int j=20; j<31; ++j){
      coll[i-10] += le[j][i];
      colr[i-10] += re[j][i];
    }
  }

  for(int i=0; i<30; ++i){
    if(coll[i]>clt){
      cli = i;
      clt = coll[i];
    }
    if(colr[i]>crt){
      cri = i;
      crt = colr[i];
    }
  }

  if(cli>15 && cri>15){
    dir[0] = 0;
  }
  else if(cli<15 && cri<15){
    dir[0] = 1;
  }
  else{
    dir[0] = 0.5;
  }
  */
}

static InitClass init("EyeDir", &EyeDir::Create, "Source/UserModules/EyeDir/");
