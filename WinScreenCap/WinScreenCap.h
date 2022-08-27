#pragma once
#include <vector>
#include <memory>
#include <windows.h>
#ifdef WINSCREENCAP_EXPORTS
#define WINSCREENCAP_EXPORTS __declspec(dllexport)
#else
#define WINSCREENCAP_EXPORTS __declspec(dllimport)
#endif
namespace WinScreenCap {
    class DisplayInfo {
    public:
        int width;
        int height;
        int x;
        int y;
    };
    class ImageData {
    public:
        std::unique_ptr<BYTE[]> image;
        const size_t image_size;
        ImageData(const size_t input_size);
    };
    class WINSCREENCAP_EXPORTS WinScreenCap
    {
    public:
        WinScreenCap(int x, int y, int width, int height);
        ~WinScreenCap();
        
        static std::vector<std::unique_ptr<DisplayInfo>> GetDisplayInfo();
        ImageData MakeBuffer();
        void Capture(BYTE* image_buffer);
        /*void Compress();
        const char* JpegBuffer() { return reinterpret_cast<const char*>(_jpeg_buffer); }
        size_t JpegSize() { return _jpeg_image_size; }
        const char* BitmapBuffer() { return _bitmap_buffer; }
        size_t BitmapSize() { return _bitmap_buffer_size; }*/
        int Top() const { return _top; }
        int Left() const { return _left; }
        int Right() const { return _right;  }
        int Bottom() const { return _bottom; }
        int Width() const { return _width; }
        int Height() const { return _height; }
        bool PointOnScreen(int x, int y) const;


    private:
        int _top;
        int _left;
        int _right;
        int _bottom;
        int _width;
        int _height;
        /*unsigned char* _bitmap_buffer;
        unsigned long _bitmap_buffer_size;
        unsigned char* _jpeg_buffer;
        unsigned long _jpeg_buffer_size;
        unsigned long _jpeg_image_size;*/
        HDC _window_handle;
        HDC _memory_handle;
        BITMAPINFOHEADER _bitmap_info;
        BITMAP _bitmap;
        HBITMAP _screen_bitmap_handle;
       
    };

}