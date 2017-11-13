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
{ "InternalFormat", PIDX_InternalFormat, offsetof(Texture, m_InternalFormat), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Format", PIDX_Format, offsetof(Texture, m_Format), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "PixelType", PIDX_PixelType, offsetof(Texture, m_PixelType), 0, LXType_GLPixelType, false, LXType_None, 0, 0, 0, }, 
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
{
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}


// create texture from file.
Texture::Texture(string filename, bool isCubeMap)
	: m_Filename(filename),
	  m_IsCubeMap(isCubeMap)
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

	glTexParameteri(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(bindingTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(bindingTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int i = 0;
	for (std::string& file : filenames)
	{
		if (m_IsCubeMap)
		{
			target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		}

		LoadFromFile(target, filenames[i]);
		i++;
	}

	glGenerateMipmap(bindingTarget);

	glBindTexture(bindingTarget, 0);
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

	GLuint internalFormat = GL_RGBA;
	GLuint format = GL_BGRA;
	GL::PixelType type = GL::PixelType_uByte;
	
	if (m_ObjectID == 23389)
	{
		FIBITMAP* bitmapRF;
		bitmap32 = FreeImage_ConvertToFloat(bitmap32);

		bits = FreeImage_GetBits(bitmap32);

		internalFormat = GL_R32F;
		format = GL_RED;
		type = GL::PixelType_Float;
	}

	SetSize(glm::ivec2(width, height));
	SetBitsPerPixel(bitsPerPixel);

	SetInternalFormat(internalFormat);
	SetFormat(format);
	SetPixelType(type);

	glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, bits);
	Renderer::outputGLError(__func__, __LINE__);

	FreeImage_Unload(bitmap32);

	// If we had to do a conversion to 32-bit colour, then unload the original
	// non-32-bit-colour version of the image data too. Otherwise, bitmap32 and
	// bitmap point at the same data, and that data's already been free'd, so
	// don't attempt to free it again! (or we'll crash).
	if (bitsPerPixel != 32)
	{
		FreeImage_Unload(bitmap);
	}
}

