// ScreenForwarderConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <Windows.h>
#include <vector>
#include "WinScreenCap.h"
#include "ImageComparator.h"
#include "ImageCompressor.h"
#include "Base64Converter.h"

int main()
{
    std::cout << "Hello World!\n";
   
    

    auto* cap = new WinScreenCap::WinScreenCap(0, 0, 1920, 1080);
    WinScreenCap::ImageCompressor compressor((1920 * 1080)/2);
    WinScreenCap::ImageData buffer = cap->MakeBuffer();
    WinScreenCap::ImageComparator compraror(1920, 1080, 10, 10);
    std::unique_ptr<BYTE[]> compressed = std::make_unique<BYTE[]>((1920 * 1080)/2);
    unsigned long image_size = 0;
    int region_width_count = 10;
    int region_height_count = 10;
    int crop_width = 1920 / region_width_count;
    int crop_height = 1080 / region_height_count;
    int crop_byte_width = crop_width * 4;
    int screen_byte_width = 1920 * 4;

    std::cout << GetTickCount64() << std::endl;

    

    //std::vector<std::unique_ptr<BYTE[]>> regions;
    //std::unique_ptr<BYTE[]> working = std::make_unique<BYTE[]>(crop_height * crop_byte_width);
    //for (int r = 0; r < region_width_count * region_height_count; r++) {
    //    regions.push_back(std::make_unique<BYTE[]>(crop_height * crop_byte_width));
    //}

    //Takes about 50ms
    cap->Capture(compraror.CurrentBuffer());
    compraror.Swap();
    //Sleep(1000);
    std::cout << GetTickCount64() << std::endl;
    cap->Capture(compraror.CurrentBuffer());

    std::cout << GetTickCount64() << std::endl;
    int differences = compraror.GenerateDifferenceRegions();

    std::cout << differences << std::endl;

    std::cout << GetTickCount64() << std::endl;
  /*  for (int ry = 0; ry < region_height_count; ry++) {
        int y_offset = ry * crop_height;
        for (int rx = 0; rx < region_width_count; rx++) {
            int x_offset = rx * crop_byte_width;
            for (int y = 0; y < crop_height; y++) {
                for (int x = 0; x < crop_byte_width; x++) {
                    working[x + y * crop_byte_width] = buffer.image[(x+x_offset) + (y+y_offset) * screen_byte_width];
                }
            }
            compressor.Compress(crop_width, crop_height, working, regions[rx+ry*region_width_count], image_size);
        }
    }*/


    DWORD dwBytesWritten = 0;
    HANDLE hFile = NULL;

    for (int r = 0; r < differences; r++) {
        
        WinScreenCap::ImageRegion* region = compraror.Region(r);
        std::wstringstream ss;
        ss << L"capture" << region->Y << "_" << region->X << L".jpg";
        hFile = CreateFile(ss.str().c_str(),
                GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL, NULL);

       
        WriteFile(hFile, (LPSTR)region->Buffer.get(), region->ImageSize, &dwBytesWritten, NULL);


        //// Close the handle for the file that was created.
        CloseHandle(hFile);
    }

    // A file is created, this is where we will save the screen capture.
   
   
    //Utils::Base64Converter conv1(image_size*2);

    //hFile = CreateFile(L"capture1_64.txt",
    //    GENERIC_WRITE,
    //    0,
    //    NULL,
    //    CREATE_ALWAYS,
    //    FILE_ATTRIBUTE_NORMAL, NULL);

    //const char* ret = conv1.Convert(reinterpret_cast<char*>(compressed.get()), image_size);
    //WriteFile(hFile, (LPSTR)ret, strlen(ret), &dwBytesWritten, NULL);

    //CloseHandle(hFile);
    //return 0;
    //

    //char test[] = "test is a crazy test of a string";
    //
    //if (ret != nullptr) {
    //    std::cout << ret;
    //}

    //
    //std::vector<WinScreenCap::WinScreenCap*> screen_regions;

    //for (auto& di : WinScreenCap::WinScreenCap::GetDisplayInfo()) {
    //    screen_regions.push_back(new WinScreenCap::WinScreenCap(di->x, di->y, di->width, di->height));
    //    std::wcout << di->width << " " << di->height << std::endl;
    //}


    //for (auto wsc : screen_regions) {
    //    wsc->Capture();
    //    wsc->Compress();
    //    DWORD dwBytesWritten = 0;
    //    HANDLE hFile = NULL;



    //    // A file is created, this is where we will save the screen capture.
    //    hFile = CreateFile(L"capture1.jpeg",
    //        GENERIC_WRITE,
    //        0,
    //        NULL,
    //        CREATE_ALWAYS,
    //        FILE_ATTRIBUTE_NORMAL, NULL);

    //    WriteFile(hFile, (LPSTR)wsc->JpegBuffer(), wsc->JpegSize(), &dwBytesWritten, NULL);


    //    //// Close the handle for the file that was created.
    //    CloseHandle(hFile);
    //}
    
}
