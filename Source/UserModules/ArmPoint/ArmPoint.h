#ifndef ArmPoint_
#define ArmPoint_

#include "IKAROS.h"

class ArmPoint: public Module
{
 public:

  float * motor_position;
  float ** depth;
  float * PP;
  float d;
  int size_x;
  int size_y;
  float h;
  float hv;

  static Module * Create(Parameter * p) { return new ArmPoint(p); }

 ArmPoint(Parameter * p) : Module(p) {}
  virtual ~ArmPoint() {}

  void 		Init();
  void 		Tick();
};

#endif
