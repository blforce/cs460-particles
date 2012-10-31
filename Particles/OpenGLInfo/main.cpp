
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>

int main(int argc, char* argv[])
{
	fprintf(
			stdout,
			"INFO: OpenGL Version: %s\n",
			glGetString(GL_VERSION)
		);
}