// GUIslice_hmi.h

#ifndef _GUISLICE_HMI_h
#define _GUISLICE_HMI_h

#ifdef __ARDUINO__
#include <Arduino.h>
#endif

#include "GUIslice.h"


// uint8_t buffer[HMI_SERIAL_BUFFER_SIZE]; // User configurable so as not to strain memory
// uint16_t currentBufferIndex = 0; // Used to hold the index of the buffer

enum OBJECT_ELEM_DATA_FIELDS { // This holds the different data fields that a element can hold
	BACKGROUND_COLOR = 0,
	FOREGROUND_COLOR = 1,
	WIDTH = 2,
	HEIGHT = 3,
	XPOS = 4,
	YPOS = 5,
	MINVALUE = 6,
	MAXVALUE = 7,
};

// WHEN SENDING FROM MCU TO DISPLAY IT IS VALUE + 0x00. WHEN SENDING FROM DISPLAY TO MCU IT IS VALUE + 0x20
enum HMI_SERIAL_COMMANDS {
	SET_DATA = 0x00, // ELEMENT DATA(BG/X/Y/Width/Height/ETC)
	GET_DATA = 0x01,
	EVENT = 0x02,
	PAGE = 0x03,
	SET_VALUE = 0x04, // ELEMENT VALUE(Text/Checkbox State/Slider value)
	GET_VALUE = 0x05,
	CMD = 0x06
};

enum HMI_SERIAL_CMD_COMMANDS {
	REBOOT = 0x00,
	STARTED = 0x01,
	REFRESH = 0x02,
};

typedef struct {
	uint8_t header;			// Header byte.
	uint8_t packetNumber;	// PacketNumber
	uint16_t size;			// complete packet size
	uint16_t dataSize;		// DataSize
	uint8_t command;		// HMICommand
	int16_t elementId;		// ElementId. We directly parse it here
	uint8_t* data;			// Datapointer. MAKE SURE TO FREE IT Otherwisae memory leak city
	uint8_t crc;			// CRC
	uint8_t footer;			// Footer byte.
} hmiCommand_s;

#define HMI_BUFFER_ERROR (uint8_t[]){0xA0,0xFF,0xFF,0xFF}

void gslc_hmi_sendTouchDown(void* pvGui, void* pvElemRef);
void gslc_hmi_sendTouchUp(void* pvGui, void* pvElemRef);
void gslc_hmi_sendTouchMove(void* pvGui, void* pvElemRef);
void gslc_hmi_sendPageLoaded(void* pvGui, int16_t pageId);
void gslc_hmi_sendPageUnloaded(void* pvGui, int16_t pageId);
void gslc_hmi_sendValueChanged(void* pvGui, void* pvElemRef);
void gslc_hmi_sendCmd(uint8_t cmd);
void gslc_hmi_sendCmdWithData(uint8_t cmd,uint8_t* data, uint8_t dataLength);

void gslc_hmi_read_packet();
void gslc_hmi_write_packet(hmiCommand_s hmiCommand);
void gslc_hmi_loop(void* pvGui);
#endif