// ScreenForwarderConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include "WinScreenCap.h"

#include "Base64Converter.h"

int main()
{
    std::cout << "Hello World!\n";
   
    auto* cap = new WinScreenCap::WinScreenCap(0, 0, 1920, 1080);
    cap->Capture();
    cap->Compress();



    DWORD dwBytesWritten = 0;
    HANDLE hFile = NULL;



    // A file is created, this is where we will save the screen capture.
    hFile = CreateFile(L"capture1.jpeg",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    WriteFile(hFile, (LPSTR)cap->JpegBuffer(), cap->JpegSize(), &dwBytesWritten, NULL);


    //// Close the handle for the file that was created.
    CloseHandle(hFile);
    Utils::Base64Converter conv1(cap->JpegSize()*2);

    hFile = CreateFile(L"capture1_64.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    const char* ret = conv1.Convert(cap->JpegBuffer(), cap->JpegSize());
    WriteFile(hFile, (LPSTR)ret, strlen(ret), &dwBytesWritten, NULL);

    CloseHandle(hFile);
    return 0;
    

    char test[] = "test is a crazy test of a string";
    
    if (ret != nullptr) {
        std::cout << ret;
    }

    
    std::vector<WinScreenCap::WinScreenCap*> screen_regions;

    for (auto& di : WinScreenCap::WinScreenCap::GetDisplayInfo()) {
        screen_regions.push_back(new WinScreenCap::WinScreenCap(di->x, di->y, di->width, di->height));
        std::wcout << di->width << " " << di->height << std::endl;
    }


    for (auto wsc : screen_regions) {
        wsc->Capture();
        wsc->Compress();
        DWORD dwBytesWritten = 0;
        HANDLE hFile = NULL;



        // A file is created, this is where we will save the screen capture.
        hFile = CreateFile(L"capture1.jpeg",
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL);

        WriteFile(hFile, (LPSTR)wsc->JpegBuffer(), wsc->JpegSize(), &dwBytesWritten, NULL);


        //// Close the handle for the file that was created.
        CloseHandle(hFile);
    }
    
    
}
