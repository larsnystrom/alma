#include "PrioMod.h"
#include <math.h>

using namespace ikaros;

float
PrioMod::dist(float x1, float y1, float z1, float x2, float y2, float z2){
  float d;

  d = pow((x1-x2), 2) + pow((y1-y2), 2) + pow((z1-z2), 2);
  d = sqrt(d);

  return d;
}

/**
 *Ex:
 *1=Head center
 *2=Face center
 *3=AR-tagg location
 **/
float
PrioMod::linePlaneIntersect(
    float x1, float y1, float z1, 
    float x2, float y2, float z2, 
    float x3, float y3, float z3)
{
    float num = x1 * ( (z3/x3) + (z3*y3)/(2*x3*x3) + z3/(2*x3*x3) ) - y1*( z3/(2*x3) ) - z1 - z3/(2*x3);
    
    float denom = z2 - z1 - (x2-x1)*( (z3/x3) + (z3*y3)/(2*x3*x3) + z3/(2*x3*x3) ) + (y2-y1)*( z3/(2*x3) );
    
    float dl = dist( 
        (x1 + (num/denom)*(x2-x1)), 
        (y1 + (num/denom)*(y2-y1)), 
        (z1 + (num/denom)*(z2-z1)), 
        x3, 
        y3, 
        z3
    );
    
    return dl;
}

int
PrioMod::attendedMarker(float* center, float* front, float threshold)
{
    float curDist = 0.0f;
    float minDist = threshold;
    int markerId = -1;
    
    for (int i = 0; i < maxMarkers; ++i) {
        
        if (markers[i][2] > 0.1f) {
            curDist = linePlaneIntersect(
                center[0], center[1], center[2], 
                front[0], front[1], front[2],
                markers[i][0], markers[i][1], markers[i][23] / 1000.f
            );
            
            if (curDist < minDist) {
                markerId = i;
                minDist = curDist;
            }
        }
    }
    
    return markerId;
}

void
PrioMod::lookAtDefault()
{
    attended[0] = 0.5f;
    attended[1] = 0.5f;
    attended[2] = 2.0f;
}

void
PrioMod::lookAtHead()
{
    attended[0] = head_center[0];
    attended[1] = head_center[1];
    attended[2] = head_center[2];
}

void
PrioMod::lookAtHand()
{
    attended[0] = hand_center[0];
    attended[1] = hand_center[1];
    attended[2] = hand_center[2];
    waveAttCounter--;
}

void
PrioMod::lookAtMarker(int markerId)
{
    attended[0] = markers[markerId][0];
    attended[1] = markers[markerId][1];
    attended[2] = markers[markerId][23] / 1000.f;
}

void
PrioMod::Init(){
    maxMarkers = GetIntValue("max_markers");
    
    headPoseAttendMaxDist = GetFloatValue("HEAD_ATTEND_MAX_DIST");   
    printf("PrioMod: HEAD_ATTEND_MAX_DIST=%f\n", headPoseAttendMaxDist);
    
    markers = GetInputMatrix("ATTENDABLES");
    head_center = GetInputArray("HEAD_CENTER");
    head_front = GetInputArray("HEAD_FRONT");
    
    
    handPoseAttendMaxDist = GetFloatValue("HAND_ATTEND_MAX_DIST");
    printf("PrioMod: HAND_ATTEND_MAX_DIST=%f\n", handPoseAttendMaxDist);
    
    hand_center = GetInputArray("HAND_CENTER");
    hand_front = GetInputArray("HAND_FRONT");
    hand_wave = GetInputArray("HAND_WAVE");
    
    attended = GetOutputArray("ATTENDED");
    attendedView = GetOutputArray("ATTENDED_VIEW");
    
    waveAttCounter = 0;
    prevHandMarker = -1;
    prevHeadMarker = -1;
}

void
PrioMod::Tick(){
    int headMarker = -1;
    int handMarker = -1;
    bool headExists = false;
    bool handExists = false;
    
    if (head_center[0] > 0.1f) {
        headMarker = attendedMarker(head_center, head_front, headPoseAttendMaxDist);
        headExists = true;
    } 
    
    if (hand_center[0] > 0.1f) {
        handMarker = attendedMarker(hand_center, hand_front, handPoseAttendMaxDist);
        handExists = true;
    }
    
    if (hand_wave[0] > 0.1f) {
        waveAttCounter = 20;
    }
    
    if (waveAttCounter > 0) {
        lookAtHand();
    } else {
        
        if (false == headExists && false == handExists) {
            lookAtDefault();
        }
        
        if (true == headExists && false == handExists) {
            if (-1 != headMarker) {
                lookAtMarker(headMarker);
            } else {
                lookAtHead();
            }
        }
        
        if (false == headExists && true == handExists) {
            if (-1 != handMarker) {
                lookAtMarker(handMarker);
            } else {
                lookAtDefault();
            }
        }
        
        if (true == headExists && true == handExists) {
            if (-1 == headMarker && -1 == handMarker) {
                lookAtHead();
            }
            
            if (-1 == headMarker && -1 != handMarker) {
                lookAtMarker(handMarker);
            }
            
            if (-1 != headMarker && -1 == handMarker) {
                lookAtMarker(headMarker);
            }
            
            if (-1 != headMarker && -1 != handMarker) {
                if (headMarker == handMarker) {
                    lookAtMarker(headMarker);
                } else {
                    if (prevHeadMarker != headMarker && prevHandMarker != handMarker) {
                        // Hand pointing has precedence
                        lookAtMarker(handMarker);
                    }
                    
                    if (prevHeadMarker == headMarker && prevHandMarker != handMarker) {
                        lookAtMarker(handMarker);
                    }
                    
                    if (prevHeadMarker != headMarker && prevHandMarker == handMarker) {
                        lookAtMarker(headMarker);
                    }
                    
                    if (prevHeadMarker == headMarker && prevHandMarker == handMarker) {
                        // Hand pointing has precedence
                        lookAtMarker(handMarker);
                    }
                }
            }
        }
    }
    
    attendedView[0] = attended[0];
    attendedView[1] = attended[1];
    
    prevHandMarker = handMarker;
    prevHeadMarker = headMarker;
}

static InitClass init("PrioMod", &PrioMod::Create, "Source/UserModules/PrioMod/");
