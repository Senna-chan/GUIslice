// 
// 
// 

#include "GUIslice_hmi.h"

#if defined(HMI_SERIAL)
void gslc_hmi_init()
{
    // HardwareSerial Serial2(USART2);
}

void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef){}
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef)
{
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    gslc_DebugPrintf("PageId %d, ElementId %d", pGui->asPage->nPageId, pElem->nId);
}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef){}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef){}

#else

void gslc_hmi_init() {}
void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef) {}

#endif