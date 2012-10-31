
#include <stdio.h>
#include <GL/glew.h>

void main(void)
{
	fprintf(
			stdout,
			"INFO: OpenGL Version: %s\n",
			glGetString(GL_VERSION)
		);
}