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
const GLuint& GetColorTexture() { return m_ColorTexture; }; 
void SetColorTexture(GLuint value) { m_ColorTexture = value; };
const GLuint& GetDepthTexture() { return m_DepthTexture; }; 
void SetDepthTexture(GLuint value) { m_DepthTexture = value; };
const bool& GetHasDepth() { return m_HasDepth; }; 
void SetHasDepth(bool value) { m_HasDepth = value; };
const int& GetNumColorTargets() { return m_NumColorTargets; }; 
void SetNumColorTargets(int value) { m_NumColorTargets = value; };
private:
std::string m_Name;
int m_Width;
int m_Height;
GLuint m_ColorTexture;
GLuint m_DepthTexture;
bool m_HasDepth;
int m_NumColorTargets;
public:
static const int g_FramebufferPropertyCount = 7;
static const ClassPropertyData g_Properties[g_FramebufferPropertyCount];


#pragma endregion  HEADER Framebuffer

Framebuffer();
Framebuffer(std::string name, int width, int height, GL::PixelFormat pixelFormat, GL::PixelType pixelType);

void Initialize();

GLuint GetHWObject() { return m_HWObject; }
void InitAttachment(GLuint& texture, GLuint attachment, GL::PixelFormat pixelFormat, GL::PixelType pixelType);

private:
GLuint m_HWObject;




public:
	// todo : check how to do enums in code gen
	GL::PixelFormat m_PixelFormat;
	GL::PixelType m_PixelType;
	
	// todo : we should have a list of textures and their corresponding attachments rather than a single one
	GLuint m_Attachment;
};
