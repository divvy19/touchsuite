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
// more info can be found TouchlibCOMwrapper dir (readme_COM.txt)
// and in the TouchlibCOMwrapper_Csharp_example directory
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// Include files
// ¯¯¯¯¯¯¯¯¯¯¯¯¯
#include "TouchlibCOMwrapper.h"

// Wrapper
class ComAdapter : public ITouchListener
{
	public:
		TouchlibCOMwrapper::COM_ITouchListener* listener;

		//! Notify that a finger has just been made active. 
		virtual void fingerDown(TouchData data)
		{
			//printf("fingerDown C++\n");
			listener->fingerDown(*(TouchlibCOMwrapper::TouchData*)&data);			
		}

		//! Notify that a finger has just been made active. 
		virtual void fingerUpdate(TouchData data)
		{
			//printf("fingerUpdate C++\n");
			listener->fingerUpdate(*(TouchlibCOMwrapper::TouchData*)&data);
		}

		//! A finger is no longer active..
		virtual void fingerUp(TouchData data)
		{
			//printf("fingerUp C++\n");
			listener->fingerUp(*(TouchlibCOMwrapper::TouchData*)&data);
		}
};

// Global variables
ITouchScreen *screen;	// TouchScreen device
ComAdapter myapp;		// Your application
string bgLabel1;
string recLabel1;

// DLL
HINSTANCE g_hInstance = NULL;

BOOL APIENTRY DllMain(	HINSTANCE hinstDLL,	// DLL module handle
						DWORD dwReason,		// reason called
						LPVOID lpvReserved)	// reserved
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			printf("Loading TouchlibCOMwrapper v0.1\n");
			g_hInstance = hinstDLL;
		}
			break;

		case DLL_PROCESS_DETACH:
		{
			printf("Unloading TouchlibCOMwrapper\n");
			kill_touchlib();
		}
		break;

		default:
			break;
	}
	
	return TRUE;
}

void init_touchlib(bool debug, TouchlibCOMwrapper::COM_ITouchListener *listener)
{
	// Setup Touchlib
	screen = TouchScreenDevice::getTouchScreen();	// Assign the TouchScreen device
	screen->setDebugMode(debug);	// Enable/Disable touchlib debug mode, on default its set to true.

	if(!screen->loadConfig("config.xml"))	// Setup default values in no config.xml is available.
	{		
		string label;
		label = screen->pushFilter("dsvlcapture");
		screen->setParameter(label, "source", "cam");
		screen->pushFilter("mono");
		screen->pushFilter("smooth");
		screen->pushFilter("backgroundremove");
		label = screen->pushFilter("brightnesscontrast");
			screen->setParameter(label, "brightness", "0.1");
			screen->setParameter(label, "contrast", "0.4");		
		label = screen->pushFilter("rectify");
			screen->setParameter(label, "level", "90");		
		screen->saveConfig("config.xml");
	}

	string bgLabel = screen->findFirstFilter("backgroundremove");
	string recLabel = screen->findFirstFilter("rectify");
	bgLabel1 = bgLabel;
	recLabel1 = recLabel;

	myapp.listener = listener;			// Attach our wrapper
	screen->registerListener(&myapp);	// Attach our application
	
	SLEEP(1000);
	screen->setParameter(bgLabel1, "mask", (char*)screen->getCameraPoints());
	screen->setParameter(bgLabel1, "capture", "");
	
	// Note: Begin processing should only be called after the screen is set up
	screen->beginProcessing();
	screen->beginTracking();
}

void kill_touchlib()
{
	TouchScreenDevice::destroy();
}

void getEvents()
{		
	int keypressed = cvWaitKey(32) & 255;
	screen->getEvents();		// Update environment
}

void recapture_background()
{	
	screen->setParameter(bgLabel1, "capture", "");
}

void auto_rectify()
{
	screen->setParameter(recLabel1, "level", "auto");
}