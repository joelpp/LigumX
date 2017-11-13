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

class Texture;


#pragma endregion  FORWARD_DECLARATIONS Texture
class Texture
{
#pragma region  HEADER Texture
public:
static const int ClassID = 2571916692;
static const LXType Type = LXType_Texture;
static constexpr const char* ClassName = "Texture";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
const bool& GetIsCubeMap() { return m_IsCubeMap; }; 
void SetIsCubeMap(bool value) { m_IsCubeMap = value; }; 
const GLuint& GetHWObject() { return m_HWObject; }; 
void SetHWObject(GLuint value) { m_HWObject = value; }; 
const int& GetNumChannels() { return m_NumChannels; }; 
void SetNumChannels(int value) { m_NumChannels = value; }; 
const int& GetBitsPerPixel() { return m_BitsPerPixel; }; 
void SetBitsPerPixel(int value) { m_BitsPerPixel = value; }; 
const int& GetInternalFormat() { return m_InternalFormat; }; 
void SetInternalFormat(int value) { m_InternalFormat = value; }; 
const int& GetFormat() { return m_Format; }; 
void SetFormat(int value) { m_Format = value; }; 
const int& GetPixelType() { return m_PixelType; }; 
void SetPixelType(int value) { m_PixelType = value; }; 
const glm::ivec2& GetSize() { return m_Size; }; 
void SetSize(glm::ivec2 value) { m_Size = value; }; 
private:
int m_ObjectID;
std::string m_Name;
std::string m_Filename;
bool m_IsCubeMap;
GLuint m_HWObject;
int m_NumChannels;
int m_BitsPerPixel;
int m_InternalFormat;
int m_Format;
int m_PixelType;
glm::ivec2 m_Size;
public:
static const int g_PropertyCount = 11;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TexturePIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Filename,
PIDX_IsCubeMap,
PIDX_HWObject,
PIDX_NumChannels,
PIDX_BitsPerPixel,
PIDX_InternalFormat,
PIDX_Format,
PIDX_PixelType,
PIDX_Size,
};
bool Serialize(bool writing);
void PostSerialization(bool writing);

#pragma endregion  HEADER Texture

public:
	Texture::Texture();


	void LoadFromFile(GLuint target, std::string filename);
	void Initialize();


public:
    Texture(std::string filename, bool isCubeMap = false);
};

#endif // TEXTURE_H
