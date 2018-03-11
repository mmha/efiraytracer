#include "Shader.h"

using namespace lmi;

vec3 fresnel(vec3 F0, vec3 l, vec3 normal) {
	return F0 + (vec3(1, 1, 1) - F0) * powf(1.0f - dot(l, normal), 5.0f);
}

float GGX(vec3 halfway, vec3 normal, float roughness) {
	float roughnessSq = roughness * roughness;
	return roughnessSq / (float(M_PI) * powf(powf(dot(normal, halfway), 2.0f) * (roughnessSq - 1.0f) + 1.0f, 2.0f));
}

vec3 BRDF(vec3 lightPos, vec3 lightColor, vec3 pos, vec3 normal, vec3 albedo, float roughness, bool metal) {
	const vec3 ambient(0.1f);
	vec3 relLightPos = lightPos - pos;
	vec3 halfway = normalize(relLightPos - pos);
	vec3 l = normalize(pos * -1);
	vec3 F = metal ? lightColor * fresnel(albedo, l, normal) : vec3(0.04f);

	if(dot(normal, normalize(relLightPos)) < 0.0f) {
		return albedo * ambient;
	}

	vec3 specular = F * GGX(halfway, normal, roughness) / (4.0f * dot(l, halfway) * dot(l, halfway));
	vec3 diffuse = metal ? vec3(0) : (vec3(1, 1, 1) - F) / float(M_PI);	// Lambertian BRDF

	return specular + albedo * diffuse + albedo * ambient;
}

/**
 * We are using a Microfacet model with the GGX distribution and the Fresnel Schlick approximation
 */
/*
vec3 directIllumination(vec3 pos, vec3 normal, vec3 albedo, float roughness, float metalness)
{
	vec3 finalColor = vec3(0);

	int i = 0;
	while(!isnan(light[i].type))
	{
		vec3 lightColor = vec3(1);//doSomeProcessing(light[i]);
		finalColor += BRDF(light[i].pos, lightColor, pos, normal, albedo, roughness, metalness > 0.5);
		++i;
	}

	return albedo; //should be finalColor, for now just display the albedo
}*/

FragmentShader fragmentShader = defaultFragmentShader;

uint32_t defaultFragmentShader(Fragment f) {
	auto color = BRDF({20, 20, 0}, {1, 1, 1}, f.position, f.normal, {1.00f, 0.71f, 0.29f}, 0.6f, true);
	auto hack = [](auto col) -> uint32_t {
		auto const res = uint32_t(col * 256);
		if(res >= 255) {
			return 0;
		}
		return res;
	};

	return hack(color.r) << 16 | hack(color.g) << 8 | hack(color.b);
}
