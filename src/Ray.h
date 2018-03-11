#pragma once

#include <lmi/lmi.h>

struct Ray {
	lmi::vec3 origin;
	float minT;
	lmi::vec3 direction;
	float maxT;
};
