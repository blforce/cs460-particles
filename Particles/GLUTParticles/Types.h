

#ifndef __PARTICLE_SYSTEM_TYPES__
#define __PARTICLE_SYSTEM_TYPES__

typedef struct {
	float x, y, z;
} Vector;

typedef struct {
	float r, g, b, a;
} Color;

typedef struct {

	Vector	Position;		// Current position
	Vector	Velocity;		// Velocity
	Color	BirthColor;		// Color when the particle is created
	Color	DeathColor;		// Color when the particle is destroyed

	float	Size;			// Particle size

	bool	Alive;			// Is this particle alive?
	int		CurrentLife;	// How many frames has this particle been alive?
	int		MaximumLife;	// How many frames should this particle live?

} Particle;

#endif