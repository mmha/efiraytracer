#pragma once

#include <lmi/lmi.h>

struct Fragment
{
	lmi::vec3 position;
	lmi::vec3 normal;
};

using FragmentShader = uint32_t (*)(Fragment);

extern FragmentShader fragmentShader;

lmi::vec3 BRDF(lmi::vec3 lightPos,
			   lmi::vec3 lightColor,
			   lmi::vec3 pos,
			   lmi::vec3 normal,
			   lmi::vec3 albedo,
			   float roughness,
			   bool metal);

uint32_t defaultFragmentShader(Fragment f);
