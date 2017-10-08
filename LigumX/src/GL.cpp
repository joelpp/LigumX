#include "GL.h"
#include "Logging.h"
#include <string>
#include <sstream>

namespace GL
{
	bool g_CheckGLErrors = false;

	void SetDepthFunction(DepthFunction func)
	{
		glDepthFunc(func);
	}

	void SetCapability(GL::Capabilities cap, bool enabled)
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

	// TODO : Should this be in like a GL device object?
	GLFWwindow* CreateGLWindow(int width, int height, const char* title)
	{
		// set window paramaters
		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		return glfwCreateWindow(width, height, title, 0, NULL);
	}

	GLuint CreateFrameBuffer()
	{
		GLuint val;
		glGenFramebuffers(1, &val);
		glBindFramebuffer(GL_FRAMEBUFFER, val);

		return val;
	}

	GLuint CreateTexture()
	{
		GLuint val;
		glGenTextures(1, &val);

		return val;
	}
	
	void BindTexture(GLuint& hwTexture)
	{
		glBindTexture(GL_TEXTURE_2D, hwTexture);
	}

	void GL::SetTextureParameter(GLuint param, GLuint value)
	{
		glTexParameteri(GL_TEXTURE_2D, param, value);
	}

	void GL::ClearTexture(int width, int height, PixelFormat pixelFormat, PixelType pixelType)
	{
		// todo problem : assuming all parameters
		glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, pixelType, 0);
	}

	void GL::AttachTextureToFramebuffer(GLuint texture, GLuint slot)
	{
		// todo : see what's needed for mrt
		glFramebufferTexture(GL_FRAMEBUFFER, slot, texture, 0);
	}

	void GL::BindFramebuffer(GLuint frameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	}

	void GL::SetViewport(int bufferWidth, int bufferHeight)
	{
		glViewport(0, 0, bufferWidth, bufferHeight);
	}

	void GL::ClearColorBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ClearColorAndDepthBuffers()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void DrawElements(GLuint primitiveType, int numIndices, GLuint indicesType, int firstIndex)
	{
		glDrawElements(primitiveType, numIndices, indicesType, (const GLvoid *)firstIndex);

		if (g_CheckGLErrors)
		{
			OutputErrors();
		}
	}

	void OutputErrors()
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

			OUTPUT_STRING_LINE(error.c_str());
			err = glGetError();
		}
	}
}