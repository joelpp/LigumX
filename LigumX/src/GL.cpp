#include "GL.h"
#include "Logging.h"
#include "LXError.h"
#include <string>
#include <sstream>

#pragma region  CLASS_SOURCE GL
const std::string EnumValues_GLPixelType[] = 
{
"uByte",
"Byte",
"uShort",
"Short",
"uInt",
"Int",
"Float",
"uByte332",
"uByte233Rev",
"uShort565",
"uShort565Rev",
"uShort4444",
"uShort4444Rev",
"Short5551",
"uShort1555Rev",
"Int8888",
"Int8888Rev",
"Int1010102",
"Int2101010Rev",
};

const GLPixelType Indirection_GLPixelType[] =
{
	GLPixelType_uByte,
	GLPixelType_Byte,
	GLPixelType_uShort,
	GLPixelType_Short,
	GLPixelType_uInt,
	GLPixelType_Int,
	GLPixelType_Float,
	GLPixelType_uByte332,
	GLPixelType_uByte233Rev,
	GLPixelType_uShort565,
	GLPixelType_uShort565Rev,
	GLPixelType_uShort4444,
	GLPixelType_uShort4444Rev,
	GLPixelType_Short5551,
	GLPixelType_uShort1555Rev,
	GLPixelType_Int8888,
	GLPixelType_Int8888Rev,
	GLPixelType_Int1010102,
	GLPixelType_Int2101010Rev,
};
const std::string EnumValues_GLPixelFormat[] = 
{
"RED",
"RG",
"RGB",
"BGR",
"RGBA",
"BGRA",
"Ri",
"RGi",
"RGBi",
"BGRi",
"RGBAi",
"BGRAi",
"StencilIndex",
"DepthComponent",
"DepthStencil",
"RGBA16F",
"R32F",
};

const GLPixelFormat Indirection_GLPixelFormat[] =
{
	GLPixelFormat_RED,
	GLPixelFormat_RG,
	GLPixelFormat_RGB,
	GLPixelFormat_BGR,
	GLPixelFormat_RGBA,
	GLPixelFormat_BGRA,
	GLPixelFormat_Ri,
	GLPixelFormat_RGi,
	GLPixelFormat_RGBi,
	GLPixelFormat_BGRi,
	GLPixelFormat_RGBAi,
	GLPixelFormat_BGRAi,
	GLPixelFormat_StencilIndex,
	GLPixelFormat_DepthComponent,
	GLPixelFormat_DepthStencil,
	GLPixelFormat_RGBA16F,
	GLPixelFormat_R32F,
};
#pragma endregion  CLASS_SOURCE GL



















































































































bool GL::g_CheckGLErrors = false;

void GL::SetDepthFunction(GL::DepthFunction func)
{
	glDepthFunc(func);
}

void GL::SetCapability(GL::Capabilities cap, bool enabled)
{
	if (enabled)
	{
		glEnable(cap);
	}
	else
	{
		glDisable(cap);
	}
}


void GL::SetBlendMode(GLuint target, GL::BlendMode srcFunc, GL::BlendMode dstFunc)
{
	glBlendFunci(target, srcFunc, dstFunc);
}

void GL::SetBlendMode(GL::BlendMode srcFunc, GL::BlendMode dstFunc)
{
	glBlendFunc(srcFunc, dstFunc);
}

// TODO : Should this be in like a GL device object?
GLFWwindow* GL::CreateGLWindow(int width, int height, const char* title)
{
	// set window paramaters
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	return glfwCreateWindow(width, height, title, 0, NULL);
}

GLuint GL::CreateFrameBuffer()
{
	GLuint val;
	glGenFramebuffers(1, &val);
	glBindFramebuffer(GL_FRAMEBUFFER, val);

	return val;
}

GLuint GL::CreateTexture()
{
	GLuint val;
	glGenTextures(1, &val);

	return val;
}
	
void GL::BindTexture(GLuint& hwTexture)
{
	glBindTexture(GL_TEXTURE_2D, hwTexture);
}

void GL::SetTextureParameter(GLuint param, GLuint value)
{
	glTexParameteri(GL_TEXTURE_2D, param, value);
}

void GL::SetTextureParameter(TextureMagFilterMode mode)
{
	glTexParameteri(GL_TEXTURE_2D, TextureParameter::MagFilter, mode);
}

void GL::SetTextureParameter(TextureMinFilterMode mode)
{
	glTexParameteri(GL_TEXTURE_2D, TextureParameter::MinFilter, mode);
}

void GL::SetTextureParameter(TextureParameter param, TextureWrapMode value)
{
	lxAssert(param == TextureParameter::WrapR || param == TextureParameter::WrapS || param == TextureParameter::WrapT);
	glTexParameteri(GL_TEXTURE_2D, param, value);
}

void GL::ClearTexture(int width, int height, GLPixelFormat internalPixelFormat, GLPixelFormat pixelFormat, GLPixelType pixelType)
{
	glTexImage2D(GL_TEXTURE_2D, 0, internalPixelFormat, width, height, 0, pixelFormat, pixelType, 0);
}

void GL::AttachTextureToFramebuffer(GLuint texture, GLuint slot)
{
	// todo : see what's needed for mrt
	glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_2D, texture, 0);
}

void GL::BindFramebuffer(GLuint frameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void GL::SetViewport(glm::ivec2 size)
{
	glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);
}


void GL::SetViewport(int bufferWidth, int bufferHeight)
{
	glViewport(0, 0, bufferWidth, bufferHeight);
}

void GL::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GL::ClearColorAndDepthBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GL::DrawElements(PrimitiveMode mode, int numIndices, GLuint indicesType, int firstIndex)
{
#pragma warning(disable:4312) // TODO : figure out how to handle this properly
	glDrawElements(mode, numIndices, indicesType, (const GLvoid *) firstIndex);
#pragma warning(default:4312) 

	if (g_CheckGLErrors)
	{
		OutputErrors();
	}
}


void GL::DrawArrays(PrimitiveMode mode, int startingIndex, int count)
{
#pragma warning(disable:4312) // TODO : figure out how to handle this properly
	glDrawArrays(mode, startingIndex, count);
#pragma warning(default:4312) 

	if (g_CheckGLErrors)
	{
		OutputErrors();
	}
}


void GL::OutputErrors()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
			case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		PRINT(error.c_str());
		err = glGetError();
	}
}


void GL::DepthWriteEnabled(bool enabled)
{
	glDepthMask(enabled);
}
