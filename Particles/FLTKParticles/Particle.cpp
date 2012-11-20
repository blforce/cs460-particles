
#include <FL/gl.h>
#include "Particle.h"

#pragma comment(lib, "fltkgld.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//#include <gl/glut.h>
#include <time.h>



CParticle::CParticle(void)
{
	Alive = false;
}


bool CParticle::isAlive(void)
{
	if(!Alive)
		return false;


	if((clock() - BirthTime) > LifeTime)
	{
		Alive = false;
		return false;
	}
	else
		return true;
}


void CParticle::Activate(void)
{
	// Initialize a starting position
	Position.x = Math::RandomValue(-0.1f, 0.1f);
	Position.y = Math::RandomValue(0.01f, 0.1f);
	Position.z = Math::RandomValue(-0.1f, 0.1f);


	// Give the particle a random velocity
	Velocity.x = Math::RandomValue(-1.0f, 1.0f);
	Velocity.y = Math::RandomValue(1.0f, 8.0f);
	Velocity.z = Math::RandomValue(-1.0f, 1.0f);

	// Create a random particle size
	Size = Math::RandomValue(PARTICLE_SIZE_MIN, PARTICLE_SIZE_MAX);
	//Size = PARTICLE_SIZE_MAX;

	// Setup the birth color
	BirthColor.r = Math::RandomValue(0.0f, 0.0f);
	BirthColor.g = Math::RandomValue(0.0f, 0.8f);
	BirthColor.b = Math::RandomValue(0.5f, 1.0f);
	BirthColor.a = 0.01f;

	// Setup the death
	DeathColor.r = Math::RandomValue(0.0f, 0.5f);
	DeathColor.g = Math::RandomValue(0.0f, 0.5f);
	DeathColor.b = Math::RandomValue(0.5f, 1.0f);
	DeathColor.a = 0.75f;

	// Make the particle alive
	Alive = true;
	BirthTime = clock();
	LifeTime = (MAX_PARTICLE_LIFE * CLOCKS_PER_SEC);
}


Color CParticle::getColor(int time)
{
	float ratio = (float)(time - BirthTime) / (float)LifeTime;

	Color tempColor;

	tempColor.r = BirthColor.r + (DeathColor.r - BirthColor.r) * ratio;
	tempColor.g = BirthColor.g + (DeathColor.g - BirthColor.g) * ratio;
	tempColor.b = BirthColor.b + (DeathColor.b - BirthColor.b) * ratio;
	tempColor.a = BirthColor.a + (DeathColor.a - BirthColor.a) * ratio;

	return tempColor;
}

float CParticle::getSize(CEnvironment *env, Vector camera)
{
	return Size * env->windowWidth / Math::Distance(Position, camera);
}

Vector CParticle::getPosition(CEnvironment *env, int time)
{
	Vector currentPosition;
	float deltaT = dT(time);


	currentPosition.x = Position.x + (Velocity.x * deltaT) + (0.5f * env->gravity.x * pow(deltaT, 2));
	currentPosition.y = Position.y + (Velocity.y * deltaT) + (0.5f * env->gravity.y * pow(deltaT, 2));
	currentPosition.z = Position.z + (Velocity.z * deltaT) + (0.5f * env->gravity.z * pow(deltaT, 2));


	// Do a collision test with the ground plane
	if( currentPosition.y <= 0.0f )
	{
		currentPosition.y = 0.0001f;

		Velocity.y = 0.0f;
		//Velocity.x -= 0.9f;
		//Velocity.z *= 0.9f;
	}


	return currentPosition;
}


void CParticle::Render(CEnvironment *env, Vector camera, int time)
{
	Color	color			= getColor(time);
	Vector	currentPosition = getPosition(env, time);

	// scale the particle as it gets closer and further from the camera
	glPointSize(getSize(env, camera));

	//glPushMatrix();
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_POINTS);
		// set the particle color
		

		glVertex3f(currentPosition.x,
				   currentPosition.y,
				   currentPosition.z);

	/*glTranslatef(currentPosition.x,
					 currentPosition.y,
					 currentPosition.z);

	glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		
		glVertex3f(-Size, -Size, 0.0f);
		glVertex3f(Size, -Size, 0.0f);
		glVertex3f(Size, Size, 0.0f);
		glVertex3f(-Size, Size, 0.0f);*/

		

	glEnd();
	//glPopMatrix();
}



CParticle::~CParticle(void)
{
}
