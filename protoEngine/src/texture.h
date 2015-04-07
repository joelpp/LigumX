#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include <string>

class Texture
{
public:
    GLuint glidTexture;
public:
    Texture(std::string filename);
};

#endif // TEXTURE_H
