#include "pch.h"
#include "WinScreenCap.h"
#include "turbojpeg.h"
#include <iostream>
namespace WinScreenCap {

    const int JPEG_QUALITY = 75;
    const int COLOR_COMPONENTS = 3;

    std::vector<std::unique_ptr<DisplayInfo>> WinScreenCap::GetDisplayInfo() {
        std::vector<std::unique_ptr<DisplayInfo>> displays;
        DEVMODE dm;
        dm.dmSize = sizeof(DEVMODE);
        dm.dmDriverExtra = 0;
        DISPLAY_DEVICE dd;
        dd.cb = sizeof(DISPLAY_DEVICE);
        int i = 0;

        //Enumerate the Displays to get the actual Resolution and offset
        while (EnumDisplayDevices(NULL, i, &dd, 0))
        {
            if (EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
            {
                std::unique_ptr<DisplayInfo> di = std::make_unique<DisplayInfo>();
                std::wcout << dd.DeviceName << "\n";
                di->x = dm.dmPosition.x;
                di->y = dm.dmPosition.y;
                di->width = dm.dmPelsWidth;
                di->height = dm.dmPelsHeight;
                displays.push_back(std::move(di));
            }
            i++;
        }
        std::wcout << displays.size() << "\n";
        return displays;
    }

    ImageData::ImageData(const size_t data_size) :image(std::make_unique<BYTE[]>(data_size)), image_size(data_size) {}


    WinScreenCap::WinScreenCap(int left, int top, int width, int height) :
        _left(left),
        _top(top),
        _width(width),
        _height(height),
        _right(left + width),
        _bottom(top + height)
        //,_jpeg_image_size(0)
    {
        // Retrieve the handle to a display device context for the client 
      // area of the window. 
        _window_handle = GetDC(NULL);

        // Create a compatible DC, which is used in a BitBlt from the window DC.
        _memory_handle = CreateCompatibleDC(_window_handle);

        if (!_memory_handle)
        {
            return;
        }

        // Create a compatible bitmap from the Window DC.
        _screen_bitmap_handle = CreateCompatibleBitmap(_window_handle, _width, _height);

        if (!_screen_bitmap_handle)
        {
            return;
        }

        _bitmap_info.biSize = sizeof(BITMAPINFOHEADER);
        _bitmap_info.biWidth = _width;
        _bitmap_info.biHeight = -_height;
        _bitmap_info.biPlanes = 1;
        _bitmap_info.biBitCount = 32;
        _bitmap_info.biCompression = BI_RGB;
        _bitmap_info.biSizeImage = 0;
        _bitmap_info.biXPelsPerMeter = 0;
        _bitmap_info.biYPelsPerMeter = 0;
        _bitmap_info.biClrUsed = 0;
        _bitmap_info.biClrImportant = 0;

        //_bitmap_buffer_size = ((_width * _bitmap_info.biBitCount + 31) / 32) * 4 * _height;
        //_bitmap_buffer = new char[_bitmap_buffer_size]();

        ////initial buffer size.. turbojpeg will resize if necessary
        //_jpeg_buffer_size = (_width * _height) / 2;
        //_jpeg_buffer = tjAlloc(_jpeg_buffer_size);

        // Select the compatible bitmap into the compatible memory DC.
        SelectObject(_memory_handle, _screen_bitmap_handle);
    }
    WinScreenCap::~WinScreenCap() {
        
        DeleteObject(_memory_handle);
        ReleaseDC(NULL, _window_handle);
        DeleteObject(_screen_bitmap_handle);
        /*
        delete _bitmap_buffer;*/
        _memory_handle = NULL;
        _window_handle = NULL;
        _screen_bitmap_handle = NULL;
        /*_jpeg_buffer = nullptr;
        _bitmap_buffer = nullptr;*/
    }

    ImageData WinScreenCap::MakeBuffer() {
        return ImageData(((_width * _bitmap_info.biBitCount + 31) / 32) * 4 * _height);
    }

    void WinScreenCap::Capture(BYTE* image_buffer) {
        // Bit block transfer into our compatible memory DC.
        if (!BitBlt(_memory_handle,
            0, 0,
            _width, _height,
            _window_handle,
            _left, _top,
            SRCCOPY))
        {
            return;
        }

        //Capture the cursor and draw it over the image
        CURSORINFO cursor_info = { sizeof(cursor_info) };
        GetCursorInfo(&cursor_info);
        //Check if the cursor point is with in our screen
        if (cursor_info.flags == CURSOR_SHOWING &&
            PointOnScreen(cursor_info.ptScreenPos.x, cursor_info.ptScreenPos.y)) {
            ICONINFO info = { sizeof(info) };
                GetIconInfo(cursor_info.hCursor, &info);
                BITMAP bmpCursor = { 0 };
                GetObject(info.hbmColor, sizeof(bmpCursor), &bmpCursor);
                DrawIconEx(_memory_handle, cursor_info.ptScreenPos.x, cursor_info.ptScreenPos.y, cursor_info.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight,
                    0, NULL, DI_NORMAL);
        }


        // Get the BITMAP from the HBITMAP.
        GetObject(_screen_bitmap_handle, sizeof(BITMAP), &_bitmap);

        // Gets the "bits" from the bitmap, and copies them into a buffer 
        // that's pointed to by lpbitmap.
        GetDIBits(_window_handle, _screen_bitmap_handle, 0,
            _bitmap.bmHeight,
            image_buffer,
            (BITMAPINFO*)&_bitmap_info, DIB_RGB_COLORS);
    }

    bool WinScreenCap::PointOnScreen(int x, int y) const {
        return (x >= _left && x <= _right && y >= _top && y <= _bottom);
    }
}