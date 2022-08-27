#include "pch.h"
#include "ImageCompressor.h"
#include "turbojpeg.h"
#include <algorithm>

namespace WinScreenCap {
    const int JPEG_QUALITY = 75;

    ImageCompressor::ImageCompressor(int initial_size) :
        _jpeg_buffer_size(initial_size)
    {
        _jpeg_buffer = tjAlloc(_jpeg_buffer_size);
    }

    ImageCompressor::~ImageCompressor() {
        tjFree(_jpeg_buffer);
    }

    void ImageCompressor::Compress(int width, int height, std::unique_ptr<BYTE[]>& raw_image_buffer, std::unique_ptr<BYTE[]>& compressed_image_buffer, unsigned long& compressed_image_size) {
        tjhandle _jpegCompressor = tjInitCompress();
        compressed_image_size = _jpeg_buffer_size;\
        tjCompress2(_jpegCompressor, raw_image_buffer.get(), width, 0, height, TJPF_BGRX,
            &_jpeg_buffer, &compressed_image_size, TJSAMP_444, JPEG_QUALITY,
            TJFLAG_FASTDCT);

        if (compressed_image_size > _jpeg_buffer_size) {
            _jpeg_buffer_size = compressed_image_size;
        }

        tjDestroy(_jpegCompressor);
        memcpy(compressed_image_buffer.get(), _jpeg_buffer, compressed_image_size);
    }
}