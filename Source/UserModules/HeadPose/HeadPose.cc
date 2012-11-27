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

// Path to trees
std::string g_treepath;
// Number of trees
int g_ntrees;
// Patch width
int g_p_width;
// Patch height
int g_p_height;
//maximum distance form the sensor - used to segment the person
int g_max_z = 0;
//head threshold - to classify a cluster of votes as a head
int g_th = 400;
//threshold for the probability of a patch to belong to a head
float g_prob_th = 1.0f;
//threshold on the variance of the leaves
float g_maxv = 1000.f;
//stride (how densely to sample test patches - increase for higher speed)
int g_stride = 5;
//radius used for clustering votes into possible heads
float g_larger_radius_ratio = 1.f;
//radius used for mean shift
float g_smaller_radius_ratio = 6.f;

//pointer to the actual estimator
CRForestEstimator* g_Estimate;
//input 3D image
cv::Mat g_im3D;
//input image size
int g_im_w = 640;
int g_im_h = 480;
//kinect's frame rate
int g_fps = 30;

XnUInt64 g_focal_length;
XnDouble g_pixel_size;

xn::Context g_Context;
xn::DepthGenerator g_DepthGenerator;

xn::DepthMetaData g_depthMD;

XnStatus g_RetVal;

std::vector< cv::Vec<float,POSE_SIZE> > g_means; //outputs
std::vector< std::vector< Vote > > g_clusters; //full clusters of votes
std::vector< Vote > g_votes; //all votes returned by the forest

math_vector_3f g_face_curr_dir, g_face_dir(0,0,-1);





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
//    g_context = OpenNi::niContext;
    OpenNi& on = OpenNi::getInstance();
    g_Context = on.getContext();
    
    
    // Load Config
	g_treepath = "/home/lars/ikaros/alma/Source/UserModules/HeadPose/trees/tree";
	g_ntrees = 10;
	g_maxv = 800;
	g_larger_radius_ratio = 1.6;
	g_smaller_radius_ratio = 5;
	g_stride = 5;
	g_max_z = 1300;
	g_th = 500;

	std::cout << std::endl << "------------------------------------" << std::endl << std::endl;
	std::cout << "Estimation:       " << std::endl;
	std::cout << "Trees:            " << g_ntrees << " " << g_treepath << std::endl;
	std::cout << "Stride:           " << g_stride << std::endl;
	std::cout << "Max Variance:     " << g_maxv << std::endl;
	std::cout << "Max Distance:     " << g_max_z << std::endl;
	std::cout << "Head Threshold:   " << g_th << std::endl;

	std::cout << std::endl << "------------------------------------" << std::endl << std::endl;
	
	
	g_Estimate =  new CRForestEstimator();
	if( !g_Estimate->loadForest(g_treepath.c_str(), g_ntrees) ){

		std::cerr << "could not read forest!" << std::endl;
		exit(-1);
	}

	
    std::cout << "initializing kinect... " << std::endl;

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
    g_RetVal = g_Context.WaitNoneUpdateAll();
    if (g_RetVal != XN_STATUS_OK)
    {
	    printf("Failed updating data: %s\n", xnGetStatusString(g_RetVal));
	    return;
    }

    // Take current depth map
    g_DepthGenerator.GetMetaData(g_depthMD);

    float f = g_focal_length/g_pixel_size;
    int valid_pixels = 0;

    //generate 3D image
    for(int y = 0; y < g_im3D.rows; y++)
    {
	    cv::Vec3f* Mi = g_im3D.ptr<cv::Vec3f>(y);
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
    
    g_means.clear();
    g_votes.clear();
    g_clusters.clear();

    //do the actual estimation
    g_Estimate->estimate( 	g_im3D,
						    g_means,
						    g_clusters,
						    g_votes,
						    g_stride,
						    g_maxv,
						    g_prob_th,
						    g_larger_radius_ratio,
						    g_smaller_radius_ratio,
						    false,
						    g_th
					    );
    
    //draw head poses
    if(g_means.size()>0){

	    float mult = 0.0174532925f;

	    for(unsigned int i=0;i<g_means.size();++i){

		    rigid_motion<float> rm;
		    rm.m_rotation = euler_to_rotation_matrix( mult*g_means[i][3], mult*g_means[i][4], mult*g_means[i][5] );
		    math_vector_3f head_center( g_means[i][0], g_means[i][1], g_means[i][2] );

		    g_face_curr_dir = rm.m_rotation * (g_face_dir);
		    math_vector_3f head_front(head_center + 150.f*g_face_curr_dir);
            
//            printf("[%d]ZPD: %llu \t ZPPS: %f\n", i, g_focal_length, g_pixel_size);
            printf("[%d]head_center: (%f, %f, %f)\n", i, head_center[0], head_center[1], head_center[2]);
            printf("[%d]head_front:  (%f, %f, %f)\n", i, head_front[0], head_front[1], head_front[2]);
            
            if (0 == i) {
	        out_head_center[0] = xCon(head_center[0], head_center[2])/640;
	        out_head_center[1] = yCon(head_center[1], head_center[2])/480;
                out_head_center[2] = head_center[2];
                out_head_front[0] = xCon(head_front[0], head_front[2])/640;
                out_head_front[1] = yCon(head_front[1], head_front[2])/480;
                out_head_front[2] = head_front[2];
		out_head_center2[0] = out_head_center[0]/640;
		out_head_center2[1] = out_head_center[1]/480;
		out_head_front2[0] = out_head_front[0]/640;
		out_head_front2[1] =out_head_front[1]/480;
            }
            printf("[%d]out_head_center: (%f, %f, %f)\n", i, out_head_center[0], out_head_center[1], out_head_center[2]);
            printf("[%d]out_head_front:  (%f, %f, %f)\n\n", i, out_head_front[0], out_head_front[1], out_head_front[2]);
	    }

    }
}



static InitClass init("HeadPose", &HeadPose::Create, "Source/UserModules/HeadPose/");


