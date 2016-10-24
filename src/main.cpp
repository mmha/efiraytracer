#include "Camera.h"
#include "Framebuffer.h"
#include "Primitive.h"
#include "Shader.h"
#include <efi.h>
#include <eficon.h>
#include <efilib.h>
#include <efiprot.h>

void exitBootServices(EFI_BOOT_SERVICES *bootServices, EFI_HANDLE imageHandle);

extern "C"[[noreturn]] EFI_STATUS EFIAPI efi_main(IN EFI_HANDLE imageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	// Edit this variable to set the resolution
	constexpr lmi::vec2i resolution{800, 600};

	Framebuffer<resolution[0], resolution[1], uint32_t> f(SystemTable->BootServices, SystemTable->ConOut);

	exitBootServices(SystemTable->BootServices, imageHandle);

	f.clear();
	f.swapToScreen();

	// 1.03 rad = ~60 deg
	Camera cam(1.03f, resolution, {0, 0, 0}, lmi::Quat(1, 0, 0, 0));

	auto t = [] {
		uint64_t a, d;
		__asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
		return (d << 32) | a;
	};

	auto scene = [](const lmi::vec3 &rayCurrentT, float scale) {
		return std::min({
			Primitive::sphere(lmi::vec3(0.0, 0.0, 50) + rayCurrentT, scale)
			// Primitive::sphere(lmi::vec3(-10.0, 0.0, -50) + rayCurrentT, 0.5)
		});
	};

	while(1)
	{
		// const auto scale = (t() % 65536) / (65536 / 10.0);
		f.clear();

		for(size_t i = 0; i < f.size().x; ++i)
			for(size_t j = 0; j < f.size().y; ++j)
			{
				auto ray = cam.rayFromPixel({(int)i, (int)j});

				float precision = 0.1;
				float t = ray.minT;

				while(t < ray.maxT)
				{
					const auto pos = ray.origin + ray.direction * t;
					const auto distance = scene(pos, 30);

					if(distance < precision)
					{
						const auto normal = normalize(pos - lmi::vec3(0.0, 0.0, -50));

						f(i, j) = defaultFragmentShader({pos, normal});
						break;
					}

					t += distance;
				}
			}

		f.swapToScreen();
	}
}

void exitBootServices(EFI_BOOT_SERVICES *bootServices, EFI_HANDLE imageHandle)
{
	const uint64_t memoryMapSize = 1024 * 1024 * sizeof(EFI_MEMORY_DESCRIPTOR);
	uint8_t memoryMap[memoryMapSize];
	auto tmpMmapSize = memoryMapSize;
	uint64_t mapKey, descriptorSize;
	uint32_t descriptorVersion;

	bootServices->GetMemoryMap(&tmpMmapSize, (EFI_MEMORY_DESCRIPTOR *)memoryMap, &mapKey, &descriptorSize,
							   &descriptorVersion);
	bootServices->ExitBootServices(imageHandle, mapKey);
}
