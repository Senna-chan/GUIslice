// 
// 
// 

#include "GUIslice_hmi.h"
#if defined(GSLC_HMI_ENABLE)


typedef uint8_t HMI_SERIAL_CMD_COMMANDS;

const char* HMI_SERIAL_COMMANDS_string_table[7] = {
    "SET_DATA",
    "GET_DATA",
    "EVENT",
    "PAGE",
    "SET_VALUE",
    "GET_VALUE",
    "CMD",
};

void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef){
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    int16_t pageId = gslc_GetPageCur(pGui);
    int16_t elemId = pElem->nId;
    g_pfHmiOut(EVENT + 0x20);
    g_pfHmiOut((pageId >> 8) & 0xFF);
    g_pfHmiOut(pageId & 0xFF);
    g_pfHmiOut((elemId >> 8) & 0xFF);
    g_pfHmiOut(elemId & 0xFF);
    g_pfHmiOut(GSLC_HMI_TOUCH_DOWN);
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    gslc_DebugPrintf("DOWN: PageId %d, ElementId %d\n", pageId, elemId);
}
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef)
{
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    int16_t pageId = gslc_GetPageCur(pGui);
    int16_t elemId = pElem->nId;
    g_pfHmiOut(EVENT + 0x20);
    g_pfHmiOut((pageId >> 8) & 0xFF);
    g_pfHmiOut(pageId & 0xFF);
    g_pfHmiOut((elemId >> 8) & 0xFF);
    g_pfHmiOut(elemId & 0xFF);
    g_pfHmiOut(GSLC_HMI_TOUCH_UP);
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    gslc_DebugPrintf("UP: PageId %d, ElementId %d\n", pageId, elemId);
}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef){}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId){}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef){
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    int16_t pageId = gslc_GetPageCur(pGui);
    int16_t elemId = pElem->nId;
    g_pfHmiOut(EVENT + 0x20);
    g_pfHmiOut((pageId >> 8) & 0xFF);
    g_pfHmiOut(pageId & 0xFF);
    g_pfHmiOut((elemId >> 8) & 0xFF);
    g_pfHmiOut(elemId & 0xFF);
    g_pfHmiOut(GSLC_HMI_VALUE_CHANGED);
    switch (pElem->nType) {
        case GSLC_TYPE_TXT:
        case GSLC_TYPE_BASE_EXTEND + 4: { //GSLC_TYPEX_TEXTBOX:
            char* str = gslc_ElemGetTxtStr(pGui, pElemRef);
            gslc_HmiPrintf("%s", str);
            break;
        }
        case GSLC_TYPE_BASE_EXTEND + 1: {//GSLC_TYPEX_CHECKBOX
            bool checkBoxState = gslc_ElemXCheckboxGetState(pGui, pElemRef);
            g_pfHmiOut(checkBoxState ? 1 : 0);
            break;
        }
    }
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    gslc_DebugPrintf("VALUE CHANGED: PageId %d, ElementId %d\n", pageId, elemId);
}


void gslc_hmi_sendCmd(unsigned char cmd) {
    g_pfHmiOut(CMD + 0x20);
    g_pfHmiOut(0x00);
    g_pfHmiOut(0x00);
    g_pfHmiOut(0x00);
    g_pfHmiOut(0x00);
    
    g_pfHmiOut(cmd);

    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
}
void gslc_hmi_sendCmdWithData(unsigned char cmd, uint8_t* data, uint8_t dataLength) {
    g_pfHmiOut(CMD + 0x20);
    g_pfHmiOut(0x00);
    g_pfHmiOut(0x00);
    g_pfHmiOut(0x00);
    g_pfHmiOut(0x00);

    g_pfHmiOut(cmd);
    for(uint8_t i; i < dataLength; i++){
        g_pfHmiout(data[i]);
    }
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
    g_pfHmiOut(0xFF);
}


uint8_t buffer[HMI_SERIAL_BUFFER_SIZE]; // User configurable so as not to strain memory
uint16_t currentBufferIndex = 0; // Used to hold the index of the buffer
uint8_t FFCount = 0; // A command is valid when it is ended with 3 0xFF's
uint8_t validCommand = 0;
void gslc_hmi_loop(void* pvGui)
{
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    //int32_t bytesToRead = HMI_SERIAL.available();
    //if (HMI_SERIAL.available() > 0) // Read serial input
    //{
    //    uint8_t validCommand = 0;
    //    while (HMI_SERIAL.available() > 0)
    //    {
    //        // if (currentBufferIndex > HMI_SERIAL_BUFFER_SIZE)
    //        // {
    //        //     HMI_SERIAL.write((const char[]){0xA0,0xFF,0xFF,0xFF});
    //        // }
    //        uint8_t b = HMI_SERIAL.read();
    //        Serial.printf("I %3d B %#04X\r\n", currentBufferIndex, b);
    //        buffer[currentBufferIndex] = b;
    //        if (b == 0xFF) {
    //            FFCount++;
    //        }
    //        currentBufferIndex++;
    //        if (FFCount >= 3 && HMI_SERIAL.peek() != 0xFF)
    //        {
    //            validCommand = 1;
    //            break;
    //        }
    //        else if (FFCount >= 3 && HMI_SERIAL.peek() == 0xFF) {
    //            // Do not break
    //        }
    //        delayMicroseconds(100); // Small delay to not directly exit the read loop but also not to starve the MCU of clock cycles and responsiveness
    //    }
    int16_t t = g_pfHmiIn();
    if (t != -1) // Read serial input
    {
        while (t != -1)
        {
            // if (currentBufferIndex > HMI_SERIAL_BUFFER_SIZE)
            // {
            //     HMI_SERIAL.write((const char[]){0xA0,0xFF,0xFF,0xFF});
            // }
            //if (t == -1) break;
            uint8_t b = t & 0xFF; // Go from int16 to uint32
            delayMicroseconds(100); // Small delay to not directly read a non existing byte but also not to starve the MCU of clock cycles and responsiveness
            t = g_pfHmiIn(); // Read byte here so we can use it later in this loop

            gslc_DebugPrintf("I %3d B %#04X\r\n", currentBufferIndex, b);
            buffer[currentBufferIndex] = b;
            if (b == 0xFF) {
                FFCount++;
            }
            currentBufferIndex++;
            if (FFCount >= 3 && t != 0xFF)
            {
                validCommand = 1;
                break;
            }
            else if (FFCount >= 3 && t == 0xFF) {
                // Do not break
            }
}

        if (validCommand) // Exec command
        {
            validCommand = false;
            FFCount = 0;
            char command = buffer[0];
            int16_t pageId = buffer[1] << 8 | buffer[2];
            int16_t elementId = buffer[3] << 8 | buffer[4];
            //int16_t pageId = gslc_GetPageCur(pGui);
            gslc_tsPage* pPageElem = gslc_PageFindById(pGui, pageId);
            gslc_tsElemRef* pElemRef = gslc_PageFindElemById(pGui, pageId, elementId);
            gslc_tsElem* pElem;
            uint8_t dataSize = currentBufferIndex - 8;
            uint8_t data[dataSize]; // Put the buffer without ids/command/0xFF in separate buffer
            memmove(data, buffer + 5, dataSize);
            bool elementFound = pElemRef != NULL;

            if (!elementFound) {
                gslc_DebugPrintf("INFO: Element with id %#04X is not found\r\n", elementId);
            }
            else {
                pElem = gslc_GetElemFromRef(pGui, pElemRef);
            }
            gslc_DebugPrintf("command %s, elementId %d, pageId %d, buffer: ", HMI_SERIAL_COMMANDS_string_table[command], elementId, pageId);
            for (uint16_t i = 0; i < currentBufferIndex; i++) {
                gslc_DebugPrintf("%#04X ", buffer[i]);
            }
            gslc_DebugPrintf("\r\n");
            switch (command)
            {
            case GET_DATA: // 0x01
            {
                if (!elementFound) {
                    break;
                }
                uint8_t dataType = data[0];

                // Always return data
                g_pfHmiOut(GET_DATA + 0x20);
                g_pfHmiOut(buffer[1]); // Element Id
                g_pfHmiOut(buffer[2]); // Element Id
                g_pfHmiOut(buffer[3]); // Page Id
                g_pfHmiOut(buffer[4]); // Page Id
                g_pfHmiOut(dataType);
                switch (dataType)
                {
                case BACKGROUND_COLOR:
                    g_pfHmiOut(pElem->colElemFill.r);
                    g_pfHmiOut(pElem->colElemFill.g);
                    g_pfHmiOut(pElem->colElemFill.b);
                    break;
                case FOREGROUND_COLOR:
                    g_pfHmiOut(pElem->colElemText.r);
                    g_pfHmiOut(pElem->colElemText.g);
                    g_pfHmiOut(pElem->colElemText.b);
                    break;
                case WIDTH:
                    g_pfHmiOut(pElem->rElem.w >> 8 & 0xFF);
                    g_pfHmiOut(pElem->rElem.w & 0xFF);
                    break;
                case HEIGHT:
                    g_pfHmiOut(pElem->rElem.h >> 8 & 0xFF);
                    g_pfHmiOut(pElem->rElem.h & 0xFF);
                    break;
                case XPOS:
                    g_pfHmiOut(pElem->rElem.x >> 8 & 0xFF);
                    g_pfHmiOut(pElem->rElem.x & 0xFF);
                    break;
                case YPOS:
                    g_pfHmiOut(pElem->rElem.y >> 8 & 0xFF);
                    g_pfHmiOut(pElem->rElem.y & 0xFF);
                    break;
                default:
                    gslc_DebugPrintf("DataType %#04X is not known for element %#06X P %#06X\r\n", dataType, elementId, pageId);
                    g_pfHmiOut(0x00); // Make sure we atleast send back one byte as return data
                }
                g_pfHmiOut(0xFF);
                g_pfHmiOut(0xFF);
                g_pfHmiOut(0xFF);
                break;
            }
            case SET_DATA: // 0x00
            {
                if (!elementFound) {
                    break;
                }
                uint8_t dataType = data[0];
                bool redraw = true;
                switch (dataType)
                {
                case BACKGROUND_COLOR:
                    pElem->colElemFill.r = data[1];
                    pElem->colElemFill.g = data[2];
                    pElem->colElemFill.b = data[3];
                    break;
                case FOREGROUND_COLOR:
                    pElem->colElemText.r = data[1];
                    pElem->colElemText.g = data[2];
                    pElem->colElemText.b = data[3];
                    break;
                case WIDTH:
                    pElem->rElem.w = data[1] << 8 | data[2];
                    break;
                case HEIGHT:
                    pElem->rElem.h = data[1] << 8 | data[2];
                    break;
                case XPOS:
                    pElem->rElem.x = data[1] << 8 | data[2];
                    break;
                case YPOS:
                    pElem->rElem.y = data[1] << 8 | data[2];
                    break;
                default:
                    gslc_DebugPrintf("DataType %#04X is not known for element %#06X P %#06X\r\n", dataType, elementId, pageId);
                    redraw = false;
                }
                if (redraw) gslc_PageRedrawSet(pGui, true);// When redrawing redraw entire page
                break;
            }
            case PAGE:// 0x03 Switch Page
            {
                gslc_SetPageCur(pGui, pageId);
                break;
            }
            case EVENT: // 0x02 Press stuff(Button/checkbox)
            {
                if (!elementFound) {
                    break;
                }
                uint16_t eY, eX;
                eX = pElem->rElem.x + (pElem->rElem.w / 2);
                eY = pElem->rElem.y + (pElem->rElem.h / 2);
                gslc_ElemSendEventTouch(pGui, pElemRef, GSLC_TOUCH_DOWN_IN, eX, eY);
                gslc_ElemSendEventTouch(pGui, pElemRef, GSLC_TOUCH_UP_IN, eX, eY);
                break;
            }
            case SET_VALUE: // 0x04 Set data elements
            {
                if (!elementFound) {
                    break;
                }
                switch (pElem->nType)
                {
#ifdef GSLC_FEATURE_COMPOUND
                case GSLC_TYPE_BASE_EXTEND + 1: // GSLC_TYPEX_CHECKBOX:
                    gslc_ElemXCheckboxSetState(pGui, pElemRef, buffer[5]);
                    break;
#endif
                case GSLC_TYPE_TXT:
                case GSLC_TYPE_BTN:
                default:
                {
                    char text[dataSize + 1];
                    memmove(data, buffer, dataSize);
                    text[dataSize + 1] = '\0'; // End byte
                    gslc_ElemSetTxtStr(pGui, pElemRef, text);
                    break;
                }
                }
                break;
            }
            case GET_VALUE: { // 0x05 Get data
                if (!elementFound) {
                    break;
                }
                g_pfHmiOut(GET_DATA + 0x20);
                switch (pElem->nType)
                {
                case GSLC_TYPE_TXT:
                    gslc_HmiPrintf(gslc_ElemGetTxtStr(pGui, pElemRef));
                    break;
                case GSLC_TYPE_BASE_EXTEND + 1: {//GSLC_TYPEX_CHECKBOX: {
                    bool checkBoxState = gslc_ElemXCheckboxGetState(pGui, pElemRef);
                    g_pfHmiOut(checkBoxState ? 1 : 0);
                    break;
                }
                default:
                    gslc_DebugPrintf("WARNING: Element with id %#04X type %#04X is not handled for data\r\n", elementId, pElem->nType);
                    break;
                }
                g_pfHmiOut(0xFF);
                g_pfHmiOut(0xFF);
                g_pfHmiOut(0xFF);
            }
            case CMD: // 0x06
                switch (data[0]) {
                case 0x00:
                    gslc_DebugPrintf("RESET");
                    delay(50);
                    HAL_NVIC_SystemReset();
                    break;
                }
                break;
            default:
                gslc_DebugPrintf("Command %#02X is not known\r\n", command);
                break;
            }

            // We are done handling serial so we clear the buffer
            memset(buffer, 0, HMI_SERIAL_BUFFER_SIZE);
            currentBufferIndex = 0;
            FFCount = 0;
        }
    }
}

#else
// All blank to not cause compiler exceptions
void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId) {}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef) {}
void gslc_hmi_sendCmd(uint8_t cmd) {}
void gslc_hmi_sendCmdWithData(uint8_t cmd, uint8_t* data, uint8_t dataLength) {}
void gslc_hmi_loop(void* pvGui) {}
#endif