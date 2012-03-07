#pragma once
#include <gmath.h>

using gmath::vector;

class light
{
public:
	virtual ~light(void){}

	bool IsLight(){return true;}

protected:
	light(){}
};
