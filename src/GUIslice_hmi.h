// GUIslice_hmi.h

#ifndef _GUISLICE_HMI_h
#define _GUISLICE_HMI_h

#include <Arduino.h>
#include "GUIslice.h"


// uint8_t buffer[HMI_SERIAL_BUFFER_SIZE]; // User configurable so as not to strain memory
// uint16_t currentBufferIndex = 0; // Used to hold the index of the buffer

#define HMI_BUFFER_ERROR (uint8_t[]){0xA0,0xFF,0xFF,0xFF}


void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef);
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef);
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef);
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId);
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId);
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef);
void gslc_hmi_loop(void* pvGui);
#endif