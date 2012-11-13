



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gl/glew.h>
#include <gl/freeglut.h>


#include "constants.h"


int CurrentWidth = WINDOW_WIDTH,
	CurrentHeight = WINDOW_HEIGHT,
	WindowHandle = 0;

GLuint vertexbuffer;
GLuint ShaderProgram;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);

void RenderParticles(void);

void SetupScene(void);


int main(int argc, char* argv[])
{
	Initialize(argc, argv);

	glutMainLoop();
	
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	InitWindow(argc, argv);
	
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	SetupScene();
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);
	
	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if(WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTranslatef(0.0f, -1.0f, -10.0f);

	RenderParticles();


	glutSwapBuffers();
	glutPostRedisplay();
}


void RenderParticles(void)
{
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

	glBegin(GL_POINTS);

		glVertex3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);

	glEnd();

}


void SetupScene()
{
	


}