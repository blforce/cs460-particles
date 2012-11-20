#pragma once


#include "CustomMath.h"
#include "Environment.h"


class CParticle
{
public:
	CParticle(void);
	~CParticle(void);


	void	Activate(void *system);
	void	Render(CEnvironment *env, Vector camera, int time);

	bool	isAlive(void);

	

private:
	bool	Alive;

	Color	BirthColor;
	Color	DeathColor;

	Vector	Position;
	Vector	Velocity;
	float	Size;

	int		BirthTime;
	int		LifeTime;

	Color	getColor(int time);
	Vector	getPosition(CEnvironment *env, int time);
	float	getSize(CEnvironment *env, Vector camera);

	float	dT(int time)	{ return Math::dT(BirthTime, time); }
};
