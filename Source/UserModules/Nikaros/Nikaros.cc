//
//	Nikaros.cc		This file is a part of the IKAROS project
//
//    Copyright (C) 2012 <Author Name>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    See http://www.ikaros-project.org/ for more information.
//


#include "Nikaros.h"

using namespace ikaros;


void
Nikaros::Init()
{
    // START GENERATING
    OpenNi& on = OpenNi::getInstance();
    on.StartGeneratingAll();
    
    // GET OUTPUT
    intensity   = GetOutputMatrix("INTENSITY");
    red			= GetOutputMatrix("RED");
    green		= GetOutputMatrix("GREEN");
    blue		= GetOutputMatrix("BLUE");
    depth       = GetOutputMatrix("DEPTH");
    depthNorm    = GetOutputMatrix("DEPTH_NORM");
}



void
Nikaros::Tick()
{
    // Wait for new data to be available
    niRetVal = niContext.WaitNoneUpdateAll();
    if (niRetVal != XN_STATUS_OK)
    {
	    printf("Failed updating data: %s\n", xnGetStatusString(niRetVal));
	    return;
    }
    
    // Take current RGB map
    niImageGenerator.GetMetaData(niImageMD);
    
    const XnRGB24Pixel* pImageSingleData = niImageMD.RGB24Data();
    
    const float c1255 = 1.0f / 255.0f;
    
    for (XnUInt j=0; j<niImageMD.YRes();j++){
        for (XnUInt i=0; i<niImageMD.XRes();i++,  pImageSingleData++){
            red[j][i] = c1255 * static_cast<float>(pImageSingleData->nRed);
            green[j][i] = c1255 * static_cast<float>(pImageSingleData->nGreen);
            blue[j][i] = c1255 * static_cast<float>(pImageSingleData->nBlue);
            
            intensity[j][i] = (red[j][i] + green[j][i] + blue[j][i]) / 3.0f;
            
//            printf(
//                "%f %f %f\n", red[j][i], green[j][i], blue[j][i]
//            );
        }
//        printf("\n");
    }
    
    // Take current depth map
    niDepthGenerator.GetMetaData(niDepthMD);
    
    for (int y = 0; y < niDepthYRes; y++) {
	    for (int x = 0; x < niDepthXRes; x++) {
	        depth[y][x] = float(niDepthMD(x,y));
	        depthNorm[y][x] = depth[y][x] / 10000.f;
	    }
    }
}

static InitClass init("Nikaros", &Nikaros::Create, "Source/UserModules/Nikaros/");


