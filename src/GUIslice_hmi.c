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
    gslc_Printf(g_pfHmiOut, "PageId %d, ElementId %d\r\n", gslc_GetPageCur(pGui), pElem->nId);
}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef){}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef){}

void gslc_hmi_loop(void* pvGui)
{
    // if(HMI_SERIAL.available() > 0) // Read serial input
    // {
    //     uint8_t validCommand = 0;
    // 	uint8_t c = HMI_SERIAL.read();
    //     uint8_t FFCount = 0; // A command is valid when it is ended with 3 0xFF's
    //     while(FFCount < 3)
    //     {
    //     	
    //         currentBufferIndex++;
    //     	if(currentBufferIndex > HMI_SERIAL_BUFFER_SIZE)
    //     	{
    //             HMI_SERIAL.write(HMI_BUFFER_ERROR);
    //     	}
    //     }
    // 	
    //
    // 	if(validCommand) // Exec command
    // 	{
    //
    //
    //
    // 		// Command is finished. Clear buffer
    //         memset(buffer, 0, HMI_SERIAL_BUFFER_SIZE);
    // 	}
    // }
}

#else
// All blank to not cause compiler exceptions
void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef) {}
void gslc_hmi_loop(void* pvGui) {}
#endif