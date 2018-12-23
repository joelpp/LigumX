#ifndef TEXTURE_H
#define TEXTURE_H

#include <functional>
#include <iostream>
#include <vector>
//#include "FreeImage\Dist\x64\FreeImage.h"
#include "FreeImage/Dist/x64/FreeImage.h"
#include "glm/glm.hpp"
#include "GL.h"

#include <string>

#pragma region  FORWARD_DECLARATIONS Texture
#include "LXSystem.h"

class Texture;


#pragma endregion  FORWARD_DECLARATIONS Texture

#pragma region  HEADER Texture
class Texture : public LXObject
{
public:
static const int ClassID = 2571916692;
static const LXType Type = LXType_Texture;
static constexpr const char* ClassName = "Texture";
typedef LXObject super;

const std::string& GetFilename() { return m_Filename; }; 
void SetFilename(std::string value) { m_Filename = value; }; 
bool GetIsCubeMap() { return m_IsCubeMap; }; 
void SetIsCubeMap(bool value) { m_IsCubeMap = value; }; 
const GLuint& GetHWObject() { return m_HWObject; }; 
void SetHWObject(GLuint value) { m_HWObject = value; }; 
int GetNumChannels() { return m_NumChannels; }; 
void SetNumChannels(int value) { m_NumChannels = value; }; 
int GetBitsPerPixel() { return m_BitsPerPixel; }; 
void SetBitsPerPixel(int value) { m_BitsPerPixel = value; }; 
const GLPixelFormat& GetInternalFormat() { return m_InternalFormat; }; 
void SetInternalFormat(GLPixelFormat value) { m_InternalFormat = value; }; 
const GLPixelFormat& GetFormat() { return m_Format; }; 
void SetFormat(GLPixelFormat value) { m_Format = value; }; 
const GLPixelType& GetPixelType() { return m_PixelType; }; 
void SetPixelType(GLPixelType value) { m_PixelType = value; }; 
glm::ivec2& GetSize() { return m_Size; }; 
void SetSize(const glm::ivec2& value) { m_Size = value; }; 
private:
std::string m_Filename;
bool m_IsCubeMap = false;
GLuint m_HWObject;
int m_NumChannels = 0;
int m_BitsPerPixel = 0;
GLPixelFormat m_InternalFormat;
GLPixelFormat m_Format;
GLPixelType m_PixelType;
glm::ivec2 m_Size = glm::ivec2(0, 0);
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TexturePIDX
{
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
void PostSerialization(bool writing, bool success);
virtual bool ShowPropertyGrid();

#pragma endregion  HEADER Texture

public:
	Texture::Texture();
	Texture::Texture(int objectID);
	Texture(std::string filename, bool isCubeMap = false);
	
	void GenerateMipMaps();
	void UnbindTexture(GLuint bindingTarget);
	void BindTexture(GLuint bindingTarget);


	void LoadFromFile(GLuint target, std::string filename);
	void SaveToFile(std::string fileName);
	void Initialize();
	void InitBlank();

	BYTE* GetTextureData() { return m_TextureData; };

	void GenerateFromData(std::vector<float>& data);
	void UpdateFromData();


	BYTE* m_TextureData;

	void SetMagFilterMode(GL::TextureMagFilterMode value) { m_MagFilterMode = value; }
	void SetMinFilterMode(GL::TextureMinFilterMode value) { m_MinFilterMode = value; }
	void SetWrapR(GL::TextureWrapMode value) { m_WrapR = value; }
	void SetWrapS(GL::TextureWrapMode value) { m_WrapS = value; }
	void SetWrapT(GL::TextureWrapMode value) { m_WrapT = value; }

	int GetNumTexels() { return m_Size.x * m_Size.y; }
	int GetNumBytes() { return 4 * GetNumTexels(); }

	void EditData(const glm::ivec2& startTexel, const glm::ivec2& endTexel, std::function<void(unsigned char*)> operation);

private:
	GL::TextureMagFilterMode m_MagFilterMode;
	GL::TextureMinFilterMode m_MinFilterMode;
	GL::TextureWrapMode m_WrapR;
	GL::TextureWrapMode m_WrapS;
	GL::TextureWrapMode m_WrapT;
};

#endif // TEXTURE_H
