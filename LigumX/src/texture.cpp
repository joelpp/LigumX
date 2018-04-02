#include "stdafx.h"

#include "Renderer.h"
#include "texture.h"
#include <vector>

#pragma region  CLASS_SOURCE Texture

#include "Texture.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Texture::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(Texture, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(Texture, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Filename", PIDX_Filename, offsetof(Texture, m_Filename), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "IsCubeMap", PIDX_IsCubeMap, offsetof(Texture, m_IsCubeMap), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "HWObject", PIDX_HWObject, offsetof(Texture, m_HWObject), 0, LXType_GLuint, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "NumChannels", PIDX_NumChannels, offsetof(Texture, m_NumChannels), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "BitsPerPixel", PIDX_BitsPerPixel, offsetof(Texture, m_BitsPerPixel), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "InternalFormat", PIDX_InternalFormat, offsetof(Texture, m_InternalFormat), 0, LXType_GLPixelFormat, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
{ "Format", PIDX_Format, offsetof(Texture, m_Format), 0, LXType_GLPixelFormat, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
{ "PixelType", PIDX_PixelType, offsetof(Texture, m_PixelType), 0, LXType_GLPixelType, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
{ "Size", PIDX_Size, offsetof(Texture, m_Size), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
};
bool Texture::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing);
	return success;
}

#pragma endregion  CLASS_SOURCE Texture
using namespace std;

Texture::Texture()
	: m_MagFilterMode(GL::TextureMagFilterMode_Linear)
	, m_MinFilterMode(GL::TextureMinFilterMode_LinearMipMapLinear)
	, m_WrapR(GL::Repeat)
	, m_WrapS(GL::Repeat)
	, m_WrapT(GL::Repeat)
{
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

Texture::Texture(int objectID)
	: m_MagFilterMode(GL::TextureMagFilterMode_Linear)
	, m_MinFilterMode(GL::TextureMinFilterMode_LinearMipMapLinear)
	, m_WrapR(GL::Repeat)
	, m_WrapS(GL::Repeat)
	, m_WrapT(GL::Repeat)
{
	SetObjectID(objectID);
	Serialize(false);
}



// create texture from file.
Texture::Texture(string filename, bool isCubeMap)
	: m_Filename(filename)
	, m_IsCubeMap(isCubeMap)
	, m_MagFilterMode(GL::TextureMagFilterMode_Linear)
	, m_MinFilterMode(GL::TextureMinFilterMode_LinearMipMapLinear)
	, m_WrapR(GL::Repeat)
	, m_WrapS(GL::Repeat)
	, m_WrapT(GL::Repeat)
{
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

void Texture::PostSerialization(bool writing)
{
	if (!writing)
	{
		Initialize();
	}
}

void Texture::Initialize()
{
	std::vector<std::string> filenames;
	GLuint target;
	if (!m_IsCubeMap)
	{
		filenames.push_back(m_Filename);
		target = GL_TEXTURE_2D;
	}
	else
	{
		for (int i = 0; i < 6; ++i)
		{
			std::string names[] =
			{
				"front",
				"back",
				"right",
				"left",
				"top",
				"bottom",
			};
			filenames.push_back(m_Filename + names[i] + ".jpg");
		}

	}
	GLuint bindingTarget = GL_TEXTURE_2D;
	if (m_IsCubeMap)
	{
		bindingTarget = GL_TEXTURE_CUBE_MAP;
	}

	glGenTextures(1, &m_HWObject);
	glBindTexture(bindingTarget, m_HWObject);

	GL::SetTextureParameter(m_MinFilterMode);
	GL::SetTextureParameter(m_MagFilterMode);
	GL::SetTextureParameter(GL::WrapR, m_WrapR);
	GL::SetTextureParameter(GL::WrapS, m_WrapS);
	GL::SetTextureParameter(GL::WrapT, m_WrapT);

	int i = 0;
	for (std::string& file : filenames)
	{
		if (m_IsCubeMap)
		{
			target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		}

		if (filenames[i].size() > 0)
		{
			LoadFromFile(target, filenames[i]);
		}
		else
		{
			LoadBlank(target);
		}

		i++;
	}

	glGenerateMipmap(bindingTarget);

	glBindTexture(bindingTarget, 0);
}

void Texture::LoadBlank(GLuint target)
{
	glTexImage2D(target, 0, m_InternalFormat, m_Size.x, m_Size.y, 0, m_Format, m_PixelType, m_TextureData);

}


void Texture::LoadFromFile(GLuint target, std::string filename)
{
	std::string fullName = g_PathTextures + filename;

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *bitmap(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int fiWidth(0), fiHeight(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(fullName.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(fullName.c_str());
	}
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN) {
		cout << "error opening texture : " << fullName << endl;
		throw std::exception();
	}

	if (FreeImage_FIFSupportsReading(fif))
		bitmap = FreeImage_Load(fif, fullName.c_str());
	//if the image failed to load, return failure
	if (!bitmap) {
		cout << "error opening texture : " << fullName << endl;
		throw std::exception();
	}

	// How many bits-per-pixel is the source image?
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	// Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
	// but only if the image is not already 32 bits (i.e. 8 bits per channel).
	// Note: ConvertTo32Bits returns a CLONE of the image data - so if we
	// allocate this back to itself without using our bitmap32 intermediate
	// we will LEAK the original bitmap data, and valgrind will show things like this:
	//
	// LEAK SUMMARY:
	//  definitely lost: 24 bytes in 2 blocks
	//  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
	//
	// Using our intermediate and cleaning up the initial bitmap data we get:
	//
	// LEAK SUMMARY:
	//  definitely lost: 16 bytes in 1 blocks
	//  indirectly lost: 176 bytes in 4 blocks
	//
	// All above leaks (192 bytes) are caused by XGetDefault (in /usr/lib/libX11.so.6.3.0) - we have no control over this.
	//
	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32)
	{
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
		bitmap32 = bitmap;
	}
	else
	{
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	bits = FreeImage_GetBits(bitmap32);
	unsigned int width = FreeImage_GetWidth(bitmap);
	unsigned int height = FreeImage_GetHeight(bitmap);

	GLPixelFormat internalFormat = GLPixelFormat_RGBA;
	GLPixelFormat format = GLPixelFormat_BGRA;
	GLPixelType type = GLPixelType_uByte;
	
	if (m_ObjectID == 23389)
	{
		bitmap32 = FreeImage_ConvertToFloat(bitmap32);

		bits = FreeImage_GetBits(bitmap32);

		internalFormat = GLPixelFormat_R32F;
		format = GLPixelFormat_RED;
		type = GLPixelType_Float;
	}

	//if (m_ObjectID == 48463)
	//{
	//	internalFormat = GLPixelFormat_RGBi;
	//	format = GLPixelFormat_BGRAi;
	//	type = GLPixelType_Int;
	//}

	SetSize(glm::ivec2(width, height));
	SetBitsPerPixel(bitsPerPixel);

	SetInternalFormat(internalFormat);
	SetFormat(format);
	SetPixelType(type);

	glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, bits);
	Renderer::outputGLError(__func__, __LINE__);

	if (m_ObjectID == 48463)
	{
		m_TextureData = bits;
	}
	else
	{
		m_TextureData = nullptr;
		FreeImage_Unload(bitmap32);
	}

	// If we had to do a conversion to 32-bit colour, then unload the original
	// non-32-bit-colour version of the image data too. Otherwise, bitmap32 and
	// bitmap point at the same data, and that data's already been free'd, so
	// don't attempt to free it again! (or we'll crash).
	if (bitsPerPixel != 32)
	{
		FreeImage_Unload(bitmap);
	}
}

void Texture::SaveToFile(std::string fileName)
{
	//std::vector<GLubyte> pixels(4 * m_Size.x * m_Size.y);
	//glReadPixels(0, 0, m_Size.x, m_Size.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	//std::vector<float> floats(m_Size.x * m_Size.y);
	//float* data = (float*) m_TextureData;

	//for (int i = 0; i < m_Size.x; ++i)
	//{
	//	for (int j = 0; j < m_Size.y; ++j)
	//	{
	//		float& val = floats[i * m_Size.y + j];
	//		val = data[i * m_Size.y + j] == 0.f ? 0.f : (~(0));
	//	}
	//}

	FIBITMAP* image = FreeImage_ConvertFromRawBits(m_TextureData, m_Size.x, m_Size.y, 4 * m_Size.y, 32, 0x000000, 0x000000, 0x000000, false);

	if(FreeImage_Save(FIF_PNG, image, fileName.c_str(), 0)) {
		std::cout << "Save successful." << std::endl;
	} else {
		std::cout << "Save failed." << std::endl;
	}
	FreeImage_Unload(image);
}

void Texture::GenerateFromData(std::vector<float>& data)
{
	GLuint bindingTarget = GL_TEXTURE_2D;

	if (m_IsCubeMap)
	{
		bindingTarget = GL_TEXTURE_CUBE_MAP;
	}

	glGenTextures(1, &m_HWObject);
	glBindTexture(bindingTarget, m_HWObject);

	GL::SetTextureParameter(m_MinFilterMode);
	GL::SetTextureParameter(m_MagFilterMode);
	GL::SetTextureParameter(GL::WrapR,	m_WrapR);
	GL::SetTextureParameter(GL::WrapS,	m_WrapS);
	GL::SetTextureParameter(GL::WrapT,	m_WrapT);

	int i = 0;

	char* bytes = (char*)data.data();
	glTexImage2D(bindingTarget, 0, m_InternalFormat, m_Size.x, m_Size.y, 0, m_Format, m_PixelType, bytes);

	glGenerateMipmap(bindingTarget);

	glBindTexture(bindingTarget, 0);
}

void Texture::UpdateFromData()
{
	glBindTexture(GL_TEXTURE_2D, m_HWObject);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Size.x, m_Size.y, m_Format, m_PixelType, m_TextureData);
	glBindTexture(GL_TEXTURE_2D, 0);
}



void Texture::InitBlank(glm::ivec2 size)
{
	SetBitsPerPixel(32);
	SetSize(glm::ivec2(size.x, size.y));
	int numBytes = m_BitsPerPixel * size.x * size.y / 4;

	m_TextureData = (BYTE*) malloc(numBytes);
	memset(m_TextureData, 0, numBytes);

	GLPixelFormat internalFormat = GLPixelFormat_RGBA;
	GLPixelFormat format = GLPixelFormat_BGRA;
	GLPixelType type = GLPixelType_uByte;

	SetInternalFormat(internalFormat);
	SetFormat(format);
	SetPixelType(type);
	SetIsCubeMap(false);

	Initialize();

	//glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, bits);
	Renderer::outputGLError(__func__, __LINE__);
}
