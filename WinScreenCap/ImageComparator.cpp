#include "pch.h"
#include "ImageComparator.h"
namespace WinScreenCap {
	ImageRegion::ImageRegion(int buffer_size) {
		BufferSize = buffer_size;
		Buffer = std::make_unique<BYTE[]>(buffer_size);
	}

	ImageComparator::ImageComparator(int width, int height, int x_region_count, int y_region_count) :
		_width(width),
		_byte_width(width * 4),
		_height(height),
		_buffer_size(width* height * 4), //BGRX
		_x_region_count(x_region_count),
		_y_region_count(y_region_count),
		_region_width(_width / x_region_count),
		_region_byte_width(_region_width * 4),
		_region_height(_height / y_region_count),
		_buffer_swapped(false),
		_compressor(_region_width* _region_byte_width)
	{
		_buffer1 = std::make_unique<BYTE[]>(_buffer_size);
		_buffer2 = std::make_unique<BYTE[]>(_buffer_size);
		_working_buffer = std::make_unique<BYTE[]>(_region_width * _region_byte_width);
		_compressed = std::make_unique<BYTE[]>(_buffer_size);
		for (int r = 0; r < x_region_count * y_region_count; r++) {
			_regions.push_back(std::make_unique<ImageRegion>(_region_width * _region_byte_width));
		}
	}
	int ImageComparator::GenerateDifferenceRegions()
	{
		const BYTE* buffer1 = CurrentBuffer();
		const BYTE* buffer2 = OtherBuffer();
		int differences = 0;
		unsigned long image_size = 0;
		int y_offset = 0;
		int x_offset = 0;
		bool region_difference = false;
		for (int ry = 0; ry < _y_region_count; ry++) {
			y_offset = ry * _region_height;

			for (int rx = 0; rx < _x_region_count; rx++) {
				x_offset = rx * _region_byte_width;

				region_difference = false;
				//copy all bytes
				for (int y = 0; y < _region_height; y++) {
					for (int x = 0; x < _region_byte_width; x++) {
						int yidx = (y + y_offset);
						int xidx = (x + x_offset);
						int idx = xidx + yidx * _byte_width;
						BYTE b = buffer1[idx];
						region_difference = region_difference || b != buffer2[idx];
						_working_buffer[x + y * _region_byte_width] = b;
					}
				}
				if (region_difference) {
					_compressor.Compress(_region_width, _region_height, _working_buffer, _regions[differences]->Buffer, image_size);
					_regions[differences]->ImageSize = image_size;
					_regions[differences]->X = rx*_region_width;
					_regions[differences]->Y = y_offset;
					_regions[differences]->Width = _region_width;
					_regions[differences]->Height = _region_height;

					differences++;
				}
				
			}
		}

		return differences;
	}

	ImageRegion* ImageComparator::Region(int index) {
		return _regions[index].get();
	}

	BYTE* ImageComparator::CurrentBuffer() {
		return _buffer_swapped ? _buffer2.get() : _buffer1.get();
	}

	BYTE* ImageComparator::OtherBuffer() {
		return _buffer_swapped ? _buffer1.get() : _buffer2.get();
	}

	BYTE* ImageComparator::CompressedBuffer() {
		_compressor.Compress(_width, _height, CurrentBuffer(), _compressed.get(), _compressed_buffer_size);
		return _compressed.get();
	}

}