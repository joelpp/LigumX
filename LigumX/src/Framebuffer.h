#pragma once
#include <string>

typedef unsigned int GLuint;

#pragma region  FORWARD_DECLARATIONS Framebuffer
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS Framebuffer

class Framebuffer
{
#pragma region  HEADER Framebuffer
static const int ClassID = 199658736;
public:
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; };
const int& GetWidth() { return m_Width; }; 
void SetWidth(int value) { m_Width = value; };
const int& GetHeight() { return m_Height; }; 
void SetHeight(int value) { m_Height = value; };
const GLuint& GetTexture() { return m_Texture; }; 
void SetTexture(GLuint value) { m_Texture = value; };
private:
std::string m_Name;
int m_Width;
int m_Height;
GLuint m_Texture;
public:
static const int g_FramebufferPropertyCount = 4;
static const ClassPropertyData g_Properties[g_FramebufferPropertyCount];


#pragma endregion  HEADER Framebuffer

Framebuffer();
Framebuffer(std::string name, int width, int height, GL::PixelFormat pixelFormat, GL::PixelType pixelType);

void Initialize();

GLuint GetHWObject() { return m_HWObject; }

private:
GLuint m_HWObject;




public:
	// todo : check how to do enums in code gen
	GL::PixelFormat m_PixelFormat;
	GL::PixelType m_PixelType;
	
	// todo : we should have a list of textures and their corresponding attachments rather than a single one
	GLuint m_Attachment;
};
