#pragma once
#ifdef WINSCREENCAP_EXPORTS
#define WINSCREENCAP_EXPORTS __declspec(dllexport)
#else
#define WINSCREENCAP_EXPORTS __declspec(dllimport)
#endif
#include <memory>

namespace WinScreenCap {
	class WINSCREENCAP_EXPORTS ImageCompressor
	{
	private:
		BYTE* _jpeg_buffer;
		size_t _jpeg_buffer_size;
	public:
		ImageCompressor(int initial_size);
		~ImageCompressor();
		void Compress(int width, int height, std::unique_ptr<BYTE[]>& raw_image_buffer, std::unique_ptr<BYTE[]>& compressed_image_buffer, unsigned long& compress_image_size);
	};
}

