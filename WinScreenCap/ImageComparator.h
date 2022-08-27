#pragma once
#include <vector>
#include <memory>
#include "ImageCompressor.h"
#ifdef WINSCREENCAP_EXPORTS
#define COMPRESSOR __declspec(dllexport)
#else
#define COMPRESSOR __declspec(dllimport)
#endif
namespace WinScreenCap {
	class ImageRegion {
	public:
		int X;
		int Y;
		unsigned long ImageSize;
		int BufferSize;
		std::unique_ptr<BYTE[]> Buffer;
		ImageRegion(int buffer_size);
	};
	class COMPRESSOR ImageComparator
	{
	private:
		int _width;
		int _byte_width;
		int _height;
		int _x_region_count;
		int _y_region_count;
		int _region_width;
		int _region_byte_width;
		int _region_height;
		std::unique_ptr<BYTE[]> _buffer1;
		std::unique_ptr<BYTE[]> _buffer2;
		std::unique_ptr<BYTE[]> _working_buffer;
		bool _buffer_swapped;
		int _buffer_size;
		std::vector<std::unique_ptr<ImageRegion>> _regions;
		ImageCompressor _compressor;

	public:
		ImageComparator(int width, int height, int x_region_count, int y_region_count);
		int GenerateDifferenceRegions();
		BYTE* CurrentBuffer();
		BYTE* OtherBuffer();
		void Swap() { _buffer_swapped = !_buffer_swapped; }
		ImageRegion* Region(int index);
	};
}

