#include "SetKinect.h"

extern "C" {
#include "libfreenect_sync.h"
}

using namespace ikaros;

void
SetKinect::Init()
{
  TILT = GetInputArray("TILT");
  LED = GetInputArray("LED");
}

void
SetKinect::Tick()
{
  int tilt = (int) TILT[0];
  freenect_led_options led = (freenect_led_options) LED[0];
  freenect_sync_set_led(led,0);
  freenect_sync_set_tilt_degs(tilt,0);
}

static InitClass init("SetKinect", &SetKinect::Create, "Source/UserModules/SetKinect/");
