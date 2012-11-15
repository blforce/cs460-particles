#ifndef	__PARTICLESYSTEM_H__
#define	__PARTICLESYSTEM_H__

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
	float	BirthRate;


private:
	CParticle	Particles[MAX_PARTICLE_COUNT];
	int			lastSpawnTime_;
};

#endif