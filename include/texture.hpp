#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "util.hpp"
#include "image.h"

using namespace std;

class Texture
{
public:
    Image *color;
    int width, height;
    Texture() {}
    Texture(string filePath) // 读取BMP格式素材
    {
        if (filePath == "")
            return;
        color = Image::LoadBMP(filePath.c_str());
        width = color->Width();
        height = color->Height();
    }
};

#endif // TEXTURE_HPP
