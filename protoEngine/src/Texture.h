#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl/glew.h"
#include <string>

class Texture
{
public:
    GLuint glidTexture;
public:
    Texture(std::string filename);
};

#endif // TEXTURE_H
