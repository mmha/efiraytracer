#pragma once

#include "EFIWrapper.h"
#include <algorithm>
#include <efi.h>
#include <efilib.h>
#include <lmi/lmi.h>

template <size_t width, size_t height, typename T>
class Framebuffer {
	public:
	Framebuffer(EFI_BOOT_SERVICES *bootServices, SIMPLE_TEXT_OUTPUT_INTERFACE *conOut) {
		using std::get;
		EFI_GUID graphicsOutProtocolGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
		uint64_t handleCount = 0;

		auto locate_handle_buffer = wrap<3, 1, 1>(bootServices->LocateHandleBuffer);
		auto handle_protocol = wrap<2, 0, 1>(bootServices->HandleProtocol);
		auto maybe_gop = locate_handle_buffer(ByProtocol, graphicsOutProtocolGUID, nullptr, handleCount)
							 .and_then([&](auto proto_impl) {
								 return handle_protocol(get<0>(proto_impl)[0], graphicsOutProtocolGUID);
							 })
							 .map([](auto opened_gop) {
								 return reinterpret_cast<EFI_GRAPHICS_OUTPUT_PROTOCOL *>(get<0>(opened_gop));
							 });
		if(!maybe_gop) {
			conOut->OutputString(conOut, u"Failed to create an Graphics Out Protocol Instance\r\n"_C16);
		}

		gop = maybe_gop.value();
		auto query_mode = wrap<2, 0, 2>(gop->QueryMode);

		bool succeeded = false;
		// clang-format off
        for(int i = 0;; ++i) {
            auto mode_info = query_mode(gop, i)
            .map([](auto r) -> decltype(auto) { return *std::get<1>(r); });
            if(!mode_info) break;
            if(mode_info->HorizontalResolution == width &&
               mode_info->VerticalResolution == height &&
               mode_info->PixelFormat ==
               PixelBlueGreenRedReserved8BitPerColor) {
                 gop->SetMode(gop, i);
                 break;
            }
		}
		// clang-format on
		if(!succeeded) {
			conOut->OutputString(conOut, u"Failed to set video mode\r\n"_C16);
		}
	}

	void swapToScreen() const {
		auto pixelPtr = reinterpret_cast<uint32_t *>(gop->Mode->FrameBufferBase);
		for(const auto &row : rows) {
			std::copy(row.begin(), row.end(), pixelPtr);
			pixelPtr += gop->Mode->Info->PixelsPerScanLine;
		}
	}

	void clear() {
		for(auto &row : rows) {
			std::fill(row.begin(), row.end(), T{});
		}
	}

	T operator()(size_t x, size_t y) const {
		return rows[height - y - 1][x];
	}

	T &operator()(size_t x, size_t y) {
		return rows[height - y - 1][x];
	}

	constexpr static lmi::vec2ui size() {
		return {width, height};
	}

	private:
	using Row = std::array<T, width>;
	std::array<Row, height> rows;
	char whyIsThisNeeded[width * height * sizeof(T)];

	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
};
