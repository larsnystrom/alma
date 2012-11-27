//
//	MarkerMemory.h		This file is a part of the IKAROS project
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


#ifndef MarkerMemory_
#define MarkerMemory_

#include "IKAROS.h"

class MarkerMemory: public Module
{
public:
    float **    input;    
    float **    memory;
    int         maxMarkers;
    
    static Module * Create(Parameter * p) { return new MarkerMemory(p); }

    MarkerMemory(Parameter * p) : Module(p) {}
    virtual ~MarkerMemory() {}

    void 		Init();
    void 		Tick();
private:
    int lastIndex;
    void aCpy(float* a, float* b);
};

#endif

