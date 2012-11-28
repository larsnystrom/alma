#include "ArmPoint.h"
#include "math.h"

using namespace ikaros;

void
ArmPoint::Init()
{
    depth =  GetInputMatrix("depth");
    size_x = GetInputSizeX("depth");//640
    size_y = GetInputSizeY("depth");//480
    PP =  GetInputArray("POINT_POSITION");
    motor_position = GetOutputArray("Positions");
    h = 0.18f;//Camera position
    hv = 0.21f; //Camera angle = 12 deg == 12/180*3.14 rad
}

void
ArmPoint::Tick()
{
    int x = (int) (PP[0] * size_x);
    int y = (int) (PP[1] * size_y);

    if (PP[2] > 0.0f) {
        d = PP[2];
        printf("ArmPoint: Using input z=%f.\n", d);
    } else if (depth[y][x] < 0.9999 && depth[y][x] > 0){//if raw data from kinect
        d = depth[y][x];
        //d = 1.0 / (d*1023 * -0.0030711016 + 3.3309495161);//Probably not so good
        //d = 0.1236 * tanf(d*1023/2842.5 + 1.1863);
        //d=348/(1091.5-d*1023);
        d = 0.1236 * tanf(d*2047/2842.5 + 1.1863);
        printf("ArmPoint: Using raw z=%f.\n", d);
    } else if (depth[y][x] > 1.0f){//if mm data from kinect
        d = depth[y][x] / 1000.0f;
        printf("ArmPoint: Using mm z=%f.\n", d);
    }

    //d = 0.3;//If the pointables are located 30 cm from the arm

    float v1 = 180;
    float v2 = 180;
    float v3 = 180;

    float th = ((-43)*(PP[1]-0.5)*3.14/180) + hv;

    motor_position[0] = v1-57*(PP[0]-0.5);
    motor_position[1] = v2;
    motor_position[2] = v3-atanf((h-d*sinf(th))/(d*cosf(th)))*180/3.14;

}

static InitClass init("ArmPoint", &ArmPoint::Create, "Source/UserModules/ArmPoint/");
