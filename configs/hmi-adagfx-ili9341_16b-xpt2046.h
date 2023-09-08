#ifndef _GUISLICE_CONFIG_ARD_H_
#define _GUISLICE_CONFIG_ARD_H_

// =============================================================================
// GUIslice library (example user configuration #???) for:
//   - CPU:     STM32
//   - Display: ILI9341 16-bit
//   - Touch:   XPT2046
//   - Wiring:  Manual wiring
//              - Pinout:
//                  CPU     TFT      Touch     SD
//                  ----    -------  --------  -----
//
//
//   - Example display:
//     - 
//
// DIRECTIONS:
// - To use this example configuration, include in "GUIslice_config.h"
//
// WIRING:
// - The pinout configuration may need to be modified to match your wiring
//
// =============================================================================
// - Calvin Hass
// - https://github.com/ImpulseAdventure/GUIslice
// =============================================================================
//
// The MIT License
//
// Copyright 2016-2019 Calvin Hass
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================
// \file GUIslice_config_ard.h

// =============================================================================
// User Configuration
// - This file can be modified by the user to match the
//   intended target configuration
// =============================================================================

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


	// =============================================================================
	// USER DEFINED CONFIGURATION
	// =============================================================================

	// -----------------------------------------------------------------------------
	// Device Mode Selection
	// - The following defines the display and touch drivers
	//   and should not require modifications for this example config
	// -----------------------------------------------------------------------------
#define DRV_DISP_ADAGFX				   // Adafruit-GFX library	
#define DRV_DISP_ADAGFX_ILI9341_16BIT  // Adafruit ILI9341_16b    (StarlightNL/Adafruit_TFTLCD_16bit_STM32)
#define DRV_TOUCH_XPT2046_PS           // XPT2046 touch driver    

// -----------------------------------------------------------------------------
// Pinout
// -----------------------------------------------------------------------------
#define TFT_DATA_PORT     GPIOB
#define TFT_CNTRL_PORT    GPIOC // must be common to all control pins
#define TFT_WR_PIN        PC1
#define TFT_RS_PIN        PC2
#define TFT_CS_PIN        PC0
#define TFT_RST_PIN       PC4

// SD Card
#define ADAGFX_PIN_SDCS   PA3     // SD card chip select (if GSLC_SD_EN=1)


// -----------------------------------------------------------------------------
// Orientation
// -----------------------------------------------------------------------------

// Set Default rotation of the display
// - Values 0,1,2,3. Rotation is clockwise
// - Note that changing this value may require a change
//   to GSLC_TOUCH_ROTATE as well to ensure the touch screen
//   orientation matches the display.
#define GSLC_ROTATE     3

// Set Default rotation of the touch overlay
// - Values 0,1,2,3. Rotation is clockwise
#define GSLC_TOUCH_ROTATE 3

// -----------------------------------------------------------------------------
// Touch Handling
// - Documentation for configuring touch support can be found at:
//   https://github.com/ImpulseAdventure/GUIslice/wiki/Configure-Touch-Support
// -----------------------------------------------------------------------------

// Touch bus & pinout
#define XPT2046_CS PA4

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// SECTION 4B: Update your calibration settings here
// - These values should come from the diag_ard_touch_calib sketch output
// - Please update the values to the right of ADATOUCH_X/Y_MIN/MAX_* accordingly
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Calibration settings from diag_ard_touch_calib:
// DRV_TOUCH_XPT2046:
#define ADATOUCH_X_MIN    3902
#define ADATOUCH_X_MAX    139
#define ADATOUCH_Y_MIN    362
#define ADATOUCH_Y_MAX    3948
#define ADATOUCH_REMAP_YX 0    // Some touch controllers may swap X & Y coords

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// SECTION 4D: Additional touch configuration
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Define pressure threshold for detecting a touch
// - Specifying this range helps eliminate some erroneous touch events
//   resulting from noise in the touch overlay detection
// - For config details, please see:
//   https://github.com/ImpulseAdventure/GUIslice/wiki/Configuring-Touch-Pressure
#define ADATOUCH_PRESS_MIN  200
#define ADATOUCH_PRESS_MAX  4000
// -----------------------------------------------------------------------------
// Diagnostics
// -----------------------------------------------------------------------------

// Error reporting
// - Set DEBUG_ERR to >0 to enable error reporting via the Serial connection
// - Enabling DEBUG_ERR increases FLASH memory consumption which may be
//   limited on the baseline Arduino (ATmega328P) devices.
//   - DEBUG_ERR 0 = Disable all error messaging
//   - DEBUG_ERR 1 = Enable critical error messaging (eg. init)
//   - DEBUG_ERR 2 = Enable verbose error messaging (eg. bad parameters, etc.)
// - For baseline Arduino UNO, recommended to disable this after one has
//   confirmed basic operation of the library is successful.
#define DEBUG_ERR               2   // 1,2 to enable, 0 to disable

// Debug initialization message
// - By default, GUIslice outputs a message in DEBUG_ERR mode
//   to indicate the initialization status, even during success.
// - To disable the messages during successful initialization,
//   uncomment the following line.
//#define INIT_MSG_DISABLE

// -----------------------------------------------------------------------------
// Optional Features
// -----------------------------------------------------------------------------

// Enable of optional features
// - For memory constrained devices such as Arduino, it is best to
//   set the following features to 0 (to disable) unless they are
//   required.
#define GSLC_FEATURE_COMPOUND       1   // Compound elements (eg. XSelNum)
#define GSLC_FEATURE_XGAUGE_RADIAL  1   // XGauge control with radial support
#define GSLC_FEATURE_XGAUGE_RAMP    1   // XGauge control with ramp support
#define GSLC_FEATURE_XTEXTBOX_EMBED 1   // XTextbox control with embedded color
#define GSLC_FEATURE_INPUT          1   // Keyboard / GPIO input control

// Enable support for SD card
// - Set to 1 to enable, 0 to disable
// - Note that the inclusion of the SD library consumes considerable
//   RAM and flash memory which could be problematic for Arduino models
//   with limited resources.
#define GSLC_SD_EN    0
// Define the type of SD communication
// #define GSLC_SD_SPI
// #define GSLC_SD_SDIO

// Enable support for HMI
#define GSLC_HMI_ENABLE

// =============================================================================
// INTERNAL CONFIGURATION
// - The following settings should not require modification by users
// =============================================================================

// -----------------------------------------------------------------------------
// Touch Handling
// -----------------------------------------------------------------------------

// Define how touch orientation changes with display orientation
#define TOUCH_ROTATION_DATA 0x6350
#define TOUCH_ROTATION_SWAPXY(rotation) ((( TOUCH_ROTATION_DATA >> ((rotation&0x03)*4) ) >> 2 ) & 0x01 )
#define TOUCH_ROTATION_FLIPX(rotation)  ((( TOUCH_ROTATION_DATA >> ((rotation&0x03)*4) ) >> 1 ) & 0x01 )
#define TOUCH_ROTATION_FLIPY(rotation)  ((( TOUCH_ROTATION_DATA >> ((rotation&0x03)*4) ) >> 0 ) & 0x01 )

// - Set any of the following to 1 to perform touch display
//   remapping functions, 0 to disable. Use DBG_TOUCH to determine which
//   remapping modes should be enabled for your display
// - Please refer to "docs/GUIslice_config_guide.xlsx" for detailed examples
// - NOTE: Both settings, GLSC_TOUCH_ROTATE and SWAP / FLIP are applied, 
//         try to set _SWAP_XY and _FLIP_X/Y to 0 and only use GLSC_TOUCH_ROTATE
#define ADATOUCH_SWAP_XY  0
#define ADATOUCH_FLIP_X   0
#define ADATOUCH_FLIP_Y   0

// Define the maximum number of touch events that are handled
// per gslc_Update() call. Normally this can be set to 1 but certain
// displays may require a greater value (eg. 30) in order to increase
// responsiveness of the touch functionality.
#define GSLC_TOUCH_MAX_EVT    1

// -----------------------------------------------------------------------------
// Misc
// -----------------------------------------------------------------------------

// Define buffer size for loading images from SD
// - A larger buffer will be faster but at the cost of RAM
#define GSLC_SD_BUFFPIXEL   50

// Enable support for graphics clipping (DrvSetClipRect)
// - Note that this will impact performance of drawing graphics primitives
#define GSLC_CLIP_EN 1

// Enable for bitmap transparency and definition of color to use
#define GSLC_BMP_TRANS_EN     1               // 1 = enabled, 0 = disabled
#define GSLC_BMP_TRANS_RGB    0xFF,0x00,0xFF  // RGB color (default:pink)

#define GSLC_USE_FLOAT      0   // 1=Use floating pt library, 0=Fixed-point lookup tables

#define GSLC_DEV_TOUCH ""
#define GSLC_USE_PROGMEM 1

#define GSLC_LOCAL_STR      1   // 1=Use local strings (in element array), 0=External
#define GSLC_LOCAL_STR_LEN  30  // Max string length of text elements

// -----------------------------------------------------------------------------
// Debug diagnostic modes
// -----------------------------------------------------------------------------
// - Uncomment any of the following to enable specific debug modes
//#define DBG_LOG           // Enable debugging log output
//#define DBG_TOUCH         // Enable debugging of touch-presses
//#define DBG_FRAME_RATE    // Enable diagnostic frame rate reporting
//#define DBG_DRAW_IMM      // Enable immediate rendering of drawing primitives
//#define DBG_DRIVER        // Enable graphics driver debug reporting


// =============================================================================

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GUISLICE_CONFIG_ARD_H_