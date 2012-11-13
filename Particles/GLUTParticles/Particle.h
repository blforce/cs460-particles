

#include "constants.h"
#include <time.h>
#include <omp.h>


typedef struct {
	float x, y, z;
} Vector;

typedef struct {
	float r, g, b, a;
} Color;

struct PARTICLE {

	Vector	Position;		// Current position
	Vector	Velocity;		// Velocity
	Color	BirthColor;		// Color when the particle is created
	Color	DeathColor;		// Color when the particle is destroyed

	float	Size;			// Particle size

	bool	Alive;			// Is this particle alive?
	int		CurrentLife;	// How many frames has this particle been alive?
	int		MaximumLife;	// How many frames should this particle live?

} Particles[PARTICLE_COUNT];

typedef struct PARTICLE Particle;

float bounceFactor = 0.45f;

Vector gravity;

void InitializeParticles()
{
	srand((int)time(NULL));

	gravity.x = 0.0f;
	gravity.y = -0.002f;
	gravity.z = 0.0f;

	int i;

#pragma omp parallel for
	for( i=0; i < PARTICLE_COUNT; i++ )
		Particles[i].Alive = false;
}

float RandomValue(float min, float max)
{
	float random = ((float) rand()) / (float) RAND_MAX;

	return (random * (max - min)) + min;
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
			Particles[i].Position.x = RandomValue(-0.1, 0.1);
			Particles[i].Position.y = 0.0f;
			Particles[i].Position.z = RandomValue(-0.1, 0.1);


			// Give the particle a random velocity
			Particles[i].Velocity.x = RandomValue(-0.05f, 0.05f);
			Particles[i].Velocity.y = RandomValue(0.05f, 0.1f);
			Particles[i].Velocity.z = RandomValue(-0.05f, 0.05f);

			// Create a random particle size
			Particles[i].Size = RandomValue(PARTICLE_SIZE_MIN, PARTICLE_SIZE_MAX);


			// Setup the birth color
			Particles[i].BirthColor.r = RandomValue(0.5f, 1.0f);
			Particles[i].BirthColor.g = RandomValue(0.0f, 0.5f);
			Particles[i].BirthColor.b = RandomValue(0.0f, 0.5f);
			Particles[i].BirthColor.a = 0.0f;

			// Setup the death
			Particles[i].DeathColor.r = RandomValue(0.0f, 0.5f);
			Particles[i].DeathColor.g = RandomValue(0.0f, 0.5f);
			Particles[i].DeathColor.b = RandomValue(0.5f, 1.0f);
			Particles[i].DeathColor.a = 0.75f;

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
		// Adjust the particle velocity
		Particles[i].Velocity.x += gravity.x;
		Particles[i].Velocity.y += gravity.y;
		Particles[i].Velocity.z += gravity.z;

		// Adjust the particle position
		Particles[i].Position.x += Particles[i].Velocity.x;
		Particles[i].Position.y += Particles[i].Velocity.y;
		Particles[i].Position.z += Particles[i].Velocity.z;

		// Make the particle "bounce"
		if(Particles[i].Position.y <= 0.0f)
		{
			Particles[i].Velocity.x *= bounceFactor;
			Particles[i].Velocity.y = 0.0f - Particles[i].Velocity.y * bounceFactor;
			Particles[i].Velocity.z *= bounceFactor;

			Particles[i].Position.y = 0.0f;
		}

		// Advance the age, and make sure it isn't supposed to be dead
		Particles[i].CurrentLife++;
		if(Particles[i].CurrentLife > Particles[i].MaximumLife)
			Particles[i].Alive = false;
	}
}


Color InterpolateColor(Particle part)
{
	float ratio = (float)part.CurrentLife / (float)part.MaximumLife;

	Color tempColor;

	tempColor.r = part.BirthColor.r + (part.DeathColor.r - part.BirthColor.r) * ratio;
	tempColor.g = part.BirthColor.g + (part.DeathColor.g - part.BirthColor.g) * ratio;
	tempColor.b = part.BirthColor.b + (part.DeathColor.b - part.BirthColor.b) * ratio;
	tempColor.a = part.BirthColor.a + (part.DeathColor.a - part.BirthColor.a) * ratio;

	return tempColor;
}

void RenderParticles()
{
	

	

	for(int i = 0; i < PARTICLE_COUNT; i++ )
	{
		if(Particles[i].Alive)
		{
			// set the particle color

			glPointSize(Particles[i].Size);
			glBegin(GL_POINTS);
				Color color = InterpolateColor(Particles[i]);
				glColor4f(color.r, color.g, color.b, color.a);

				glVertex3f(Particles[i].Position.x,
						   Particles[i].Position.y,
						   Particles[i].Position.z);

			glEnd();
		}

	}

	
}