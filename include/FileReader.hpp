/**
 * @file FileReader.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines a file reading library
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <sstream>
#include "stb_image.h"

//==============================================================================
// File types

/* A file data */
struct FileString
{
    unsigned char *data;    // data string
    long long size;         // file size
};

/* An image data */
struct Bitmap
{
    int width, height;      // image dimensions
    unsigned int *pixels;   // color data
};

//==============================================================================
// File reading implementation

/* Free a readed file */
inline void FreeFile(FileString s)
{
    VirtualFree(s.data, 0, MEM_RELEASE);
}

/** 
 * Reads a file using it's path 
 * Returns a FileString containing data
 * Files must be freed after use
 */
inline FileString ReadFile(const char *filePath)
{
    FileString result = {0};

    HANDLE handle = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (handle == INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        return result;
    }

    DWORD file_size = GetFileSize(handle, 0);
    result.size = file_size;
    result.data = (unsigned char *)VirtualAlloc(0, (size_t)result.size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

    DWORD bytesRead;
    ReadFile(handle, result.data, file_size, &bytesRead, 0);

    CloseHandle(handle);
    return result;
}

/** 
 * Reads an image using it's path 
 * Returns a Bitmap containing data
 */
inline Bitmap ReadImage(const char *filePath)
{
    Bitmap result;
    
    FileString image = ReadFile(filePath);
    int n;
    stbi_set_flip_vertically_on_load(1);
    result.pixels = (unsigned int *)stbi_load_from_memory(image.data, (int)image.size, &result.width, &result.height, &n, 4);
    unsigned int *pixel = result.pixels;
    for (int y = 0; y < result.height; y++) 
    {
        for (int x = 0; x < result.width; x++) 
        {
            unsigned char r = (unsigned char)(*pixel & 0x0000ff);
            unsigned char g = (unsigned char)((*pixel & 0x00ff00) >> 8);
            unsigned char b = (unsigned char)((*pixel & 0xff0000) >> 16);
            unsigned char a = (unsigned char)((*pixel & 0xff000000) >> 24);
            *pixel++ =  b | (g << 8) | (r << 16) | (a << 24);
        }
    }
    
    FreeFile(image);
    return result;
}

/**  
 * Returns the number of files in a folder
 */
inline int NumberOfFiles(const char *path)
{
    int nb = 0;
    std::stringstream fPath;
    fPath << path << "\\*";

    WIN32_FIND_DATA ffd;
    HANDLE hFind = FindFirstFileA (fPath.str().c_str(), (LPWIN32_FIND_DATAA)&ffd);
    while (FindNextFileA(hFind, (LPWIN32_FIND_DATAA)&ffd))
    {
        nb++;
    }
    FindClose(hFind);
    return nb-1;
}