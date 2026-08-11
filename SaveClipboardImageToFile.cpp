//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.


#include <windows.h>
#include <iostream>
#include <fstream>

#include "SaveClipboardImageToFile.h"

namespace Ambiesoft {

    bool SaveClipboardImageToFile(const wchar_t* pFileToSave, std::wstring* error)
    {
        // 1. Check if the clipboard contains a DIB image format
        if (!IsClipboardFormatAvailable(CF_DIB))
        {
            if (error)
                *error = L"Error: No image found in the clipboard.";
            return false;
        }

        // 3. Fetch the data handle from the clipboard
        HANDLE hClipboard = GetClipboardData(CF_DIB);
        if (hClipboard == NULL) {
            if (error)
                *error = L"Error: Unable to retrieve clipboard data.";
            return false;
        }

        // 4. Lock the global handle to safely read the DIB raw memory block
        void* pDib = GlobalLock(hClipboard);
        if (!pDib) {
            if (error)
                *error = L"Error: Unable to lock memory handle.";
            return false;
        }

        // Extract the DIB information header size and parameters
        BITMAPINFOHEADER* bitmapHeader = reinterpret_cast<BITMAPINFOHEADER*>(pDib);
        size_t dibSize = GlobalSize(hClipboard);

        // 5. Construct the missing BITMAPFILEHEADER manually
        BITMAPFILEHEADER fileHeader;
        fileHeader.bfType = 0x4D42; // "BM" signatures
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;

        // Calculate accurate offset where the pixel bits actually begin
        DWORD paletteSize = 0;
        if (bitmapHeader->biClrUsed > 0) {
            paletteSize = bitmapHeader->biClrUsed * sizeof(RGBQUAD);
        }
        else if (bitmapHeader->biBitCount <= 8) {
            paletteSize = (1 << bitmapHeader->biBitCount) * sizeof(RGBQUAD);
        }

        fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + bitmapHeader->biSize + paletteSize;
        fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + dibSize;

        // 6. Write out the constructed file structures to disk
        std::ofstream outFile(pFileToSave, std::ios::binary);
        if (!outFile) {
            if (error)
                *error = L"Error: Could not open output file for writing.";
            GlobalUnlock(hClipboard);
            return false;
        }

        // Write file header followed by the raw DIB payload
        outFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
        outFile.write(reinterpret_cast<const char*>(pDib), dibSize);

        // 7. Cleanup resource locks and references
        GlobalUnlock(hClipboard);

        return true;
    }
}