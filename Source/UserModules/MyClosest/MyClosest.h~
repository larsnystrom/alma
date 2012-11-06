#ifndef MyClosest_
#define MyClosest_

#include "IKAROS.h"

class MyClosest: public Module
{
 public:

  int size_x;
  int size_y;
  float * dist;
  float * point_position;
  float ** depth;
  
  static Module * Create(Parameter * p) { return new MyClosest(p); }
  
 MyClosest(Parameter * p) : Module(p) {}
  virtual ~MyClosest() {}
  
  void 		Init();
  void 		Tick();
};

#endif

