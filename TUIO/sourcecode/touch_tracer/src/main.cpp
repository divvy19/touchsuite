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
// Last update: May 2009
// SVN repository: http://code.google.com/p/touchsuite/
//
// Changes:
// v0.4 - Rewritten to make it independent from Touchlib (using TUIO interface) (May 2009)
// v0.3 - Misc. fixes (02 april 2008)
// v0.2.1 - Blob structed changed, fixed compatibility. (30 july 2007)
// v0.2 - Added trace lines, blob colours and general code cleanup (linux compatibility) (12 may 2007).
// v0.1 - Initial release (02 may 2007).
//
// Todo:
// - Got suggestions?
//
// Copyright (C) 2007-2009 Laurence Muller
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
	#ifdef _DEBUG
		#define WINDOWED_MODE
	#endif  
#endif

#include "main.h"

using namespace std;
using namespace touchsuite;

touch_tracer tt;

int main(int argc, char **argv) 
{
	printf("Touch Tracer v"TTVERSION" by Laurence Muller\n\n"); 

	// TUIO Port
	int port = 3333;
	if( argc >= 2 ) 
		port = atoi( argv[1] );

	// Setup TUIO and bind with main application
	TuioClient client(port);
	client.addTuioListener(&tt);
	client.connect(false);

	// Setup GLUT
	glutInit(&argc, argv);		
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Window settings (hardcoded values for now)
	tt.screen_width = 1024;
	tt.screen_height = 768;
	tt.bit_depth = 32;
	tt.refresh_rate = 60;

#ifdef WINDOWED_MODE
	// Windowed mode
	glutInitWindowSize(tt.screen_width, tt.screen_height);
	glutInitWindowPosition(50, 70);
	glutCreateWindow("Touch Tracer v"TTVERSION);
#else
	// Game mode (Set display mode)
	char displaymode[24];
	sprintf(displaymode, "%dx%d:%d@%d", tt.screen_width, tt.screen_height, tt.bit_depth, tt.refresh_rate);	
	glutGameModeString(displaymode);
	
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
	{
		glutEnterGameMode();		
	}
	else 
	{
		printf("The select mode is not available\n");
		exit(1);	
	}
#endif

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape_window);	
	glutKeyboardFunc(keyboard);

	init_opengl();
	glutMainLoop(); 

	return 0;
}


void init_opengl()
{
	tt.display_grid = true;
	tt.smooth_grid = true;

	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);		// Blue Background	
	glClearDepth(1.0f);							// Depth Buffer Setup	

	glEnable(GL_BLEND); 
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);			// Blending Function For Translucency Based On Source Alpha Value

	if(tt.smooth_grid)
	{
		glEnable(GL_LINE_SMOOTH);				// Nice lines :) (horizontal grid lines won't be drawn in linux)
		glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
	}
}

void display()
{
	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw scene
	tt.draw_scene(GL_RENDER);
	
	// Swap buffers!
	glutSwapBuffers();		
}


void reshape_window(GLsizei w, GLsizei h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, w, h, 0);
	
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	tt.screen_width = w;
	tt.screen_height = h;
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27 || key == 'q')	// esc
	{
		exit(0);
	} 	
	else if( key == 'g')				// g = display grid
	{
		tt.display_grid = !tt.display_grid;
		return;
	} 
	else if( key == 's')				// s = smooth lines
	{		
		tt.smooth_grid = !tt.smooth_grid;

		if(tt.smooth_grid)
		{
			glEnable(GL_LINE_SMOOTH);					// Nice lines :) (horizontal grid lines won't be drawn in linux)
			glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		}
		else
		{
			glDisable(GL_LINE_SMOOTH);
		}
	}
}
