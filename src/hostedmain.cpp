#include "Camera.h"
#include "Primitive.h"
#include "Shader.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <lmi/iostream_support.h>

int main()
{
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	window =
		SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Camera cam(1.03, {800, 600}, {0, 0, 0}, lmi::Quat(1, 0, 0, 0));

	auto scene = [](const lmi::vec3 &rayCurrentT, float scale) {
		return std::min({
			Primitive::sphere(lmi::vec3(0.0, 0.0, -50) - rayCurrentT, scale)
			// Primitive::sphere(lmi::vec3(-10.0, 0.0, 50) + rayCurrentT, 0.5)
		});
	};

	while(1)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				std::exit(0);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for(int i = 0; i < 800; ++i)
		{
			for(int j = 0; j < 600; ++j)
			{
				auto ray = cam.rayFromPixel({i, j});

				float precision = 1e-3;
				float t = ray.minT;

				while(t < ray.maxT)
				{
					const auto pos = ray.origin + ray.direction * t;
					const auto distance = scene(pos, 30);

					if(distance < precision)
					{
						const auto normal = lmi::normalize(pos - lmi::vec3(0.0, 0.0, -50));

						auto color = BRDF({20, 20, 0}, {0.7, 0.7, 0.7}, pos, normal, {1.00, 0.71, 0.29}, 0.4, 1) * 255;

						SDL_SetRenderDrawColor(renderer, std::min(color.r, 255.0f), std::min(color.g, 255.0f),
											   std::min(color.b, 255.0f), 255);

						SDL_RenderDrawPoint(renderer, i, 600 - j - 1);

						break;
					}

					t += distance;
				}
			}
		}
		SDL_RenderPresent(renderer);
	}
}