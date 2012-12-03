//
//	HandPoint.h		This file is a part of the IKAROS project
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


#ifndef HandPoint_
#define HandPoint_

#include "IKAROS.h"
#include "./../OpenNi/OpenNi.h"
#include "NiHandTracker.h"
#include <XnCppWrapper.h>

class HandPoint: public Module
{
public:
    float *     handPoint;
    float *     handPointView;
    float *     waveFound;
    static Module * Create(Parameter * p) { return new HandPoint(p); }

    HandPoint(Parameter * p) : Module(p) 
    {
        OpenNi& on = OpenNi::getInstance();
        niContext = on.getContext();
        
        m_HandTracker = new HandTracker(niContext);
        
        niRetVal = m_HandTracker->Init();
        
    }
    
    virtual ~HandPoint() 
    {
        delete m_HandTracker;
    }

    void 		Init();
    void 		Tick();
    
private: 
    float xCon(float xi, float zi);
    float yCon(float yi, float zi);
    
    xn::Context     niContext;
    HandTracker*    m_HandTracker;
    XnStatus        niRetVal;
};

#endif

