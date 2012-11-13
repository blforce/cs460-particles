
#include "constants.h"

#pragma once
class CParticle
{
public:
	// Constructor/Destructor
	CParticle(void);
	~CParticle(void);

	// Public Variables
	
	// Some variables for the physics simulation
	float Position[3];
	float Velocity[3];
	float Mass, Size;

	// RGBA Color values
	float ColorBirth[4];
	float ColorDeath[4];

	bool Alive;

	float LifeCurrent, LifeMaximum;

	// Public Functions
	float getLifePercentage(void);
	void getColor(float*);
};

