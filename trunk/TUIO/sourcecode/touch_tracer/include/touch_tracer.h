//////////////////////////////////////////////////////////////////////////////////////////
// Project description
// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// Name: Touch Tracer
// Purpose: Multi-touch test application
//
// Author: Laurence Muller (aka Falcon4ever)
// Contact: lmuller@multigesture.net
// Site: www.multigesture.net
// License: GNU General Public License (GPL) v2 ( http://www.gnu.org/licenses/old-licenses/gpl-2.0.html )
//
// Read main.cpp for project details
//
// Copyright (C) 2007-2009 Laurence Muller
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef TOUCH_TRACER_H_
#define TOUCH_TRACER_H_

#ifdef WIN32	
	#define _CRT_SECURE_NO_DEPRECATE	
#endif

// Version settings
#define TTVERSION "0.4"
#define RELDAY "07"
#define RELMONTH "05"
#define RELYEAR "2009"

// TUIO
#include "TuioListener.h"
#include "TuioClient.h"

#include <math.h>
#include <map>
#include <vector>

// Glut/OpenGL
#if defined(WIN32)
	#include <glut.h>
#elif defined(__APPLE__)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <OpenThreads/Mutex>

using namespace TUIO;

namespace touchsuite {

// Blob/Point structure
struct Point
{
	float X;
	float Y;
};

// RGB color struct (blobs)
struct _RGB
{
	float R;
	float G;
	float B;
};

class touch_tracer : public TuioListener {
	
	public:
		touch_tracer();
		~touch_tracer();

		void addTuioObject(TuioObject *tobj);
		void updateTuioObject(TuioObject *tobj);
		void removeTuioObject(TuioObject *tobj);

		void addTuioCursor(TuioCursor *tcur);
		void updateTuioCursor(TuioCursor *tcur);
		void removeTuioCursor(TuioCursor *tcur);

		void refresh(TuioTime bundleTime);

		void draw_scene(GLenum mode);
		void render_grid();

		int screen_width, screen_height, bit_depth, refresh_rate;		
		bool display_grid, smooth_grid;
		
	private:
		void debug_info();
		void render_bitmap_character(int x, int y, char *string);
		void draw_info(int ID, float X, float Y);

		_RGB c[7];
		int current_colour, gridsize, gridsize_s;

		// TUIO thread
		std::map<int, int> blob_colour_tuio;
		std::map<int, std::map<int, Point> > trace_tuio;

		// Mutex lock
		OpenThreads::Mutex *sync_touchdata;

		// GL thread
		std::map<int, int> blob_colour_gl;
		std::map<int, std::map<int, Point> > trace_gl;
};

}

#endif /* TOUCH_TRACER_H_ */
