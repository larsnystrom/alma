#ifndef SetKinect_
#define SetKinect_

#include "IKAROS.h"

class SetKinect: public Module
{
 public:
  static Module * Create(Parameter * p) { return new SetKinect(p); }

  float * TILT;
  float * LED;

 SetKinect(Parameter * p) : Module(p) {}
  virtual ~SetKinect() {}

  void 		Init();
  void 		Tick();
};
#endif

