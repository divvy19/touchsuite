//////////////////////////////////////////////////////////////////////////////////////////
// Project description
// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// Name: TouchlibCOMwrapper
// Purpose: A touchlib wrapper for C#
//
// Author: Laurence Muller (aka Falcon4ever)
// Contact: info@falcon4ever.com
// Site: www.multigesture.net
// Licensetype: GNU General Public License v2
//
//////////////////////////////////////////////////////////////////////////////////////////

// General headers
#ifdef WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#include <windows.h>
#endif

// Touchlib (set path)
#include "TouchScreenDevice.h"
#include "TouchData.h"
#ifdef WIN32
  #ifdef _DEBUG
     #pragma comment(lib, "touchlibd.lib")
   #else
     #pragma comment(lib, "touchlib.lib")
   #endif  
#endif

// OpenCV
#include <cv.h>
#include <highgui.h>
#ifdef WIN32
	#pragma comment(lib, "cv.lib")
	#pragma comment(lib, "cxcore.lib")
	#pragma comment(lib, "highgui.lib")
#endif

// Set used namespaces
using namespace std;
using namespace touchlib;

// Import tlb files
#import "TouchlibCOMwrapper.tlb"

// Export
#if defined(__cplusplus)
extern "C" {
#endif
	
#define EXPORT						__declspec(dllexport)
#define CALL						__cdecl

EXPORT void CALL init_touchlib(bool debug, TouchlibCOMwrapper::COM_ITouchListener *listener);
EXPORT void CALL kill_touchlib();
EXPORT void CALL getEvents();
EXPORT void CALL recapture_background();
EXPORT void CALL auto_rectify();

#if defined(__cplusplus)
}
#endif