//#pragma once

#include "ParticleSystem.h"

#include <omp.h>
#include <time.h>


CParticleSystem::CParticleSystem(void)
{
	lastSpawnTime_ = clock();
	BirthRate = SPAWN_PER_SECOND;
	MaximumParticleLife = MAX_PARTICLE_LIFE;
	ParticleSize = PARTICLE_SIZE;
	ParticleSizeVar = PARTICLE_SIZE_VAR;
}

CParticleSystem::~CParticleSystem(void)
{
}


// Render all of the particles
void CParticleSystem::Render(CEnvironment *env, Vector camera)
{
	int time = clock();
#pragma omp parallel for
	for( int i = 0; i < MAX_PARTICLE_COUNT; i++)
	{
		if( Particles[i].isAlive() )
			Particles[i].Render(env, camera, time);
	}
}



void CParticleSystem::Spawn(void)
{
	int spawnTarget = (int)(Math::ElapsedSeconds(lastSpawnTime_) * BirthRate);


#pragma omp parallel for
	for(int i = 0; i < MAX_PARTICLE_COUNT; i++)
	{
		if(spawnTarget <= 0 )
				return;

		// TODO: Pass spawn paramaters
		if( !Particles[i].isAlive() )
		{
			Particles[i].Activate(this);

			lastSpawnTime_ = clock();

			spawnTarget--;
		}
	}
}