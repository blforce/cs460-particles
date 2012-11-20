#pragma once

#include "Types.h"

class CEnvironment
{

private:
	void	initVariables();

public:
	CEnvironment();
	CEnvironment(int  windowWidth_);

	int		windowWidth;
	Vector	gravity;

	
};
