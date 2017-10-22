#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
//#include "FreeImage\Dist\x64\FreeImage.h"
#include "FreeImage/Dist/x64/FreeImage.h"
#include "glm/glm.hpp"
#include "GL.h"

#include <string>

#pragma region  FORWARD_DECLARATIONS Texture
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS Texture
class Texture
{
#pragma region  HEADER Texture
public:
static const int ClassID = 2571916692;
const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
const bool& GetIsCubeMap() { return m_IsCubeMap; }; 
void SetIsCubeMap(bool value) { m_IsCubeMap = value; }; 
private:
int m_ObjectID;
std::string m_Filename;
bool m_IsCubeMap;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TexturePIDX
{
PIDX_ObjectID,
PIDX_Filename,
PIDX_IsCubeMap,
};
void Serialize(bool writing);

#pragma endregion  HEADER Texture

public:
	Texture::Texture();


	GLuint GetHWObject() { return glidTexture; };

	void LoadFromFile(GLuint target, std::string filename);
	void Initialize();


public:
    Texture(std::string filename, bool isCubeMap = false);
	GLuint glidTexture;
};

#endif // TEXTURE_H
