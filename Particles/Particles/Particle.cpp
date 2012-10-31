#include "Particle.h"


// Initialize all variables to default values
CParticle::CParticle(void)
{
	Alive = false;
	Mass = 1.0f;
	Size = 1.0f;

	LifeCurrent = 0.0f;
	LifeMaximum = 1.0f;

	Position[0] = 0.0f;
	Position[1] = 0.0f;
	Position[2] = 0.0f;

	Velocity[0] = 0.0f;
	Velocity[1] = 0.0f;
	Velocity[2] = 0.0f;

	ColorBirth[0] = 1.0f;
	ColorBirth[1] = 1.0f;
	ColorBirth[2] = 1.0f;
	ColorBirth[3] = 1.0f;

	ColorDeath[0] = 0.001f;
	ColorDeath[1] = 0.001f;
	ColorDeath[2] = 0.001f;
	ColorDeath[3] = 1.0f;
}


CParticle::~CParticle(void)
{
}


float CParticle::getLifePercentage(void)
{
	return LifeCurrent / LifeMaximum;
}

// Linearly interpolate the color
float* CParticle::getColor(void)
{
	float tempColor[4];
	float factor = getLifePercentage();

	// Interpolate each color value
	for(int i=0; i < 4; i++)
		tempColor[i] = ColorBirth[i] + (ColorDeath[i] - ColorBirth[i]) * factor;

	return tempColor;
}
