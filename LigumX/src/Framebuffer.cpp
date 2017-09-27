#pragma once

#include "GL.h"

#pragma region  CLASS_SOURCE Framebuffer
#include "Framebuffer.h"
#include <cstddef>
const ClassPropertyData Framebuffer::g_Properties[] = 
{
{ "Name", offsetof(Framebuffer, m_Name), 0, LXType_stdstring, false,  }, 
{ "Width", offsetof(Framebuffer, m_Width), 0, LXType_int, false,  }, 
{ "Height", offsetof(Framebuffer, m_Height), 0, LXType_int, false,  }, 
{ "Texture", offsetof(Framebuffer, m_Texture), 0, LXType_GLuint, false,  }, 
};

#pragma endregion  CLASS_SOURCE Framebuffer


Framebuffer::Framebuffer()
{

}

Framebuffer::Framebuffer(std::string name, int width, int height, GL::PixelFormat pixelFormat, GL::PixelType pixelType)
	:	m_Width(width),
		m_Height(height),
		m_Name(name),
		m_PixelFormat(pixelFormat),
		m_PixelType(pixelType)
{

}


void Framebuffer::Initialize()
{
	m_HWObject = GL::CreateFrameBuffer();

	m_Texture = GL::CreateTexture();
	GL::BindTexture(m_Texture);

	// todo : textures should be able to clear themselves ? texture->Clear calls GL::Clear
	GL::ClearTexture(m_Width, m_Height, m_PixelFormat, m_PixelType);

	// todo : set this up 
	GL::SetTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GL::SetTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::SetTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	GL::AttachTextureToFramebuffer(m_Texture, m_Attachment);
}
