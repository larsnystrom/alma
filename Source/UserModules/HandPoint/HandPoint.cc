//
//	HandPoint.cc		This file is a part of the IKAROS project
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


#include "HandPoint.h"

using namespace ikaros;

void
HandPoint::Init()
{
    handPoint = GetOutputArray("HAND_POINT");   
    handPointView = GetOutputArray("HAND_POINT_VIEW");    
    
    OpenNi& on = OpenNi::getInstance();
    on.StartGeneratingAll();
    
    m_HandTracker->Run();
}

float
HandPoint::xCon(float xi, float zi){
  float xo = (320*xi)/(zi*tan(0.4974)) + 320;
  return xo;
}

float
HandPoint::yCon(float yi, float zi){
  float yo = (240*yi)/(zi*tan(0.3752)) + 240;
  return yo;
}


void
HandPoint::Tick()
{
	niRetVal = niContext.WaitAndUpdateAll();
	if (niRetVal != XN_STATUS_OK)
	{
		printf("Read failed: %s\n", xnGetStatusString(niRetVal));
		return;
	}
	
	typedef TrailHistory			History;
	typedef History::ConstIterator	HistoryIterator;
	typedef Trail::ConstIterator	TrailIterator;

	static const float colours[][3] =
	{
		{ 0.5f, 0.5f, 0.5f},
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.5f, 1.0f},
		{ 1.0f, 1.0f, 0.0f},
		{ 1.0f, 0.5f, 0.0f},
		{ 1.0f, 0.0f, 1.0f}
	};
	const TrailHistory&	history = m_HandTracker->GetHistory();

	// History points coordinates buffer
	XnFloat	coordinates[3 * MAX_HAND_TRAIL_LENGTH];

	const HistoryIterator	hend = history.End();
	for(HistoryIterator		hit = history.Begin(); hit != hend; ++hit)
	{
		// Dump the history to local buffer
		const Trail&	trail = hit->Value();
		
		TrailIterator last = trail.Begin();
		XnPoint3D point = *last;
		
		handPoint[0] = xCon(point.X, point.Z) / 640.f;
		handPoint[1] = yCon(-point.Y, point.Z) / 480.f;
		handPoint[2] = point.Z / 1000.f;
		
		handPointView[0] = handPoint[0];
		handPointView[1] = handPoint[1];
		
		printf("HandPoint: (%f, %f, %f)\t\t(%f, %f, %f)\n", point.X, point.Y, point.Z, handPoint[0], handPoint[1], handPoint[2]);
	}
}



static InitClass init("HandPoint", &HandPoint::Create, "Source/UserModules/HandPoint/");


