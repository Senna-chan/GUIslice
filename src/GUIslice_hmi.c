// 
// 
// 

#include "GUIslice_hmi.h"
#if defined(HMI_SERIAL)


void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef){}
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef)
{
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    gslc_Printf(g_pfHmiOut, "PageId %d, ElementId %d\r\n",pGui->asPage->nPageId, pElem->nId);
}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef){}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef){}

#else

void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef) {}

#endif