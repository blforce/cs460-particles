//#pragma once

#include "Environment.h"
#include "constants.h"



void CEnvironment::initVariables()
{
	

	gravity.x = 0.0f;
	gravity.y = -9.8f;
	gravity.z = 0.0f;
}

CEnvironment::CEnvironment()
{
	initVariables();

	windowWidth = 800;
}

CEnvironment::CEnvironment(int  windowWidth_)
{
	initVariables();

	windowWidth = windowWidth_;
}

