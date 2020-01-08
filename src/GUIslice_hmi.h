// GUIslice_hmi.h

#ifndef _GUISLICE_HMI_h
#define _GUISLICE_HMI_h

#include "GUIslice.h"

void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef);
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef);
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef);
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId);
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId);
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef);

#endif