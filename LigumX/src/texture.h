#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "FreeImage\Dist\x64\FreeImage.h"
#include "glm/glm.hpp"
#include "glad\glad.h"

#include <string>

class Texture
{
public:
    GLuint glidTexture;
public:
    Texture(std::string filename);
};

#endif // TEXTURE_H
