//#pragma once

#include "ParticleSystem.h"

#include <omp.h>
#include <time.h>


CParticleSystem::CParticleSystem(void)
{
	lastSpawnTime_ = clock();
	BirthRate = SPAWN_PER_SECOND;
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
	int spawnTarget = Math::ElapsedSeconds(lastSpawnTime_) * BirthRate;
	//int spawnCount = 0;
	//if( Math::ElapsedSeconds(lastSpawnTime_) > (1.0f / BirthRate) )
	//for( int s = 0; s < spawnCount; s++)
	//{

#pragma omp parallel for
		for(int i = 0; i < MAX_PARTICLE_COUNT; i++)
		{

			// TODO: Pass spawn paramaters
			if( !Particles[i].isAlive() )
			{
				Particles[i].Activate();

				lastSpawnTime_ = clock();

				//spawnCount++;
				spawnTarget--;
				if(spawnTarget <= 0 )
					return;

				/*if( spawnCount >= spawnTarget )
					return;*/
			}
		}

	//}
}