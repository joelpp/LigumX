#pragma once

#include "GL.h"
#include <assert.h>

#pragma region  CLASS_SOURCE Framebuffer

#include "Framebuffer.h"
#include "serializer.h"
const ClassPropertyData Framebuffer::g_Properties[] = 
{
{ "Width", PIDX_Width, offsetof(Framebuffer, m_Width), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "Height", PIDX_Height, offsetof(Framebuffer, m_Height), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "ColorTexture", PIDX_ColorTexture, offsetof(Framebuffer, m_ColorTexture), 0, LXType_Object, sizeof(GLuint), LXType_GLuint, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DepthTexture", PIDX_DepthTexture, offsetof(Framebuffer, m_DepthTexture), 0, LXType_Object, sizeof(GLuint), LXType_GLuint, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "PixelFormat", PIDX_PixelFormat, offsetof(Framebuffer, m_PixelFormat), 0, LXType_Object, sizeof(GLPixelFormat), LXType_GLPixelFormat, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "InternalPixelFormat", PIDX_InternalPixelFormat, offsetof(Framebuffer, m_InternalPixelFormat), 0, LXType_Object, sizeof(GLPixelFormat), LXType_GLPixelFormat, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "HasDepth", PIDX_HasDepth, offsetof(Framebuffer, m_HasDepth), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "NumColorTargets", PIDX_NumColorTargets, offsetof(Framebuffer, m_NumColorTargets), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
};
void Framebuffer::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_Width], m_Width);
	serializer.SerializeInt(g_Properties[PIDX_Height], m_Height);
	serializer.SerializeBool(g_Properties[PIDX_HasDepth], m_HasDepth);
	serializer.SerializeInt(g_Properties[PIDX_NumColorTargets], m_NumColorTargets);
}
bool Framebuffer::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Framebuffer::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Width], &m_Width , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Height], &m_Height , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_HasDepth], &m_HasDepth  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_NumColorTargets], &m_NumColorTargets , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	return true;
}
void Framebuffer::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Framebuffer* other = (Framebuffer*) otherObj;
	other->SetWidth(m_Width);
	other->SetHeight(m_Height);
	other->SetColorTexture(m_ColorTexture);
	other->SetDepthTexture(m_DepthTexture);
	other->SetPixelFormat(m_PixelFormat);
	other->SetInternalPixelFormat(m_InternalPixelFormat);
	other->SetHasDepth(m_HasDepth);
	other->SetNumColorTargets(m_NumColorTargets);
}
const char* Framebuffer::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Framebuffer


Framebuffer::Framebuffer()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}

Framebuffer::Framebuffer(const std::string& name, int width, int height, GLPixelFormat internalpixelFormat, GLPixelFormat pixelFormat, GLPixelType pixelType)
	:	m_Width(width),
		m_Height(height),
		m_InternalPixelFormat(internalpixelFormat),
		m_PixelFormat(pixelFormat),
		m_PixelType(pixelType)
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
	SetName(name);
}

void Framebuffer::InitAttachment(GLuint& texture, GLuint attachment, GLPixelFormat internalPixelFormat, GLPixelFormat pixelFormat, GLPixelType pixelType)
{
	texture = GL::CreateTexture();
	GL::BindTexture(texture);

	// todo : textures should be able to clear themselves ? texture->Clear calls GL::Clear
	GL::ClearTexture(m_Width, m_Height, internalPixelFormat, pixelFormat, pixelType);

	// todo : set this up 
	GL::SetTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GL::SetTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::SetTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	GL::AttachTextureToFramebuffer(texture, attachment);
}

GLuint attachments[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };

void Framebuffer::Initialize()
{
	m_HWObject = GL::CreateFrameBuffer();

	if (m_HasDepth)
	{
		InitAttachment(m_DepthTexture, GL_DEPTH_ATTACHMENT, GLPixelFormat_DepthComponent, GLPixelFormat_DepthComponent, GLPixelType_Float);
	}

	for (int i = 0; i < m_NumColorTargets; ++i)
	{
		if (m_ColorTextures.size() <= i)
		{
			m_ColorTextures.push_back(0);
		}
		InitAttachment(m_ColorTextures[i], attachments[i], m_InternalPixelFormat, m_PixelFormat, m_PixelType);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(false);
	}
}

