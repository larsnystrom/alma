#include "HeadPose.h"

#include <math.h>

//#include <string>
//#include <algorithm>
//#include <iostream>
//#include <vector>
//#include "freeglut.h"
//#include <XnOS.h>
#include <XnCppWrapper.h>
//#include <XnCodecIDs.h>

#include "CRForestEstimator.h"

//#include "vector.hpp"
//#include "matrix.hpp"
#include "rigid_motion.hpp"

#include "./../OpenNi/OpenNi.h"
#include <libgen.h> 

math_vector_3f                              hpFaceCurrDir,hpFaceDir(0,0,-1);

/*
// Authors: Gabriele Fanelli, Thibaut Weise, Juergen Gall, BIWI, ETH Zurich
// Email: fanelli@vision.ee.ethz.ch

// You may use, copy, reproduce, and distribute this Software for any
// non-commercial purpose, subject to the restrictions of the
// Microsoft Research Shared Source license agreement ("MSR-SSLA").
// Some purposes which can be non-commercial are teaching, academic
// research, public demonstrations and personal experimentation. You
// may also distribute this Software with books or other teaching
// materials, or publish the Software on websites, that are intended
// to teach the use of the Software for academic or other
// non-commercial purposes.
// You may not use or distribute this Software or any derivative works
// in any form for commercial purposes. Examples of commercial
// purposes would be running business operations, licensing, leasing,
// or selling the Software, distributing the Software for use with
// commercial products, using the Software in the creation or use of
// commercial products or any other activity which purpose is to
// procure a commercial gain to you or others.
// If the Software includes source code or data, you may create
// derivative works of such portions of the Software and distribute
// the modified Software for non-commercial purposes, as provided
// herein.

// THE SOFTWARE COMES "AS IS", WITH NO WARRANTIES. THIS MEANS NO
// EXPRESS, IMPLIED OR STATUTORY WARRANTY, INCLUDING WITHOUT
// LIMITATION, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A
// PARTICULAR PURPOSE, ANY WARRANTY AGAINST INTERFERENCE WITH YOUR
// ENJOYMENT OF THE SOFTWARE OR ANY WARRANTY OF TITLE OR
// NON-INFRINGEMENT. THERE IS NO WARRANTY THAT THIS SOFTWARE WILL
// FULFILL ANY OF YOUR PARTICULAR PURPOSES OR NEEDS. ALSO, YOU MUST
// PASS THIS DISCLAIMER ON WHENEVER YOU DISTRIBUTE THE SOFTWARE OR
// DERIVATIVE WORKS.

// NEITHER MICROSOFT NOR ANY CONTRIBUTOR TO THE SOFTWARE WILL BE
// LIABLE FOR ANY DAMAGES RELATED TO THE SOFTWARE OR THIS MSR-SSLA,
// INCLUDING DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL OR INCIDENTAL
// DAMAGES, TO THE MAXIMUM EXTENT THE LAW PERMITS, NO MATTER WHAT
// LEGAL THEORY IT IS BASED ON. ALSO, YOU MUST PASS THIS LIMITATION OF
// LIABILITY ON WHENEVER YOU DISTRIBUTE THE SOFTWARE OR DERIVATIVE
// WORKS.

// When using this software, please acknowledge the effort that
// went into development by referencing the paper:
//
// Fanelli G., Weise T., Gall J., Van Gool L., Real Time Head Pose Estimation from Consumer Depth Cameras
// 33rd Annual Symposium of the German Association for Pattern Recognition (DAGM'11), 2011

*/

void
HeadPose::Init()
{
    OpenNi& on = OpenNi::getInstance();
    xnContext = on.getContext();
    
    // Load Config
    const char* file = __FILE__;
    char* dFile = strdup(file);
    dFile = strcat(dirname(dFile), "/trees/tree");
    
	hpTreepath = dFile;
	free(dFile);
	
	hpNTrees = 10;
	hpMaxV = 800;
	hpLargerRadiusRatio = 1.6;
	hpSmallerRadiusRatio = 5;
	hpStride = 5;
	hpMaxZ = 1300;
	hpTh = 500;

	std::cout << std::endl << "------------------------------------" << std::endl << std::endl;
	std::cout << "Estimation:       " << std::endl;
	std::cout << "Trees:            " << hpNTrees << " " << hpTreepath << std::endl;
	std::cout << "Stride:           " << hpStride << std::endl;
	std::cout << "Max Variance:     " << hpMaxV << std::endl;
	std::cout << "Max Distance:     " << hpMaxZ << std::endl;
	std::cout << "Head Threshold:   " << hpTh << std::endl;

	std::cout << std::endl << "------------------------------------" << std::endl << std::endl;
	
	
	hpEstimator =  new CRForestEstimator();
	if( !hpEstimator->loadForest(hpTreepath.c_str(), hpNTrees) ){

		std::cerr << "could not read forest!" << std::endl;
		exit(-1);
	}

    std::cout << "initializing kinect... " << std::endl;

    // Initialize context object
    xnRetVal = xnContext.Init();

    xnRetVal = xnDepthGenerator.Create(xnContext);
    if (xnRetVal != XN_STATUS_OK)
	    printf("Failed creating DEPTH generator %s\n", xnGetStatusString(xnRetVal));

    XnMapOutputMode outputMode;
    outputMode.nXRes = hpImW;
    outputMode.nYRes = hpImH;
    outputMode.nFPS = hpFps;
    xnRetVal = xnDepthGenerator.SetMapOutputMode(outputMode);
    if (xnRetVal != XN_STATUS_OK)
	    printf("Failed setting the DEPTH output mode %s\n", xnGetStatusString(xnRetVal));

    xnRetVal = xnContext.StartGeneratingAll();
    if (xnRetVal != XN_STATUS_OK)
	    printf("Failed starting generating all %s\n", xnGetStatusString(xnRetVal));

    // get the focal length in mm (ZPS = zero plane distance)
    xnDepthGenerator.GetIntProperty ("ZPD", xnFocalLength);
    // get the pixel size in mm ("ZPPS" = pixel size at zero plane)
    xnDepthGenerator.GetRealProperty ("ZPPS", xnPixelSize);
    xnPixelSize *= 2.f;

    hpIm3D.create(hpImH,hpImW,CV_32FC3);
    
    out_head_center = GetOutputArray("HEAD_CENTER");
    out_head_front = GetOutputArray("HEAD_FRONT");
    
    out_head_center2 = GetOutputArray("HEAD_CENTER2");
    out_head_front2 = GetOutputArray("HEAD_FRONT2");
}

float
xCon(float xi, float zi){
  float xo = (320*xi)/(zi*tan(0.4974)) + 320;
  return xo;
}

float
yCon(float yi, float zi){
  float yo = (240*yi)/(zi*tan(0.3752)) + 240;
  return yo;
}

void
HeadPose::Tick()
{
    // Wait for new data to be available
    xnRetVal = xnContext.WaitNoneUpdateAll();
    if (xnRetVal != XN_STATUS_OK)
    {
	    printf("Failed updating data: %s\n", xnGetStatusString(xnRetVal));
	    return;
    }

    // Take current depth map
    xnDepthGenerator.GetMetaData(xnDepthMD);

    float f = xnFocalLength/xnPixelSize;
    int valid_pixels = 0;

    //generate 3D image
    for(int y = 0; y < hpIm3D.rows; y++)
    {
	    cv::Vec3f* Mi = hpIm3D.ptr<cv::Vec3f>(y);
	    for(int x = 0; x < hpIm3D.cols; x++){

		    float d = (float)xnDepthMD(x,y);

		    if ( d < hpMaxZ && d > 0 ){

			    valid_pixels++;

			    Mi[x][0] = ( float(d * (x - 320)) / f );
			    Mi[x][1] = ( float(d * (y - 240)) / f );
			    Mi[x][2] = d;

		    }
		    else
			    Mi[x] = 0;

	    }
    }
    
    hpMeans.clear();
    hpVotes.clear();
    hpClusters.clear();

    //do the actual estimation
    hpEstimator->estimate( 	hpIm3D,
						    hpMeans,
						    hpClusters,
						    hpVotes,
						    hpStride,
						    hpMaxV,
						    hpProbTh,
						    hpLargerRadiusRatio,
						    hpSmallerRadiusRatio,
						    false,
						    hpTh
					    );
    
    //draw head poses
    if(hpMeans.size()>0){

	    float mult = 0.0174532925f;

	    for(unsigned int i=0;i<hpMeans.size();++i){

		    rigid_motion<float> rm;
		    rm.m_rotation = euler_to_rotation_matrix( mult*hpMeans[i][3], mult*hpMeans[i][4], mult*hpMeans[i][5] );
		    math_vector_3f head_center( hpMeans[i][0], hpMeans[i][1], hpMeans[i][2] );

		    hpFaceCurrDir = rm.m_rotation * (hpFaceDir);
		    math_vector_3f head_front(head_center + 150.f*hpFaceCurrDir);
            
//            printf("[%d]ZPD: %llu \t ZPPS: %f\n", i, g_focal_length, g_pixel_size);
//            printf("[%d]head_center: (%f, %f, %f)\n", i, head_center[0], head_center[1], head_center[2]);
//            printf("[%d]head_front:  (%f, %f, %f)\n", i, head_front[0], head_front[1], head_front[2]);
            
            if (0 == i) {
                out_head_center[0] = xCon(head_center[0], head_center[2])/640;
                out_head_center[1] = yCon(head_center[1], head_center[2])/480;
                out_head_center[2] = head_center[2];
                out_head_front[0] = xCon(head_front[0], head_front[2])/640;
                out_head_front[1] = yCon(head_front[1], head_front[2])/480;
                out_head_front[2] = head_front[2];
                out_head_center2[0] = out_head_center[0];
                out_head_center2[1] = out_head_center[1];
                out_head_front2[0] = out_head_front[0];
                out_head_front2[1] =out_head_front[1];
            }
	    /*
            printf("[%d]out_head_center: (%f, %f, %f)\n", 
                i, out_head_center[0], out_head_center[1], out_head_center[2]
            );
            printf("[%d]out_head_front:  (%f, %f, %f)\n", 
                i, out_head_front[0], out_head_front[1], out_head_front[2]
            );
	    */
    }
}



static InitClass init("HeadPose", &HeadPose::Create, "Source/UserModules/HeadPose/");


