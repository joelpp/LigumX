#include "stdafx.h"

#include "texture.h"
#include <vector>

#pragma region  CLASS_SOURCE Texture
#include "Texture.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData Texture::g_Properties[] = 
{
{ "ObjectID", offsetof(Texture, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Filename", offsetof(Texture, m_Filename), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "IsCubeMap", offsetof(Texture, m_IsCubeMap), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
};
void Texture::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing);
}

#pragma endregion  CLASS_SOURCE Texture
using namespace std;

Texture::Texture()
{
	m_ObjectID = g_ObjectIDManager->GetObjectID();
}


// create texture from file.
Texture::Texture(string filename, bool isCubeMap)
	: m_Filename(filename),
	  m_IsCubeMap(isCubeMap)
{
	m_ObjectID = g_ObjectIDManager->GetObjectID();
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

	glGenTextures(1, &glidTexture);
	glBindTexture(bindingTarget, glidTexture);

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
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int fiWidth(0), fiHeight(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
	}
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN) {
		cout << "error opening texture : " << filename << endl;
		throw std::exception();
	}

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename.c_str());
	//if the image failed to load, return failure
	if (!dib) {
		cout << "error opening texture : " << filename << endl;
		throw std::exception();
	}

	bits = FreeImage_GetBits(dib);
	unsigned int width = FreeImage_GetWidth(dib);
	unsigned int height = FreeImage_GetHeight(dib);

	unsigned int nbLevels;

	if (m_IsCubeMap)
	{
		glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);

	}
	else
	{
		glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

	}

	FreeImage_Unload(dib);
}

