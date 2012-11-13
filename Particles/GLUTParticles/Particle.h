

#include "constants.h"
#include <time.h>
#include <omp.h>

struct PARTICLE {

	float x, y, z;		// Current position
	float vX, vY, vZ;	// Velocity
	float r, g, b;		// Color
	bool Alive;			// Is this particle alive?
	int CurrentLife;	// How long has this particle been alive?
	int MaximumLife;	// How long should this particle live?

} Particles[PARTICLE_COUNT];


void InitializeParticles()
{
	srand((int)time(NULL));

	int i;

#pragma omp parallel for
	for( i=0; i < PARTICLE_COUNT; i++ )
		Particles[i].Alive = false;
}

float RandomVelocity()
{
	return (((float)((rand() % 100) + 1)) / 1000.0f) - 0.0f;
}


void ActivateParticles()
{
	for(int i = 0; i < PARTICLE_COUNT; i++ )
	{
		// Revive dead particles
		if(!Particles[i].Alive)
		{
			// Initialize a starting position
			Particles[i].x = 0.0f;
			Particles[i].y = 0.0f;
			Particles[i].z = 0.0f;


			// Give the particle a random velocity
			Particles[i].vX = RandomVelocity();
			Particles[i].vY = RandomVelocity();
			Particles[i].vZ = RandomVelocity();

			// Make the particle alive
			Particles[i].Alive = true;
			Particles[i].CurrentLife = 0;
			Particles[i].MaximumLife = MAX_PARTICLE_LIFE;

			// Don't activate them all at once!
			return;
		}
	}
}


void AdjustParticles()
{
#pragma omp parallel for
	for( int i = 0; i < PARTICLE_COUNT; i++ )
	{
		// Adjust the particle position
		Particles[i].x += Particles[i].vX;
		Particles[i].y += Particles[i].vY;
		Particles[i].z += Particles[i].vZ;

		// Advance the age, and make sure it isn't supposed to be dead
		Particles[i].CurrentLife++;
		if(Particles[i].CurrentLife > Particles[i].MaximumLife)
			Particles[i].Alive = false;
	}
}

void RenderParticles()
{
	glBegin(GL_POINTS);

	for(int i = 0; i < PARTICLE_COUNT; i++ )
	{
		if(Particles[i].Alive)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
			glVertex3f(Particles[i].x,
					   Particles[i].y,
					   Particles[i].z);
		}

	}

	glEnd();
}