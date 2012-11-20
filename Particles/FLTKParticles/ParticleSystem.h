#pragma once

#include "Types.h"
#include "constants.h"
#include "Particle.h"
#include "Environment.h"


class CParticleSystem
{
public:
	CParticleSystem(void);
	~CParticleSystem(void);

	void	Render(CEnvironment *env, Vector camera);

	void	Spawn(void);

	Vector	Position;
	Vector	Direction;
	float	Radius;
	double	BirthRate;

	double	MaximumParticleLife;

	double	ParticleSize;
	double	ParticleSizeVar;


private:
	CParticle	Particles[MAX_PARTICLE_COUNT];
	int			lastSpawnTime_;
};
