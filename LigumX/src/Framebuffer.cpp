#pragma once

#include "GL.h"
#include <assert.h>

#pragma region  CLASS_SOURCE Framebuffer
#include "Framebuffer.h"
#include "serializer.h"
#include <cstddef>
const ClassPropertyData Framebuffer::g_Properties[] = 
{
{ "Name", offsetof(Framebuffer, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Width", offsetof(Framebuffer, m_Width), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Height", offsetof(Framebuffer, m_Height), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "ColorTexture", offsetof(Framebuffer, m_ColorTexture), 0, LXType_GLuint, false, LXType_None, 0, 0, 0, }, 
{ "DepthTexture", offsetof(Framebuffer, m_DepthTexture), 0, LXType_GLuint, false, LXType_None, 0, 0, 0, }, 
{ "PixelFormat", offsetof(Framebuffer, m_PixelFormat), 0, LXType_GLPixelFormat, false, LXType_None, 0, 0, 0, }, 
{ "InternalPixelFormat", offsetof(Framebuffer, m_InternalPixelFormat), 0, LXType_GLPixelFormat, false, LXType_None, 0, 0, 0, }, 
{ "HasDepth", offsetof(Framebuffer, m_HasDepth), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "NumColorTargets", offsetof(Framebuffer, m_NumColorTargets), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
};

#pragma endregion  CLASS_SOURCE Framebuffer


Framebuffer::Framebuffer()
{

}

Framebuffer::Framebuffer(std::string name, int width, int height, GL::PixelFormat internalpixelFormat, GL::PixelFormat pixelFormat, GL::PixelType pixelType)
	:	m_Width(width),
		m_Height(height),
		m_Name(name),
		m_InternalPixelFormat(internalpixelFormat),
		m_PixelFormat(pixelFormat),
		m_PixelType(pixelType)
{

}

void Framebuffer::InitAttachment(GLuint& texture, GLuint attachment, GL::PixelFormat internalPixelFormat, GL::PixelFormat pixelFormat, GL::PixelType pixelType)
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
		InitAttachment(m_DepthTexture, GL_DEPTH_ATTACHMENT, GL::PixelFormat_DepthComponent, GL::PixelFormat_DepthComponent, GL::PixelType_Float);
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

