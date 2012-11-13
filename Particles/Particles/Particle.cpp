#include "Particle.h"


// Initialize all variables to default values
CParticle::CParticle(void)
{
	Alive = false;
	Mass = 1.0f;
	Size = 1.0f;

	LifeCurrent = 0.0f;
	LifeMaximum = 1.0f;

	Position[X] = 0.0f;
	Position[Y] = 0.0f;
	Position[Z] = 0.0f;

	Velocity[X] = 0.0f;
	Velocity[Y] = 0.0f;
	Velocity[Z] = 0.0f;

	ColorBirth[R] = 1.0f;
	ColorBirth[G] = 1.0f;
	ColorBirth[B] = 1.0f;
	ColorBirth[A] = 1.0f;

	ColorDeath[R] = 0.001f;
	ColorDeath[G] = 0.001f;
	ColorDeath[B] = 0.001f;
	ColorDeath[A] = 1.0f;
}


CParticle::~CParticle(void)
{
}


float CParticle::getLifePercentage(void)
{
	return LifeCurrent / LifeMaximum;
}

// Linearly interpolate the color
void CParticle::getColor(float* color)
{
	float factor = getLifePercentage();

	// Interpolate each color channel
	for( int channel=R; channel <= A; channel++ )
		color[channel] = ColorBirth[channel] 
							+ (ColorDeath[channel] - ColorBirth[channel]) * factor;
}
