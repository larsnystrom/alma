#include "TickCounter.h"

using namespace ikaros;

void
TickCounter::Init()
{
    count = GetOutputArray("COUNT");
}



void
TickCounter::Tick()
{
    count[0] = count[0] + 1;
}



static InitClass init("TickCounter", &TickCounter::Create, "Source/UserModules/TickCounter/");


