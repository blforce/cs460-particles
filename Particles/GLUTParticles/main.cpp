

#include <gl/glut.h>

#include "constants.h"
#include <stdio.h>

#include "Particle.h"


void renderScene(void);
void idleUpdate(void);
void changeSize(int w, int h);
double ElapsedMS(int startTime);
double Timespan(int startTime, int endTime);

int lastRenderTime = 0;
int lastSpawnTime = 0;

bool showReflection = true;

double frameMS = 1.0 / TARGET_FPS;
double spawnMS = 1.0 / SPAWN_PER_SECOND;

int frames = 0;
int fpsTime = 0;

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(renderScene);
	glutIdleFunc(idleUpdate);
	glutReshapeFunc(changeSize);

	camera.x = 0.0f;
	camera.y = 1.0f;
	camera.z = 10.0f;

	gluLookAt(camera.x, camera.y, camera.z,
			  0.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);


	InitializeParticles();

	

	glutMainLoop();


	return 1;
}

void idleUpdate(void)
{
	int currentTime = clock();
	while( Timespan(lastSpawnTime, currentTime) >= spawnMS)
	{
		lastSpawnTime += (spawnMS * CLOCKS_PER_SEC);
		ActivateParticles();
	}

	if( ElapsedMS(lastRenderTime) >= frameMS )
	{
		lastRenderTime = clock();

		// Update all particle positions
		AdjustParticles();

		// Show the scene to the user
		renderScene();
	}

	// Update the FPS
	if( ElapsedMS(fpsTime) >= 1.0 )
	{
		char title[256];
		sprintf(title, "CS460 Particles - %d fps\0", frames);
		glutSetWindowTitle(title);

		fpsTime = clock();
		frames = 0;
	}
}

double Timespan(int startTime, int endTime)
{
	return (double)(endTime - startTime) / (double)CLOCKS_PER_SEC;
}

double ElapsedMS(int startTime)
{
	return Timespan(startTime, clock());
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

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

	windowWidth = w;
	windowHeight = h;
}



void renderScene(void)
{
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

	RenderParticles(false);


	if(showReflection) {
		glPushMatrix();
			glScalef(1.0f, -1.0f, 1.0f);
			RenderParticles(true);
		glPopMatrix();
	}

	glutSwapBuffers();

	frames++;
}