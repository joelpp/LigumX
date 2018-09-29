#pragma once
#include <string>
#include <vector>

typedef unsigned int GLuint;

#pragma region  FORWARD_DECLARATIONS Framebuffer
#include "property.h"

class Framebuffer;


#pragma endregion  FORWARD_DECLARATIONS Framebuffer

class Framebuffer
{
public:
public:
public:
#pragma region  HEADER Framebuffer
public:
static const int ClassID = 199658736;
static const LXType Type = LXType_Framebuffer;
static constexpr const char* ClassName = "Framebuffer";

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
int GetWidth() { return m_Width; }; 
void SetWidth(int value) { m_Width = value; }; 
int GetHeight() { return m_Height; }; 
void SetHeight(int value) { m_Height = value; }; 
GLuint GetColorTexture() { return m_ColorTexture; }; 
void SetColorTexture(GLuint value) { m_ColorTexture = value; }; 
GLuint GetDepthTexture() { return m_DepthTexture; }; 
void SetDepthTexture(GLuint value) { m_DepthTexture = value; }; 
GLPixelFormat GetPixelFormat() { return m_PixelFormat; }; 
void SetPixelFormat(GLPixelFormat value) { m_PixelFormat = value; }; 
GLPixelFormat GetInternalPixelFormat() { return m_InternalPixelFormat; }; 
void SetInternalPixelFormat(GLPixelFormat value) { m_InternalPixelFormat = value; }; 
bool GetHasDepth() { return m_HasDepth; }; 
void SetHasDepth(bool value) { m_HasDepth = value; }; 
int GetNumColorTargets() { return m_NumColorTargets; }; 
void SetNumColorTargets(int value) { m_NumColorTargets = value; }; 
private:
int m_ObjectID;
std::string m_Name;
int m_Width = 0;
int m_Height = 0;
GLuint m_ColorTexture;
GLuint m_DepthTexture;
GLPixelFormat m_PixelFormat;
GLPixelFormat m_InternalPixelFormat;
bool m_HasDepth = false;
int m_NumColorTargets = 0;
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
bool Serialize(bool writing);

#pragma endregion  HEADER Framebuffer

Framebuffer();
Framebuffer(std::string name, int width, int height,  GLPixelFormat internalpixelFormat, GLPixelFormat pixelFormat, GLPixelType pixelType);

void Initialize();

GLuint GetHWObject() { return m_HWObject; }
void InitAttachment(GLuint& texture, GLuint attachment, GLPixelFormat internalPixelFormat, GLPixelFormat pixelFormat, GLPixelType pixelType);
const GLuint& GetColorTexture(int i) { return m_ColorTextures[i]; };

private:
GLuint m_HWObject;


std::vector<GLuint> m_ColorTextures;


public:
	GLPixelType m_PixelType;
	
	// todo : we should have a list of textures and their corresponding attachments rather than a single one
	GLuint m_Attachment;
};
