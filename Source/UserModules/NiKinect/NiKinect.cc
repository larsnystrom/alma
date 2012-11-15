#include "NiKinect.h"
#include <XnOS.h>
#include <XnCppWrapper.h>
#include <XnCodecIDs.h>

using namespace ikaros;


//input 3D image
cv::Mat g_im3D;
//input image size
int g_im_w = 640;
int g_im_h = 480;
//kinect's frame rate
int g_fps = 30;


xn::Context g_Context;
xn::DepthGenerator g_DepthGenerator;

xn::DepthMetaData g_depthMD;

XnStatus g_RetVal;


NiKinect::~NiKinect()
{
    
}



void
NiKinect::Init()
{
    std::cout << "initializing kinect... " << endl;

	// Initialize context object
	g_RetVal = g_Context.Init();

	g_RetVal = g_DepthGenerator.Create(g_Context);
	if (g_RetVal != XN_STATUS_OK)
		printf("Failed creating DEPTH generator %s\n", xnGetStatusString(g_RetVal));

	XnMapOutputMode outputMode;
	outputMode.nXRes = g_im_w;
	outputMode.nYRes = g_im_h;
	outputMode.nFPS = g_fps;
	g_RetVal = g_DepthGenerator.SetMapOutputMode(outputMode);
	if (g_RetVal != XN_STATUS_OK)
		printf("Failed setting the DEPTH output mode %s\n", xnGetStatusString(g_RetVal));

	g_RetVal = g_Context.StartGeneratingAll();
	if (g_RetVal != XN_STATUS_OK)
		printf("Failed starting generating all %s\n", xnGetStatusString(g_RetVal));

	// get the focal length in mm (ZPS = zero plane distance)
	g_DepthGenerator.GetIntProperty ("ZPD", g_focal_length);
	// get the pixel size in mm ("ZPPS" = pixel size at zero plane)
	g_DepthGenerator.GetRealProperty ("ZPPS", g_pixel_size);
	g_pixel_size *= 2.f;

	g_im3D.create(g_im_h,g_im_w,CV_32FC3);
}



void
NiKinect::Tick()
{
	// Wait for new data to be available
	g_RetVal = g_Context.WaitAndUpdateAll();
	if (g_RetVal != XN_STATUS_OK)
	{
		printf("Failed updating data: %s\n", xnGetStatusString(g_RetVal));
		return false;
	}

	// Take current depth map
	g_DepthGenerator.GetMetaData(g_depthMD);

	float f = g_focal_length/g_pixel_size;
	int valid_pixels = 0;

	//generate 3D image
	for(int y = 0; y < g_im3D.rows; y++)
	{
		Vec3f* Mi = g_im3D.ptr<Vec3f>(y);
		for(int x = 0; x < g_im3D.cols; x++){

			float d = (float)g_depthMD(x,y);

			if ( d < g_max_z && d > 0 ){

				valid_pixels++;

				Mi[x][0] = ( float(d * (x - 320)) / f );
				Mi[x][1] = ( float(d * (y - 240)) / f );
				Mi[x][2] = d;

			}
			else
				Mi[x] = 0;

		}
	}
}


static InitClass init("NiKinect", &NiKinect::Create, "Source/UserModules/NiKinect/");

