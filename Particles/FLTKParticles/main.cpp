

#pragma comment(lib, "fltkd.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "fltkgld.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glu.h>

#include "Types.h"
#include "ParticleSystem.h"
#include "Environment.h"


void IdleCallback(void* pData);

//
// Simple resizable 2D GL window
// erco 10/08/05
//

class MyGlWindow : public Fl_Gl_Window {
    // DRAW METHOD
    //      OpenGL window: (w,h) is upper right, (-w,-h) is lower left, (0,0) is center
    //
    void draw() {
        // First time? init viewport, etc.
        if (!valid()) {
            valid(1);
            
			float ratio = 1.0f* w() / h();

			// Use the Projection Matrix
			glMatrixMode(GL_PROJECTION);

			// Reset Matrix
			glLoadIdentity();

			// Set the viewport to be the entire window
			glViewport(0, 0, w(), h());

			// Set the correct perspective.
			gluPerspective(45,ratio,1,1000);

			// Get Back to the Modelview
			glMatrixMode(GL_MODELVIEW);

			glLoadIdentity();

			gluLookAt(camera.x, camera.y, camera.z,
			  0.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);
        }
        
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);


		// Draw the floor plane
		glBegin(GL_QUADS);
			glColor4f(0.0f, 0.1f, 0.1f, 0.5f);
			glVertex3f(-10.0f, 0.0f, 10.0f);
			glVertex3f(10.0f, 0.0f, 10.0f);
			glVertex3f(10.0f, 0.0f, -10.0f);
			glVertex3f(-10.0f, 0.0f, -10.0f);
		glEnd();

		particles.Render(&env, camera);

		glPopMatrix();

		swap_buffers();
    }
    // HANDLE WINDOW RESIZING
    //    If window reshaped, need to readjust viewport/ortho
    //
    void resize(int x,int y,int w,int h) {
        Fl_Gl_Window::resize(x,y,w,h);
        
		if(h == 0)
			h = 1;
		float ratio = 1.0f* w / h;

		// Use the Projection Matrix
		glMatrixMode(GL_PROJECTION);

		// Reset Matrix
		glLoadIdentity();

		// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

		// Set the correct perspective.
		gluPerspective(45,ratio,1,1000);

		// Get Back to the Modelview
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();

		gluLookAt(camera.x, camera.y, camera.z,
			  0.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	
		env = CEnvironment(w);

        redraw();
    }


private:
	CParticleSystem particles;
	CEnvironment env;
	Vector camera;

public:

	void UpdateScene(void)
	{
		particles.Spawn();
		redraw();
	}


    // CONSTRUCTOR
    MyGlWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {
		mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);

		camera.x = 0.0f;
		camera.y = 0.5f;
		camera.z = 7.0f;


		env = CEnvironment(W);

		glEnable( GL_DEPTH_TEST );

		Fl::add_idle(IdleCallback, this);
    }
};

void IdleCallback(void* pData)
{
 if (pData != NULL)
 {
	 MyGlWindow* pWindow = reinterpret_cast<MyGlWindow*>(pData);
	pWindow->UpdateScene();
 }
}

// MAIN
int main() {
	srand((int)time(NULL));


     Fl_Window win(WINDOW_WIDTH, 
					WINDOW_HEIGHT, 
					WINDOW_TITLE);

     MyGlWindow mygl(10, 10, win.w()-20, win.h()-20);
     win.end();
     win.resizable(mygl);
     win.show();
     return(Fl::run());
}