#include "Shader.h"

using namespace lmi;

vec3 fresnel(vec3 F0, vec3 l, vec3 normal)
{
	return F0 + (vec3(1, 1, 1) - F0) * pow(1.0 - dot(l, normal), 5.0);
}

float GGX(vec3 halfway, vec3 normal, float roughness)
{
	float roughnessSq = roughness * roughness;
	return roughnessSq / (M_PI * pow(pow(dot(normal, halfway), 2.0) * (roughnessSq - 1.0) + 1.0, 2.0));
}

vec3 BRDF(vec3 lightPos, vec3 lightColor, vec3 pos, vec3 normal, vec3 albedo, float roughness, bool metal)
{
	const vec3 ambient(0.05);
	vec3 relLightPos = lightPos - pos;
	vec3 halfway = normalize(relLightPos - pos);
	vec3 l = normalize(pos * -1);
	vec3 F = metal ? lightColor * fresnel(albedo, l, normal) : vec3(0.04);
	
	if(dot(normal, normalize(relLightPos)) < 0.0)
	{
		return albedo * ambient;
	}

	vec3 specular = F * GGX(halfway, normal, roughness) / (4.0f * dot(l, halfway) * dot(l, halfway));
	vec3 diffuse = metal ? vec3(0) : (vec3(1, 1, 1) - F) / M_PI;	// Lambertian BRDF

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

uint32_t defaultFragmentShader(Fragment f)
{
	auto color = BRDF({20, 20, 0}, {1, 1, 1}, f.position, f.normal, {1.00, 0.71, 0.29}, 0.5, 1);

	return std::min(255u, uint32_t(color.r * 256)) << 16 | std::min(255u, uint32_t(color.g * 256)) << 8 |
		   std::min(255u, uint32_t(color.b * 256));
}
