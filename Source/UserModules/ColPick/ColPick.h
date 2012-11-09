#ifndef ColPick_
#define ColPick_

#include "IKAROS.h"

class ColPick: public Module
{
 public:
  static Module * Create(Parameter * p) { return new ColPick(p); }

 ColPick(Parameter * p) : Module(p) {}
  virtual ~ColPick() {}

  float ** pict;
  float * col;
  float * stdd;

  void Init();
  void Tick();

private:
  int mode;
  int uLimit;
  int lLimit;

};

#endif

