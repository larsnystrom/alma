#ifndef FingerPose_
#define FingerPose_

#include "IKAROS.h"

class FingerPose: public Module
{
 public:
  static Module * Create(Parameter * p) { return new FingerPose(p); }

 FingerPose(Parameter * p) : Module(p) {}
  virtual ~FingerPose() {}

  float ** pv;
  float ** depth;
  float * hp;

  float * vp1;
  float * vp2;

  void 		Init();
  void 		Tick();
};

#endif

