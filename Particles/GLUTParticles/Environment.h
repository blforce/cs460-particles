#ifndef __ENVIRONMENT_H__
#define	__ENVIRONMENT_H__

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

#endif