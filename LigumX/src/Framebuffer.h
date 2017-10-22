#pragma once
#include <string>
#include <vector>

typedef unsigned int GLuint;

#pragma region  FORWARD_DECLARATIONS Framebuffer
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS Framebuffer

class Framebuffer
{
public:
public:
public:
#pragma region  HEADER Framebuffer
public:
static const int ClassID = 199658736;
static constexpr const char* ClassName = "Framebuffer";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
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
const GL::PixelFormat& GetPixelFormat() { return m_PixelFormat; }; 
void SetPixelFormat(GL::PixelFormat value) { m_PixelFormat = value; }; 
const GL::PixelFormat& GetInternalPixelFormat() { return m_InternalPixelFormat; }; 
void SetInternalPixelFormat(GL::PixelFormat value) { m_InternalPixelFormat = value; }; 
const bool& GetHasDepth() { return m_HasDepth; }; 
void SetHasDepth(bool value) { m_HasDepth = value; }; 
const int& GetNumColorTargets() { return m_NumColorTargets; }; 
void SetNumColorTargets(int value) { m_NumColorTargets = value; }; 
private:
int m_ObjectID;
std::string m_Name;
int m_Width;
int m_Height;
GLuint m_ColorTexture;
GLuint m_DepthTexture;
GL::PixelFormat m_PixelFormat;
GL::PixelFormat m_InternalPixelFormat;
bool m_HasDepth;
int m_NumColorTargets;
public:
static const int g_PropertyCount = 10;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_FramebufferPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Width,
PIDX_Height,
PIDX_ColorTexture,
PIDX_DepthTexture,
PIDX_PixelFormat,
PIDX_InternalPixelFormat,
PIDX_HasDepth,
PIDX_NumColorTargets,
};
void Serialize(bool writing);

#pragma endregion  HEADER Framebuffer

Framebuffer();
Framebuffer(std::string name, int width, int height,  GL::PixelFormat internalpixelFormat, GL::PixelFormat pixelFormat, GL::PixelType pixelType);

void Initialize();

GLuint GetHWObject() { return m_HWObject; }
void InitAttachment(GLuint& texture, GLuint attachment, GL::PixelFormat internalPixelFormat, GL::PixelFormat pixelFormat, GL::PixelType pixelType);
const GLuint& GetColorTexture(int i) { return m_ColorTextures[i]; };

private:
GLuint m_HWObject;


std::vector<GLuint> m_ColorTextures;


public:
	GL::PixelType m_PixelType;
	
	// todo : we should have a list of textures and their corresponding attachments rather than a single one
	GLuint m_Attachment;
};
