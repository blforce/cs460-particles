

#include <gl/glut.h>

#include "constants.h"

#include "Particle.h"


void renderScene(void);
void idleUpdate(void);
void changeSize(int w, int h);
double ElapsedMS(int startTime);

int lastRenderTime = 0;
int lastSpawnTime = 0;

bool showReflection = false;

double frameMS = 1.0 / TARGET_FPS;
double spawnMS = 1.0 / SPAWN_PER_SECOND;

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

	gluLookAt(0.0f, 1.0f, 10.0f,
			  0.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);


	InitializeParticles();

	

	glutMainLoop();


	return 1;
}

void idleUpdate(void)
{

	if( ElapsedMS(lastSpawnTime) >= spawnMS)
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
}

double ElapsedMS(int startTime)
{
	return (double)(clock() - startTime) / (double)CLOCKS_PER_SEC;
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
}



void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);


	// Draw the floor plane
	glBegin(GL_QUADS);
		glColor4f(0.25f, 0.25f, 0.25f, 0.5f);
		glVertex3f(-10.0f, 0.0f, 10.0f);
		glVertex3f(10.0f, 0.0f, 10.0f);
		glVertex3f(10.0f, 0.0f, -10.0f);
		glVertex3f(-10.0f, 0.0f, -10.0f);
	glEnd();

	RenderParticles();


	if(showReflection) {
		glPushMatrix();
			glScalef(1.0f, -1.0f, 1.0f);
			RenderParticles();
		glPopMatrix();
	}

	glutSwapBuffers();
}