#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
//#include "FreeImage\Dist\x64\FreeImage.h"
#include "FreeImage/Dist/x64/FreeImage.h"
#include "glm/glm.hpp"
#include "GL.h"

#include <string>

class Texture
{
public:
    GLuint glidTexture;

	GLuint GetHWObject() { return glidTexture; };

	bool m_IsCubeMap;
	void LoadFromFile(GLuint target, std::string filename);

public:
    Texture(std::string filename, bool isCubeMap = false);
};

#endif // TEXTURE_H
