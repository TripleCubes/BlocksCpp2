#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <stb_image.h>

enum FilterType
{
    LINEAR, NEAREST
};

class Texture
{
    private:
        int width, height, numberOfColorChannels;
        unsigned int texture;

    public:
        void load(std::string path, FilterType filterType);
        unsigned int getTexture();
        int getWidth();
        int getHeight();
        void release();
};

#endif