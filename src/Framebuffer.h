#pragma once

#include <algorithm>
#include <efi.h>
#include <efilib.h>
#include <lmi/lmi.h>

template <size_t width, size_t height, typename T>
class Framebuffer
{
	public:
	Framebuffer(EFI_BOOT_SERVICES *bootServices, SIMPLE_TEXT_OUTPUT_INTERFACE *conOut)
	{
		EFI_GUID graphicsOutProtocolGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
		uint64_t handleCount = 0;
		EFI_HANDLE *handles;

		bool status =
			bootServices->LocateHandleBuffer(ByProtocol, &graphicsOutProtocolGUID, nullptr, &handleCount, &handles);
		if(!status)
		{
			conOut->OutputString(conOut, (CHAR16 *)u"Failed to create an Graphics Out Protocol Instance\r\n");
		}

		status = bootServices->HandleProtocol(handles[0], &graphicsOutProtocolGUID, (void **)&gop);
		if(!status)
		{
			conOut->OutputString(conOut, (CHAR16 *)u"Failed to create an Graphics Out Protocol Instance\r\n");
		}

		for(uint64_t i = 0; status; ++i)
		{
			uint64_t modeInfoSize;
			EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInfo;
			status = gop->QueryMode(gop, i, &modeInfoSize, &modeInfo);
			if(modeInfo->HorizontalResolution == width && modeInfo->VerticalResolution == height &&
			   modeInfo->PixelFormat == PixelBlueGreenRedReserved8BitPerColor)
			{
				gop->SetMode(gop, i);
				break;
			}
		}
		if(!status)
		{
			conOut->OutputString(conOut, (CHAR16 *)u"Failed to set video mode");
		}
	}

	void swapToScreen() const
	{
		auto pixelPtr = reinterpret_cast<uint32_t *>(gop->Mode->FrameBufferBase);
		for(const auto &row : rows)
		{
			std::copy(row.begin(), row.end(), pixelPtr);
			pixelPtr += gop->Mode->Info->PixelsPerScanLine;
		}
	}

	void clear()
	{
		for(auto &row : rows)
		{
			std::fill(row.begin(), row.end(), T{});
		}
	}

	T operator()(size_t x, size_t y) const
	{
		return rows[height - y - 1][x];
	}

	T &operator()(size_t x, size_t y)
	{
		return rows[height - y - 1][x];
	}

	constexpr static lmi::vec2ui size()
	{
		return {width, height};
	}

	private:
	using Row = std::array<T, width>;
	std::array<Row, height> rows;
	char whyIsThisNeeded[width * height * sizeof(T)];

	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
};
