#ifndef EyeDir_
#define EyeDir_

#include "IKAROS.h"

class EyeDir: public Module
{
 public:
  static Module * Create(Parameter * p) { return new EyeDir(p); }

 EyeDir(Parameter * p) : Module(p) {}
  virtual ~EyeDir() {}

  float ** le;
  float ** re;

  float * dir;

  float ** leo;
  float ** reo;

  float * coll1;
  float * colr1;
  float * coll2;
  float * colr2;
  float * coll3;
  float * colr3;
  float * stdl1;
  float * stdl3;
  float * stdr1;
  float * stdr3;

  float ** lem;
  float ** rem;

  void 		Init();
  void 		Tick();
};

#endif

