#pragma once

#include "constants.h"
#include "CustomMath.h"
#include "Types.h"
#include <time.h>
#include <omp.h>
#include <math.h>


Particle Particles[MAX_PARTICLE_COUNT];
Vector gravity;
Vector camera;

int windowWidth, windowHeight;

void InitializeParticles()
{
	srand((int)time(NULL));

	gravity.x = 0.0f;
	gravity.y = -0.002f;
	gravity.z = 0.0f;

	int i;

#pragma omp parallel for
	for( i=0; i < MAX_PARTICLE_COUNT; i++ )
		Particles[i].Alive = false;
}





float GetPointSize(Particle part)
{
	return part.Size * windowWidth / Math::Distance(part.Position, camera);
}


void ActivateParticles()
{
	for(int i = 0; i < MAX_PARTICLE_COUNT; i++ )
	{
		// Revive dead particles
		if(!Particles[i].Alive)
		{
			// Initialize a starting position
			Particles[i].Position.x = Math::RandomValue(-0.1f, 0.1f);
			Particles[i].Position.y = 0.0f;
			Particles[i].Position.z = Math::RandomValue(-0.1f, 0.1f);


			// Give the particle a random velocity
			Particles[i].Velocity.x = Math::RandomValue(-0.05f, 0.05f);
			Particles[i].Velocity.y = Math::RandomValue(0.05f, 0.1f);
			Particles[i].Velocity.z = Math::RandomValue(-0.05f, 0.05f);

			// Create a random particle size
			Particles[i].Size = Math::RandomValue(PARTICLE_SIZE_MIN, PARTICLE_SIZE_MAX);


			// Setup the birth color
			Particles[i].BirthColor.r = Math::RandomValue(0.5f, 1.0f);
			Particles[i].BirthColor.g = Math::RandomValue(0.0f, 0.5f);
			Particles[i].BirthColor.b = Math::RandomValue(0.0f, 0.5f);
			Particles[i].BirthColor.a = 0.01f;

			// Setup the death
			Particles[i].DeathColor.r = Math::RandomValue(0.0f, 0.5f);
			Particles[i].DeathColor.g = Math::RandomValue(0.0f, 0.5f);
			Particles[i].DeathColor.b = Math::RandomValue(0.5f, 1.0f);
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
	for( int i = 0; i < MAX_PARTICLE_COUNT; i++ )
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
		if(Particles[i].Position.y <= PARTICLE_SIZE_MIN)
		{
			Particles[i].Velocity.x *= FLOOR_BOUNCE_FACTOR;
			Particles[i].Velocity.y = 0.0f - Particles[i].Velocity.y * FLOOR_BOUNCE_FACTOR;
			Particles[i].Velocity.z *= FLOOR_BOUNCE_FACTOR;

			Particles[i].Position.y = PARTICLE_SIZE_MIN + Particles[i].Velocity.y;
		}

		// Advance the age, and make sure it isn't supposed to be dead
		Particles[i].CurrentLife++;
		if(Particles[i].CurrentLife > Particles[i].MaximumLife)
			Particles[i].Alive = false;
	}
}





void RenderParticles(bool reflection)
{

#pragma omp parallel for
	for(int i = 0; i < MAX_PARTICLE_COUNT; i++ )
	{
		if(Particles[i].Alive)
		{
			// scale the particle as it gets closer and further from the camera
			glPointSize(GetPointSize(Particles[i]));

			glBegin(GL_POINTS);
				// set the particle color
				Color color = Particles[i].BirthColor;

				if( reflection)
					glColor4f(color.r, color.g, color.b, 0.9f );
				else
					glColor4f(color.r, color.g, color.b, color.a);

				glVertex3f(Particles[i].Position.x,
						   Particles[i].Position.y,
						   Particles[i].Position.z);

			glEnd();
		}

	}

	
}