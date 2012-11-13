#pragma once

#include <gl/glew.h>

class CShader
{
public:

private:
	// Some variables to keep track of the shaders
	GLuint VertexShaderID, FragmentShaderID, ShaderProgramID;



	CShader(void);
	~CShader(void);
};

