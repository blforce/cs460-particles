//#pragma once

#include "Environment.h"
#include "constants.h"

CEnvironment::CEnvironment()
{
	gravity.x = 0.0f;
	gravity.y = -9.8f;
	gravity.z = 0.0f;

	windowWidth = 800;
}

CEnvironment::CEnvironment(int  windowWidth_)
{
	gravity.x = 0.0f;
	gravity.y = -9.8f;
	gravity.z = 0.0f;

	windowWidth = windowWidth_;
}


CEnvironment::~CEnvironment(void)
{
}
