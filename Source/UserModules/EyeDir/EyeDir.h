#ifndef EyeDir_
#define EyeDir_

#include "IKAROS.h"

class EyeDir: public Module
{
 public:

  float ** le;
  float ** re;
  //float * dir;
  float * coll1;
  float * colr1;
  float * coll2;
  float * colr2;
  float * stdl1;
  float * stdl2;
  float * stdr1;
  float * stdr2;

  static Module * Create(Parameter * p) { return new EyeDir(p); }

 EyeDir(Parameter * p) : Module(p) {}
  virtual ~EyeDir() {}

  void 		Init();
  void 		Tick();
};

#endif

