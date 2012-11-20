#pragma once

#include "Types.h"

class CEnvironment
{
public:
	CEnvironment();
	CEnvironment(int  windowWidth_);
	~CEnvironment(void);

	int		windowWidth;
	Vector	gravity;
};
