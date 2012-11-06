#ifndef EyeDir_
#define EyeDir_

#include "IKAROS.h"

class EyeDir: public Module
{
 public:

  float ** le;
  float ** re;
  float * dir;
  int size_x;
  int size_y;

  static Module * Create(Parameter * p) { return new EyeDir(p); }

 EyeDir(Parameter * p) : Module(p) {}
  virtual ~EyeDir() {}

  void 		Init();
  void 		Tick();
};

#endif

