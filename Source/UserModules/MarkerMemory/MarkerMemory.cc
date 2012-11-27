//
//	MarkerMemory.cc		This file is a part of the IKAROS project
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


#include "MarkerMemory.h"
#include <math.h>

using namespace ikaros;

void
MarkerMemory::Init()
{
    input = GetInputMatrix("MARKERS");
    maxMarkers = GetIntValue("max_markers");
    memory = GetOutputMatrix("MEMORY");
    lastIndex = 0;
}



void
MarkerMemory::Tick()
{
    for (int i = 0; i < maxMarkers; i++) {
        bool in_memory = false;
        if (fabsf(input[i][2] - 0.f) > 0.1f) {
        
            for (int k = 0; k < maxMarkers; k++) {
            
                if (fabsf(input[i][2] - memory[k][2]) < 0.1f) {
                    aCpy(memory[k], input[i]);
                    in_memory = true;
                    
                    break;
                }
            }
            
            if (in_memory == false) {
                aCpy(memory[lastIndex], input[i]);
                printf("MarkerMemory[%d]: %f\n", lastIndex, memory[lastIndex][2]);
                
                lastIndex++;
                if (lastIndex >= maxMarkers) {
                    lastIndex = 0;
                }
            }
            
        }
    }
}

void MarkerMemory::aCpy(float* a, float* b)
{
    for (int k = 0; k < sizeof(b); k++) {
        a[k] = b[k];
    }
}



static InitClass init("MarkerMemory", &MarkerMemory::Create, "Source/UserModules/MarkerMemory/");


