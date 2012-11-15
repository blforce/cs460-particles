//#pragma once

//#include <omp.h>
#include <gl/glut.h>

#include "CustomMath.h"
#include "constants.h"
#include <stdio.h>

#include "ParticleSystem.h"
#include "Environment.h"
#include "Particle.h"

//#include "Scene.h"

//CScene scene;
CParticleSystem particles;
CEnvironment env;
Vector camera;

void renderScene(void);
void idleUpdate(void);
void changeSize(int w, int h);
double ElapsedMS(int startTime);

//int lastRenderTime = 0;
//int lastSpawnTime = 0;

//bool showReflection = true;

//double frameMS = 1.0 / TARGET_FPS;
//double spawnMS = 1.0 / SPAWN_PER_SECOND;

int frames = 0;
int fpsTime = 0;

int rotationTimer = 0;
//float angle = 0.0f;

int main(int argc, char **argv)
{
	//scene = CScene(WINDOW_WIDTH);

	srand((int)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(renderScene);
	glutIdleFunc(idleUpdate);
	glutReshapeFunc(changeSize);

	camera.x = 0.0f;
	camera.y = 0.5f;
	camera.z = 7.0f;

	gluLookAt(camera.x, camera.y, camera.z,
			  0.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);


	env = CEnvironment(WINDOW_WIDTH);

	rotationTimer = clock();
	

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();


	return 1;
}

void idleUpdate(void)
{
	int currentTime = clock();
	
	particles.Spawn();
	renderScene();

	// Update the FPS
	if( Math::ElapsedSeconds(fpsTime) >= 1.0 )
	{
		char title[256];
		sprintf_s(title, "CS460 Particles - %d fps\0", frames);
		glutSetWindowTitle(title);

		fpsTime = clock();
		frames = 0;
	}
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
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

	
	env.windowWidth = w;
	env.windowWidth = h;
}



void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

	glPushMatrix();

	float angle = (Math::ElapsedSeconds(rotationTimer) / 20.0f) * 360.0f;

	if( angle >= 360.0f )
	{
		angle = 0;
		rotationTimer = clock();
	}

	//glRotatef(angle, 0.0f, 1.0f, 0.0f);

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

	glutSwapBuffers();

	frames++;
}


void CParticle::Render(CEnvironment *env, Vector camera, int time)
{
	Color	color			= getColor(time);
	Vector	currentPosition = getPosition(env, time);

	// scale the particle as it gets closer and further from the camera
	glPointSize(getSize(env, camera));

	//glPushMatrix();
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_POINTS);
		// set the particle color
		

		glVertex3f(currentPosition.x,
				   currentPosition.y,
				   currentPosition.z);

	/*glTranslatef(currentPosition.x,
					 currentPosition.y,
					 currentPosition.z);

	glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		
		glVertex3f(-Size, -Size, 0.0f);
		glVertex3f(Size, -Size, 0.0f);
		glVertex3f(Size, Size, 0.0f);
		glVertex3f(-Size, Size, 0.0f);*/

		

	glEnd();
	//glPopMatrix();
}