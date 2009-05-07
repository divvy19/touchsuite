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

#include "touch_tracer.h"

namespace touchsuite {

touch_tracer::touch_tracer()
{
	c[0].R = 1.0f; c[0].G = 0.0f; c[0].B = 0.0f;	// Red
	c[1].R = 0.0f; c[1].G = 1.0f; c[1].B = 0.0f;	// Green
	c[2].R = 0.0f; c[2].G = 0.0f; c[2].B = 1.0f;	// Blue		
	c[3].R = 1.0f; c[3].G = 0.0f; c[3].B = 1.0f;	// Magenta
	c[4].R = 1.0f; c[4].G = 1.0f; c[4].B = 0.0f;	// Yellow
	c[5].R = 0.0f; c[5].G = 1.0f; c[5].B = 1.0f;	// Cyan
	c[6].R = 1.0f; c[6].G = 0.5f; c[6].B = 1.0f;	// Orange

	current_colour = 0;	

	gridsize = 100;
	gridsize_s = 10;

	sync_touchdata = new OpenThreads::Mutex();
}

touch_tracer::~touch_tracer()
{
	delete sync_touchdata;
}

void touch_tracer::addTuioObject(TuioObject *tobj)
{
	// ignore fiducial marker object
}

void touch_tracer::updateTuioObject(TuioObject *tobj)
{
	// ignore fiducial marker object
}

void touch_tracer::removeTuioObject(TuioObject *tobj)
{
	// ignore fiducial marker object
}

void touch_tracer::addTuioCursor(TuioCursor *tcur)
{
	sync_touchdata->lock();	
		trace_tuio[(int)tcur->getSessionID()][0].X = tcur->getX();
		trace_tuio[(int)tcur->getSessionID()][0].Y = tcur->getY();
		blob_colour_tuio[(int)tcur->getSessionID()] = current_colour;
	sync_touchdata->unlock();

	++current_colour;
	if(current_colour > 6)
		current_colour = 0;
}

void touch_tracer::updateTuioCursor(TuioCursor *tcur)
{
	std::map<int, Point>::const_iterator iter;
	
	sync_touchdata->lock();
		iter = --trace_tuio[(int)tcur->getSessionID()].end();
		trace_tuio[(int)tcur->getSessionID()][iter->first + 1].X = tcur->getX();
		trace_tuio[(int)tcur->getSessionID()][iter->first + 1].Y = tcur->getY();		
	sync_touchdata->unlock();
}

void touch_tracer::removeTuioCursor(TuioCursor *tcur)
{
	sync_touchdata->lock();
		blob_colour_tuio.erase((int)tcur->getSessionID());
		trace_tuio.erase((int)tcur->getSessionID());	
	sync_touchdata->unlock();
}

void touch_tracer::refresh(TuioTime bundleTime)
{
	// empty
}

void touch_tracer::draw_scene(GLenum mode)
{	
	// Sync copy
	sync_touchdata->lock();
		blob_colour_gl = blob_colour_tuio;
		trace_gl = trace_tuio;
	sync_touchdata->unlock();

	if(display_grid)
		render_grid();

	debug_info();

	// Should we bother to paint?
	if(trace_gl.size())
	{		
		// Paint trace
		std::map<int, std::map<int, Point> >::iterator iter1;		
		std::map<int, Point>::iterator iter2;	
		
		// Run through blobs
		float alpha;
		const int SEGMENTS = 24;
		const float DEG2RAD = 2 * 3.14159 / SEGMENTS;
		const float RADIUS = 15.0;

		for(iter1 = trace_gl.begin(); iter1 != trace_gl.end(); ++iter1)
		{	
			// Fat tail
			glLineWidth(5.5);
			alpha = 0.0;

			glBegin(GL_LINE_STRIP);					
			for(iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2)
			{
				// Use the colour lookup table...
				glColor4f(c[blob_colour_gl[iter1->first]].R, c[blob_colour_gl[iter1->first]].G, c[blob_colour_gl[iter1->first]].B, alpha);	
				glVertex2f(trace_gl[iter1->first][iter2->first].X * screen_width, trace_gl[iter1->first][iter2->first].Y * screen_height);					
				alpha += 0.1;
			}		
			glEnd();				

			// Create a circle			
			glColor4f(c[blob_colour_gl[iter1->first]].R, c[blob_colour_gl[iter1->first]].G, c[blob_colour_gl[iter1->first]].B, alpha);
			glLineWidth(2.0);
			alpha = 1.0;

			iter2 = --iter1->second.end();						
			glBegin(GL_LINE_LOOP);
 			for (int i = 0; i < SEGMENTS; ++i)
			{
				float degInRad = i * DEG2RAD;
				glVertex2f(iter2->second.X * screen_width + cos(degInRad) * RADIUS, iter2->second.Y * screen_height + sin(degInRad) * RADIUS);
			} 
			glEnd();	

			// Draw info label
			draw_info(iter1->first, iter2->second.X, iter2->second.Y);
		}	

		sync_touchdata->lock();
		for(iter1 = trace_tuio.begin(); iter1 != trace_tuio.end(); ++iter1)
		{
			// Erase first state of current blobs if list is larger than 10
			if((int)trace_tuio[iter1->first].size() > 10)
			{				
				std::map<int, Point>::iterator iter3;
				iter3 = trace_tuio[iter1->first].begin();
				trace_tuio[iter1->first].erase(iter3->first);				
			}
		}
		sync_touchdata->unlock();
	}
}

void touch_tracer::render_grid()
{
	#if defined(WIN32)
	int x_first = 1;
	#else
	int x_first = 0;
	#endif

	glLineWidth(1.0);
	glBegin(GL_LINES);
		// small blocks
		glColor3f(0.0f, 0.0f, 0.2f);

		for(int x=x_first; x <= screen_width; x += gridsize_s)
		{
			glVertex2f(x, 1);
			glVertex2f(x, screen_height);			
		}
		for(int y=0; y <= screen_height; y += gridsize_s)
		{
			glVertex2f(0, y);
			glVertex2f(screen_width, y);			
		}

		// large blocks
		glColor3f(0.0f, 0.0f, 0.5f);
		for(int x=x_first; x <= screen_width; x += gridsize)
		{
			glVertex2f(x, 1);
			glVertex2f(x, screen_height);			
		}
		for(int y=0; y <= screen_height; y += gridsize)
		{
			glVertex2f(0, y);
			glVertex2f(screen_width, y);			
		}
	glEnd();
}

void touch_tracer::debug_info()
{
	char buffer[64];	
	static int frame = 0, time, timebase=0;
	static float currentfps;

	++frame;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) 
	{
		currentfps = frame * 1000.0 / (time - timebase);
		timebase = time;		
		frame = 0;
	}

	glColor4f(1.0, 1.0, 1.0, 1.0);
	sprintf(buffer, "Touch Tracer v%s by Laurence Muller", TTVERSION);   // dont change please :)
	render_bitmap_character(10, 22, buffer);
	sprintf(buffer, "Average FPS: %4.2f", currentfps);   
 	render_bitmap_character(10, 38, buffer);
	sprintf(buffer, "Display settings: %dx%d %db @ %dHz", screen_width, screen_height, bit_depth, refresh_rate);   	
	render_bitmap_character(10, 54, buffer);		
	
	sprintf(buffer, "Number of touches: %d", trace_gl.size());   
	render_bitmap_character(10, screen_height - 16, buffer);	
	sprintf(buffer, "www.multigesture.net");   
	render_bitmap_character(screen_width - 180, screen_height - 16, buffer);	
}

void touch_tracer::render_bitmap_character(int x, int y, char *string)
{
	char *c;
	glRasterPos2i(x, y);
	
	for (c=string; *c != '\0'; c++) 
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

void touch_tracer::draw_info(int ID, float X, float Y)
{
	glLineWidth(1.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);			
		glVertex2f((X * screen_width)	  , (Y * screen_height)	);
		glVertex2f((X * screen_width) + 16, (Y * screen_height) + 16);
		glVertex2f((X * screen_width) + 16, (Y * screen_height) + 16);
		glVertex2f((X * screen_width) + 16, (Y * screen_height) + 52);
	glEnd( );		
			
	char buffer[64];	
	sprintf(buffer, "ID #%d", ID);   
	render_bitmap_character((int)(X * screen_width) + 17, (int)(Y * screen_height) + 26, buffer);
	sprintf(buffer, "X = %d", (int)(X * screen_width));   
	render_bitmap_character((int)(X * screen_width) + 17, (int)(Y * screen_height) + 38, buffer);
	sprintf(buffer, "Y = %d", (int)(Y * screen_height));   
	render_bitmap_character((int)(X * screen_width) + 17, (int)(Y * screen_height) + 50, buffer); 
}

}
