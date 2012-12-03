//
//	Nikaros.h		This file is a part of the IKAROS project
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


#ifndef Nikaros_
#define Nikaros_

#include "IKAROS.h"
#include <XnCppWrapper.h>
#include "./../OpenNi/OpenNi.h"


class Nikaros: public Module
{
public:
    float **        intensity;
    float **		red;
    float **		green;
    float **		blue;
    float **        depth;
    float **        depthNorm;
    
    static Module * Create(Parameter * p) { return new Nikaros(p); }

    Nikaros(Parameter * p) : Module(p) 
    {
        OpenNi& on = OpenNi::getInstance();
        niContext = on.getContext();
        
        niRetVal = niContext.FindExistingNode(XN_NODE_TYPE_DEPTH, niDepthGenerator);
	    if (niRetVal != XN_STATUS_OK)
	    {
		    printf("No depth node exists! Check your XML.");
	    }

	    niRetVal = niContext.FindExistingNode(XN_NODE_TYPE_IMAGE, niImageGenerator);
	    if (niRetVal != XN_STATUS_OK)
	    {
		    printf("No image node exists! Check your XML.");
	    }
    }
    virtual ~Nikaros() {}

    void 		Init();
    void 		Tick();
    
private:
    xn::Context         niContext; 
    xn::ImageGenerator  niImageGenerator;
    XnStatus            niRetVal;
    xn::ImageMetaData   niImageMD;
    xn::DepthGenerator  niDepthGenerator;
    xn::DepthMetaData   niDepthMD;
    
    static const int niDepthXRes = 640;
    static const int niDepthYRes = 480;
    static const int niDepthFPS = 30;
};

#endif

