
#version 400	core

layout(location = 0) in float life;
layout(location = 1) in float death;
layout(location = 2) in vec3 Position;
layout(location = 3) in vec3 Color;

in Particle
{
	vec3 Position;
	vec4 Color;
};

uniform Transform
{
	mat4	ModelViewMatrix;
	mat4	ModelViewProjectionMatrix;
};

void main()
{
	// Transform the vertex
	gl_Position = Position; 
}