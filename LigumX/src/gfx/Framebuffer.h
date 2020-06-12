#pragma once
#include <string>
#include <vector>

typedef unsigned int GLuint;
enum GLPixelFormat;
enum GLPixelType;

#pragma region  FORWARD_DECLARATIONS Framebuffer
#include "LXSystem.h"
class Serializer2;

class Framebuffer;


#pragma endregion  FORWARD_DECLARATIONS Framebuffer

#pragma region  HEADER Framebuffer
class Framebuffer : public LXObject
{
public:
static const int ClassID = 1477904080;
static const LXType Type = LXType_Framebuffer;
static constexpr const char* ClassName = "Framebuffer";
virtual LXType GetLXType() { return LXType_Framebuffer; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

int GetWidth() { return m_Width; }; 
void SetWidth(int value) { m_Width = value; }; 
int GetHeight() { return m_Height; }; 
void SetHeight(int value) { m_Height = value; }; 
const GLuint& GetColorTexture() { return m_ColorTexture; }; 
void SetColorTexture(GLuint value) { m_ColorTexture = value; }; 
const GLuint& GetDepthTexture() { return m_DepthTexture; }; 
void SetDepthTexture(GLuint value) { m_DepthTexture = value; }; 
const GLPixelFormat& GetPixelFormat() { return m_PixelFormat; }; 
void SetPixelFormat(GLPixelFormat value) { m_PixelFormat = value; }; 
const GLPixelFormat& GetInternalPixelFormat() { return m_InternalPixelFormat; }; 
void SetInternalPixelFormat(GLPixelFormat value) { m_InternalPixelFormat = value; }; 
bool GetHasDepth() { return m_HasDepth; }; 
void SetHasDepth(bool value) { m_HasDepth = value; }; 
int GetNumColorTargets() { return m_NumColorTargets; }; 
void SetNumColorTargets(int value) { m_NumColorTargets = value; }; 
private:
int m_Width = 0;
int m_Height = 0;
GLuint m_ColorTexture;
GLuint m_DepthTexture;
GLPixelFormat m_PixelFormat;
GLPixelFormat m_InternalPixelFormat;
bool m_HasDepth = false;
int m_NumColorTargets = 0;
public:
static const int g_PropertyCount = 8;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_FramebufferPIDX
{
PIDX_Width,
PIDX_Height,
PIDX_ColorTexture,
PIDX_DepthTexture,
PIDX_PixelFormat,
PIDX_InternalPixelFormat,
PIDX_HasDepth,
PIDX_NumColorTargets,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER Framebuffer

Framebuffer();
Framebuffer(const std::string& name, int width, int height,  GLPixelFormat internalpixelFormat, GLPixelFormat pixelFormat, GLPixelType pixelType);

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