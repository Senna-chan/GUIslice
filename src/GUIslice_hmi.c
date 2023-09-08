// 
// 
// 

#include "GUIslice_hmi.h"
#if defined(GSLC_HMI_ENABLE)
#ifdef GSLC_FEATURE_COMPOUND
#include "elem/XCheckbox.h"
#include "elem/XGraph.h"
#include "elem/XKeyPad_Alpha.h"
#include "elem/XKeyPad_Num.h"
#include "elem/XListbox.h"
#include "elem/XProgress.h"
#include "elem/XRadial.h"
#include "elem/XRamp.h"
#include "elem/XRingGauge.h"
#include "elem/XSeekbar.h"
#include "elem/XSlider.h"
#include "elem/XSpinner.h"
#include "elem/XTextbox.h"
#include "elem/XTogglebtn.h"
#endif

//typedef uint8_t HMI_SERIAL_CMD_COMMANDS;

const char* HMI_SERIAL_COMMANDS_string_table[7] = {
    "SET_DATA",
    "GET_DATA",
    "EVENT",
    "PAGE",
    "SET_VALUE",
    "GET_VALUE",
    "CMD",
};


const uint8_t messageSizeNoData = 9;

const uint8_t headerByte = 0xFE;
const uint8_t footerByte = 0xFF;

#define writeDebugByte(b) gslc_DebugPrintf("0x%x ",b); g_pfHmiOut(b)

void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef){
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    int16_t elemId = pElem->nId << 1;

    hmiCommand_s hmiC = { 0 };
    hmiC.header = headerByte;
    hmiC.footer = footerByte;

    hmiC.command = EVENT + 0x20;
    hmiC.elementId = elemId;
    hmiC.data = malloc(1);
    hmiC.data[0] = GSLC_HMI_TOUCH_DOWN;
    hmiC.crc = 0;
    hmiC.size = messageSizeNoData + 1;
    gslc_hmi_write_packet(hmiC);
    gslc_DebugPrintf("DOWN: ElementId %d\n", elemId);
}

void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef)
{
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    int16_t elemId = pElem->nId << 1;

    hmiCommand_s hmiC = { 0 };
    hmiC.header = headerByte;
    hmiC.footer = footerByte;
    
    hmiC.command = EVENT + 0x20;
    hmiC.elementId = elemId;
    hmiC.data = malloc(1);
    hmiC.data[0] = GSLC_HMI_TOUCH_UP;
    hmiC.crc = 0;
    hmiC.size = messageSizeNoData + 1;
    gslc_hmi_write_packet(hmiC);
    gslc_DebugPrintf("UP: ElementId %d\n", elemId);
}
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef){}
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId){
    int16_t pId = pageId << 1 | 0x01;

    hmiCommand_s hmiC = { 0 };
    hmiC.header = headerByte;
    hmiC.footer = footerByte;

    hmiC.command = EVENT + 0x20;
    hmiC.elementId = pId;
    hmiC.data = malloc(1);
    hmiC.data[0] = GSLC_HMI_PAGE_ENTER;
    hmiC.crc = 0;
    hmiC.size = messageSizeNoData + 1;
    gslc_hmi_write_packet(hmiC);
    gslc_DebugPrintf("PAGE_LOADED: Id %d\n", pId);
}
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId){
    int16_t pId = pageId << 1 | 0x01;

    hmiCommand_s hmiC = { 0 };
    hmiC.header = headerByte;
    hmiC.footer = footerByte;

    hmiC.command = EVENT + 0x20;
    hmiC.elementId = pId;
    hmiC.data = malloc(1);
    hmiC.data[0] = GSLC_HMI_PAGE_LEAVE;
    hmiC.crc = 0;
    hmiC.size = messageSizeNoData + 1;

    gslc_hmi_write_packet(hmiC);
    gslc_DebugPrintf("PAGE_UNLOADED: Id %d\n", pId);
}
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef){
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
    gslc_tsElem* pElem = pElemRef->pElem;
    int16_t elemId = pElem->nId << 1;

    hmiCommand_s hmiC = { 0 };
    hmiC.header = headerByte;
    hmiC.footer = footerByte;

    hmiC.command = EVENT + 0x20;
    hmiC.elementId = elemId;
    hmiC.data = malloc(101);
    hmiC.data[0] = GSLC_HMI_VALUE_CHANGED;
    hmiC.crc = 0;
    hmiC.size = messageSizeNoData + 1;


   switch (pElem->nType) {
        case GSLC_TYPE_TXT:
#ifdef GSLC_FEATURE_COMPOUND
        case GSLC_TYPEX_TEXTBOX:
#endif
        {
            char* str = gslc_ElemGetTxtStr(pGui, pElemRef);
            char* strPtr;
            int i = 1;
            hmiC.size += strlen(str);
            for (strPtr = str; *strPtr != '\0'; strPtr++) {
                hmiC.data[i] = (*strPtr);
                i++;
            }
            break;
        }
#ifdef GSLC_FEATURE_COMPOUND
        case GSLC_TYPEX_LISTBOX:
        {
            int16_t selItem = gslc_ElemXListboxGetSel(pGui, pElemRef);
            if (selItem == -1) {
                gslc_DebugPrintf("Listbox Element is -1\r\n");
                break;
            }
            char acTxt[100 + 1];
            gslc_ElemXListboxGetItem(pGui, pElemRef, selItem, acTxt, 100);
            char* strPtr = &acTxt;

            int i = 1;
            hmiC.size += strlen(acTxt);
            for (strPtr; *strPtr != '\0'; strPtr++) {
                hmiC.data[i] = (*strPtr);
                i++;
            }
            break;
        }
        case GSLC_TYPEX_CHECKBOX: {
            bool checkBoxState = gslc_ElemXCheckboxGetState(pGui, pElemRef);
            hmiC.size++;
            hmiC.data[2] = checkBoxState;
            break;
        }
        case GSLC_TYPEX_SLIDER: {
            int16_t pos = gslc_ElemXSliderGetPos(pGui, pElemRef);
            hmiC.size+=2;
            hmiC.data[2] = (pos >> 8 & 0xFF);
            hmiC.data[3] = (pos & 0xFF);
            break;
        }
        case GSLC_TYPEX_SPINNER: {
            int16_t pos = gslc_ElemXSpinnerGetCounter(pGui, pElemRef);
            hmiC.size += 2;
            hmiC.data[2] = (pos >> 8 & 0xFF);
            hmiC.data[3] = (pos & 0xFF);
            break;
        }
        case GSLC_TYPEX_TOGGLEBTN: {
            bool toggleState = gslc_ElemXTogglebtnGetState(pGui, pElemRef);
            hmiC.size++;
            hmiC.data[2] = (toggleState ? 1 : 0);
            break;
        }
#endif
    }
   gslc_hmi_write_packet(hmiC);
    gslc_DebugPrintf("VALUE CHANGED: ElementId %d\n", elemId);
}


void gslc_hmi_sendCmd(unsigned char cmd) {

    hmiCommand_s hmiC = { 0 };
    hmiC.header = headerByte;
    hmiC.footer = footerByte;

    hmiC.command = CMD + 0x20;
    hmiC.elementId = 0;
    hmiC.data = malloc(1);
    hmiC.data[0] = cmd;
    hmiC.crc = 0;
    hmiC.size = messageSizeNoData + 1;
    gslc_hmi_write_packet(hmiC);
    gslc_DebugPrintf("SENDING RAW COMMAND: 0x%x\n", cmd);
}
void gslc_hmi_sendCmdWithData(unsigned char cmd, uint8_t* data, uint8_t dataLength) {


    hmiCommand_s hmiC = { 0 };
    hmiC.header = headerByte;
    hmiC.footer = footerByte;

    hmiC.command = CMD + 0x20;
    hmiC.elementId = 0;
    hmiC.data = malloc(1 + dataLength);
    hmiC.data[0] = cmd;
    memcpy(hmiC.data + 1, data, dataLength);
    hmiC.crc = 0;
    hmiC.size = messageSizeNoData + 1;
    gslc_hmi_write_packet(hmiC);
    gslc_DebugPrintf("SENDING RAW COMMAND WITH DATA: 0x%x\n", cmd);
}

uint8_t calccrc8(uint8_t* buff, uint8_t size)
{
    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < size; i++) {
        crc ^= buff[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return crc;
}

typedef enum {
    header,
    packetNumber,
    size,
    command,
    elementId,
    data,
    crc,
    footer,
    validcommand
} readStates;
readStates readState = header;
const char* readStates_str[9] = {
    "header",
    "packetNumber",
    "size",
    "command",
    "elementId",
    "data",
    "crc",
    "footer",
    "validcommand"
};

int16_t byteToParse = -1;
uint8_t buffer[255];
uint8_t bufferIndex;
uint8_t nextPacketNumber = 0;
hmiCommand_s hmiReadCommand;

void gslc_hmi_read_packet() {
    if (byteToParse == -1) {
        byteToParse = g_pfHmiIn();
    }
    else // Read serial input
    {
        while (byteToParse != -1)
        {
            uint8_t b = byteToParse & 0xFF; // Go from int16 to uint8
            #ifdef ARDUINO
            delayMicroseconds(90); // Small delay to not directly read a non existing byte but also not to starve the MCU of clock cycles and responsiveness
            #else
            usleep(90);
            #endif
            byteToParse = g_pfHmiIn(); // Read byte here so we can use it later in this loop
            //gslc_DebugPrintf("D 0x%x,RS'%s'\n", b, readStates_str[readState]);
            switch (readState) {
            case header:
                if (b == headerByte) {
                    hmiReadCommand.header = b;
                    readState = packetNumber;
                }
                else {
                    // Error_Handler();
                    // TODO: Trigger error back for unexpected byte
                }
                break;
            case packetNumber:
                hmiReadCommand.packetNumber = b;
                readState = size;
                break;
            case size:
                if (bufferIndex == 2) {
                    hmiReadCommand.size |= b << 8;
                }
                else {
                    hmiReadCommand.size |= b;
                    hmiReadCommand.dataSize = hmiReadCommand.size - messageSizeNoData;
                    gslc_DebugPrintf("Message size %d datasize %d\n", hmiReadCommand.size, hmiReadCommand.dataSize);
                    readState = command;
                }
                break;
            case command:
                hmiReadCommand.command = b;
                readState = elementId;
                break;
            case elementId:
                if (bufferIndex == 5) {
                    hmiReadCommand.elementId |= b << 8;
                }
                else {
                    hmiReadCommand.elementId |= b;
                    if (hmiReadCommand.dataSize != 0) {
                        readState = data;
                    }
                    else {
                        readState = crc;
                    }
                }
                break;
            case data:
                if (hmiReadCommand.data == NULL) {
                    hmiReadCommand.data = malloc(hmiReadCommand.dataSize);
                    memset(hmiReadCommand.data, 0, hmiReadCommand.dataSize);
                }
                uint8_t* dataPtr = hmiReadCommand.data;
                dataPtr += (bufferIndex - (messageSizeNoData - 2)); // We have no footer and CRC yet. So minus 2
                *dataPtr = b;
                hmiReadCommand.dataSize--;
                if (hmiReadCommand.dataSize == 0) {
                    readState = crc;
                    hmiReadCommand.dataSize = hmiReadCommand.size - messageSizeNoData;
                }
                break;
            case crc:
                hmiReadCommand.crc = b;
                readState = footer;
                break;
            case footer:
                if (b == footerByte) {
                    hmiReadCommand.footer = b;
                    readState = validcommand;
                    gslc_DebugPrintf("D 0x%x, RS '%s'\n\n", b, readStates_str[readState]);
                }
                else {
                    //gslc_DebugPrintf("FOOTER Expected 0xFF, Got 0x%x", b);
                    gslc_DebugPrintf("FOOTER Expected 0xFF, Got 0x%x. Buffer: ", b);
                    for (uint16_t i = 0; i < bufferIndex; i++) {
                        gslc_DebugPrintf("0x%x ", buffer[i]);
                    }
                    gslc_DebugPrintf("\r\n");
                    // Error_Handler();
                    // TODO: Trigger error back for unexpected byte
                }
                break;
            case validcommand:
                break;
            }
            //gslc_DebugPrintf("I %d B 0x%x\r\n", currentBufferIndex, b);
            buffer[bufferIndex] = b;
            bufferIndex++;
            if (readState == validcommand) {
                break;
            }
        }
    }
}

void gslc_hmi_write_packet(hmiCommand_s hmiCommand)
{
    writeDebugByte(hmiCommand.header);
    writeDebugByte(nextPacketNumber);
    nextPacketNumber++;
    writeDebugByte((hmiCommand.size >> 8) & 0xFF);
    writeDebugByte(hmiCommand.size & 0xFF);
    writeDebugByte(hmiCommand.command);
    writeDebugByte((hmiCommand.elementId >> 8) & 0xFF);
    writeDebugByte(hmiCommand.elementId & 0xFF);
    for (int i = 0; i < hmiCommand.size - messageSizeNoData; i++) {
        writeDebugByte(hmiCommand.data[i]);
    }
    writeDebugByte(hmiCommand.crc);
    writeDebugByte(hmiCommand.footer);
    free(hmiCommand.data); // Freeing this because otherwise we will get huge memory leaks
}

void gslc_hmi_loop(void* pvGui)
{
    gslc_tsGui* pGui = (gslc_tsGui*)(pvGui);
    gslc_hmi_read_packet();
    if (readState == validcommand) // Exec command
    {
        readState = header;
        if (bufferIndex != hmiReadCommand.size) {
            gslc_DebugPrintf("Packet size incorrect. want %d got %d\n", hmiReadCommand.size, bufferIndex);
            // Error_Handler();
            //TODO: Trigger invalid size error
        }
        char command = hmiReadCommand.command;
        //int16_t pageId = buffer[1] << 8 | buffer[2];
        int16_t elementId = hmiReadCommand.elementId;
        int16_t pageId = gslc_GetPageCur(pGui);
        //gslc_tsPage* pPageElem = gslc_PageFindById(pGui, pageId);
        gslc_tsElemRef* pElemRef = gslc_PageFindElemById(pGui, pageId, elementId);
        gslc_tsElem* pElem;
        uint8_t crc = hmiReadCommand.crc;
        uint8_t calccrc = calccrc8(&buffer[3], hmiReadCommand.size - 5);
        if (crc != calccrc) {
            gslc_DebugPrintf("crc is not the same. received 0x%x calculated 0x%x\r\n", crc, calccrc);

            //gslc_DebugPrintf("crc is not the same. received 0x%x calculated 0x%x. Data: \r\n", crc, calccrc);

            //gslc_DebugPrintf("command %s, elementId %d, pageId %d, buffer: ", HMI_SERIAL_COMMANDS_string_table[command], elementId, pageId);
            //for (uint16_t i = 0; i < bufferIndex; i++) {
            //    gslc_DebugPrintf("0x%x ", buffer[i]);
            //}
            //gslc_DebugPrintf("\r\n");
            //// We are done handling serial so we clear the buffer
            //memset(buffer, 0, 255);
            //free(hmiReadCommand.data);
            //memset(&hmiReadCommand, 0, sizeof(hmiCommand_s));
            //bufferIndex = 0;
            //readState = header;
            ////TODO: Print error to HMI
            //return; // invalid CRC. We need to return something here to say that a retransmit is needed
        }
        uint8_t dataSize = hmiReadCommand.dataSize;
        bool elementFound = pElemRef != NULL;
        if (!elementFound) {
            if (command != CMD && command != PAGE) {
                gslc_DebugPrintf("INFO: Element with id %d is not found\r\n", elementId);
            }
        }
        else {
            pElem = gslc_GetElemFromRef(pGui, pElemRef);
        }

        switch (command)
        {
        case SET_DATA: // 0x00
        {
            if (!elementFound) {
                break;
            }
            uint8_t dataType = (hmiReadCommand.data[0] & 0xFE); // We will put the redraw in bit 8. so 0b10000001 will mean redraw when setting BG COLOR
            bool redraw = hmiReadCommand.data[0] & ~0xFE;
            switch (dataType)
            {
            case BACKGROUND_COLOR:
                pElem->colElemFill.r = hmiReadCommand.data[1];
                pElem->colElemFill.g = hmiReadCommand.data[2];
                pElem->colElemFill.b = hmiReadCommand.data[3];
                break;
            case FOREGROUND_COLOR:
                pElem->colElemFrame.r = hmiReadCommand.data[1];
                pElem->colElemFrame.g = hmiReadCommand.data[2];
                pElem->colElemFrame.b = hmiReadCommand.data[3];
                break;
            case WIDTH:
                pElem->rElem.w = hmiReadCommand.data[1] << 8 | hmiReadCommand.data[2];
                break;
            case HEIGHT:
                pElem->rElem.h = hmiReadCommand.data[1] << 8 | hmiReadCommand.data[2];
                break;
            case XPOS:
                pElem->rElem.x = hmiReadCommand.data[1] << 8 | hmiReadCommand.data[2];
                break;
            case YPOS:
                pElem->rElem.y = hmiReadCommand.data[1] << 8 | hmiReadCommand.data[2];
                break;
            default:
                gslc_DebugPrintf("DataType %d is not known for element %d P %d\r\n", dataType, elementId, pageId);
                redraw = false;
            }
            if (redraw) gslc_PageRedrawSet(pGui, true);// When redrawing redraw entire page
            break;
        }
        case GET_DATA: // 0x01
        {
            if (!elementFound) {
                break;
            }
            uint8_t dataType = hmiReadCommand.data[0];

            // Always return data
            g_pfHmiOut(GET_DATA + 0x20);
            g_pfHmiOut(buffer[1]); // Element Id
            g_pfHmiOut(buffer[2]); // Element Id
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
            case MINVALUE:
                switch (pElem->nType)
                {
#ifdef GSLC_FEATURE_COMPOUND
                case GSLC_TYPEX_SLIDER: {
                    gslc_tsXSlider* pXData = (gslc_tsXSlider*)gslc_GetXDataFromRef(pGui, pElemRef, pElem->nType, __LINE__);
                    g_pfHmiOut(pXData->nPosMin >> 8 & 0xFF);
                    g_pfHmiOut(pXData->nPosMin & 0xFF);
                    break;
                }
                case GSLC_TYPEX_SEEKBAR: {
                    gslc_tsXSeekbar* pXData = (gslc_tsXSeekbar*)gslc_GetXDataFromRef(pGui, pElemRef, pElem->nType, __LINE__);
                    g_pfHmiOut(pXData->nPosMin >> 8 & 0xFF);
                    g_pfHmiOut(pXData->nPosMin & 0xFF);
                    break;
                }
                case GSLC_TYPEX_SPINNER: {
                    gslc_tsXSpinner* pXData = (gslc_tsXSpinner*)gslc_GetXDataFromRef(pGui, pElemRef, pElem->nType, __LINE__);
                    g_pfHmiOut(pXData->nMin >> 8 & 0xFF);
                    g_pfHmiOut(pXData->nMin & 0xFF);
                    break;
                }
#endif
                default:
                    gslc_DebugPrintf("No min value exists for element %d P %d\r\n", elementId, pageId);
                    break;
                }
                break;
            case MAXVALUE:
                switch (pElem->nType)
                {
#ifdef GSLC_FEATURE_COMPOUND
                case GSLC_TYPEX_SLIDER: {
                    gslc_tsXSlider* pXData = (gslc_tsXSlider*)gslc_GetXDataFromRef(pGui, pElemRef, pElem->nType, __LINE__);
                    g_pfHmiOut(pXData->nPosMax >> 8 & 0xFF);
                    g_pfHmiOut(pXData->nPosMax & 0xFF);
                    break;
                }
                case GSLC_TYPEX_SEEKBAR: {
                    gslc_tsXSeekbar* pXData = (gslc_tsXSeekbar*)gslc_GetXDataFromRef(pGui, pElemRef, pElem->nType, __LINE__);
                    g_pfHmiOut(pXData->nPosMax >> 8 & 0xFF);
                    g_pfHmiOut(pXData->nPosMax & 0xFF);
                    break;
                }
                case GSLC_TYPEX_SPINNER: {
                    gslc_tsXSpinner* pXData = (gslc_tsXSpinner*)gslc_GetXDataFromRef(pGui, pElemRef, pElem->nType, __LINE__);
                    g_pfHmiOut(pXData->nMax >> 8 & 0xFF);
                    g_pfHmiOut(pXData->nMax & 0xFF);
                    break;
                }
#endif
                default:
                    gslc_DebugPrintf("No max value exists for element %d P %d\r\n", elementId, pageId);
                    break;
                }
                break;
            default:
                gslc_DebugPrintf("DataType %d is not known for element %d P %d\r\n", dataType, elementId, pageId);
                g_pfHmiOut(0x00); // Make sure we atleast send back one byte as return data
            }
            g_pfHmiOut(0xFF);
            g_pfHmiOut(0xFF);
            g_pfHmiOut(0xFF);
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
        case PAGE:// 0x03 Switch Page
        {
            if (elementId == -1) { // Request is to pop the current popup page
                gslc_PopupHide(pGui);
            }
            else {
                if (dataSize != 0) { // Page is a popup. Data is 1 bool for modal type
                    gslc_PopupShow(pGui, elementId, hmiReadCommand.data[0] == 1);
                }
                else { // Page is a normal page
                    // Element ID is PageID
                    gslc_SetPageCur(pGui, elementId);
                }
            }
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
            case GSLC_TYPEX_CHECKBOX:
                gslc_ElemXCheckboxSetState(pGui, pElemRef, hmiReadCommand.data[0]);
                break;
            case GSLC_TYPEX_TOGGLEBTN:
                gslc_ElemXTogglebtnSetState(pGui, pElemRef, hmiReadCommand.data[0]);
                break;
            case GSLC_TYPEX_LISTBOX: {
                char action = hmiReadCommand.data[0];
                if (action == 'C') {
                    gslc_ElemXListboxReset(pGui, pElemRef);
                    gslc_DebugPrintf("Clearing Listbox elem %d\r\n", pElemRef->pElem->nId);
                }
                else if (action == 'A') {
                    gslc_DebugPrintf("Adding item to Listbox elem %d\r\n", pElemRef->pElem->nId);
                    gslc_ElemXListboxAddItem(pGui, pElemRef, (const char*)data + 1);
                }
                else if (action == 'O') {
                    uint16_t itemId = hmiReadCommand.data[1] << 8 | hmiReadCommand.data[2];
                    gslc_DebugPrintf("Overriding item %d in Listbox elem %d\r\n", itemId, pElemRef->pElem->nId);
                    gslc_ElemXListboxDeleteItemAt(pGui, pElemRef, itemId);
                    if (!gslc_ElemXListboxInsertItemAt(pGui, pElemRef, itemId, ((const char*)hmiReadCommand.data) + 3)) {
                        gslc_DebugPrintf("ERROR: Failed to override item %d in Listbox elem %d\r\n", itemId, pElemRef->pElem->nId);
                    }
                }
                else if (action == 'S') {
                    int16_t itemId = hmiReadCommand.data[1] << 8 | hmiReadCommand.data[2];
                    gslc_ElemXListboxSetSel(pGui, pElemRef, itemId);
                }
                break;
            }
            case GSLC_TYPEX_TEXTBOX:
                gslc_ElemXTextboxReset(pGui, pElemRef);
                gslc_ElemXTextboxAdd(pGui, pElemRef, (char*)hmiReadCommand.data);
                break;
            case GSLC_TYPEX_SLIDER: {
                int16_t newPos = hmiReadCommand.data[0] << 8 | hmiReadCommand.data[1];
                gslc_ElemXSliderSetPos(pGui, pElemRef, newPos);
                break;
            }
            case GSLC_TYPEX_SEEKBAR: {
                int16_t newPos = hmiReadCommand.data[0] << 8 | hmiReadCommand.data[1];
                gslc_ElemXSeekbarSetPos(pGui, pElemRef, newPos);
                break;
            }
            case GSLC_TYPEX_SPINNER: {
                int16_t newPos = hmiReadCommand.data[0] << 8 | hmiReadCommand.data[1];
                gslc_ElemXSpinnerSetCounter(pGui, pElemRef, newPos);
                break;
            }
            case GSLC_TYPEX_RING: {
                int16_t newPos = hmiReadCommand.data[0] << 8 | hmiReadCommand.data[1];
                gslc_ElemXRingGaugeSetVal(pGui, pElemRef, newPos);
                break;
            }
            case GSLC_TYPEX_RADIAL: {
                int16_t newPos = hmiReadCommand.data[0] << 8 | hmiReadCommand.data[1];
                gslc_ElemXRadialSetVal(pGui, pElemRef, newPos);
                break;
            }
            case GSLC_TYPEX_PROGRESS: {
                int16_t newPos = hmiReadCommand.data[0] << 8 | hmiReadCommand.data[1];
                gslc_ElemXProgressSetVal(pGui, pElemRef, newPos);
                break;
            }
            case GSLC_TYPEX_RAMP: {
                int16_t newPos = hmiReadCommand.data[0] << 8 | hmiReadCommand.data[1];
                gslc_ElemXRampSetVal(pGui, pElemRef, newPos);
                break;
            }
            case GSLC_TYPEX_GRAPH: {
                char action = hmiReadCommand.data[0];
                if (action == 'C') {
                    gslc_DebugPrintf("Clearing Graph elem %d\r\n", pElemRef->pElem->nId);
                    gslc_ElemXGraphClear(pGui, pElemRef);
                }
                else if (action == 'A') {
                    gslc_DebugPrintf("Adding item to Graph elem %d\r\n", pElemRef->pElem->nId);
                    int16_t newVal = hmiReadCommand.data[1] << 8 | hmiReadCommand.data[2];
                    gslc_ElemXGraphAdd(pGui, pElemRef, newVal);
                }

                break;
            }
#endif
            case GSLC_TYPE_TXT:
            case GSLC_TYPE_BTN:
            default:
            {
                gslc_ElemSetTxtStr(pGui, pElemRef, (char*)hmiReadCommand.data);
                break;
            }
            }
            break;
        }
        case GET_VALUE: // 0x04 Get value of element values
        { // 0x05 Get data
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
            switch (hmiReadCommand.data[0]) {
            case 0x00: // Reset
                #ifdef ARDUINO
                gslc_DebugPrintf("RESET");
                delay(50);
                HAL_NVIC_SystemReset();
                #else
                gslc_DebugPrintf("NOT SUPPORTED");
                #endif
                break;
            case 0x02: // Refresh
                //gslc_DebugPrintf("REFRESH");
                gslc_PageRedrawSet(pGui, true);
                break;
            }
            break;
        default:
            gslc_DebugPrintf("Command 0x%x is not known\r\n", command);
            break;
        }
        if (hmiReadCommand.data != 0)
            free(hmiReadCommand.data);// Make sure to free data;
        // We are done handling serial so we clear the buffer
        memset(buffer, 0, 255);
        memset(&hmiReadCommand, 0, sizeof(hmiCommand_s));
        bufferIndex = 0;
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