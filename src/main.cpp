#include "Camera.h"
#include "EFIWrapper.h"
#include "Framebuffer.h"
#include "Primitive.h"
#include "Shader.h"
#include <efi.h>
#include <eficon.h>
#include <efilib.h>
#include <efiprot.h>

void exit_boot_services(EFI_BOOT_SERVICES *bootServices, EFI_HANDLE imageHandle);

extern "C"[[noreturn]][[gnu::ms_abi]] EFI_STATUS efi_main(IN EFI_HANDLE imageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
	// Edit this variable to set the resolution
	constexpr lmi::vec2i resolution{800, 600};

	Framebuffer<resolution[0], resolution[1], uint32_t> f(SystemTable->BootServices, SystemTable->ConOut);

	exit_boot_services(SystemTable->BootServices, imageHandle);

	f.clear();
	f.swapToScreen();

	// 1.03 rad = ~60 deg
	Camera cam(1.03f, resolution, {0, 0, 0}, lmi::Quat(1, 0, 0, 0));

	[[maybe_unused]] auto time = [] {
		uint64_t a, d;
		__asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
		return (d << 32) | a;
	};

	auto scene = [](const lmi::vec3 &rayCurrentT) {
		return Primitive::pUnion(Primitive::sphere(lmi::vec3(10.0, 0.0, 50) + rayCurrentT, 20),
								 Primitive::sphere(lmi::vec3(-10.0, 5.0, 50) + rayCurrentT, 20));
	};

	while(true) {
		// const auto scale = (t() % 65536) / (65536 / 10.0);
		f.clear();

		for(size_t i = 0; i < f.size().x; ++i)
			for(size_t j = 0; j < f.size().y; ++j) {
				auto ray = cam.rayFromPixel({(int)i, (int)j});

				float precision = 0.1f;
				float t = ray.minT;

				while(t < ray.maxT) {
					const auto pos = ray.origin + ray.direction * t;
					const auto distance = scene(pos);

					if(distance < precision) {
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

void exit_boot_services(EFI_BOOT_SERVICES *bootServices, EFI_HANDLE imageHandle) {
	std::array<EFI_MEMORY_DESCRIPTOR, 1024 * 1024> memory_map;
	auto mmap_size = memory_map.size();
	auto const get_mem_map = wrap<0, 2>(bootServices->GetMemoryMap);

	auto result = get_mem_map(mmap_size, memory_map.data());
	if(result) {
		auto const [mapKey, descriptorSize, descriptorVersion] = result.value();
		auto const exit_boot_services = wrap<2, 0>(bootServices->ExitBootServices);
		exit_boot_services(&imageHandle, mapKey);
	}
}
