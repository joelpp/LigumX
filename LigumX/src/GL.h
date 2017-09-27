#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace GL
{
	enum DepthFunction
	{
		Depth_Never		= GL_NEVER,
		Depth_Less		= GL_LESS,
		Depth_Equal		= GL_EQUAL,
		Depth_LEqual	= GL_LEQUAL,
		Depth_Greater	= GL_GREATER,
		Depth_NotEqual	= GL_NOTEQUAL,
		Depth_GEqual	= GL_GEQUAL,
		Depth_Always	= GL_ALWAYS,
	};

	enum Capabilities
	{
		Blend					= GL_BLEND,
		ClipDistance0			= GL_CLIP_DISTANCE0,
		ColorLogicOp			= GL_COLOR_LOGIC_OP,
		CullFace				= GL_CULL_FACE,
		DepthClamp				= GL_DEPTH_CLAMP,
		DepthTest				= GL_DEPTH_TEST,
		Dither					= GL_DITHER,
		FramebufferSRGB			= GL_FRAMEBUFFER_SRGB,
		LineSmooth				= GL_LINE_SMOOTH,
		MultiSample				= GL_MULTISAMPLE,
		PolygonOffsetFill		= GL_POLYGON_OFFSET_FILL,
		PolygonOffsetLine		= GL_POLYGON_OFFSET_LINE,
		PolygonOffsetPoint		= GL_POLYGON_OFFSET_POINT,
		PolygonSmooth			= GL_POLYGON_SMOOTH,
		PrimitiveRestart		= GL_PRIMITIVE_RESTART,
		SampleAlphaToCoverage	= GL_SAMPLE_ALPHA_TO_COVERAGE,
		SampleAlphaToOne		= GL_SAMPLE_ALPHA_TO_ONE,
		SampleCoverage			= GL_SAMPLE_COVERAGE,
		ScissorTest				= GL_SCISSOR_TEST,
		StencilTest				= GL_STENCIL_TEST,
		TextureCubemapSeamless	= GL_TEXTURE_CUBE_MAP_SEAMLESS,
		ProgramPointSize		= GL_PROGRAM_POINT_SIZE,
	};

	enum PixelFormat
	{
		PixelFormat_R				= GL_RED,
		PixelFormat_RG				= GL_RG,
		PixelFormat_RGB				= GL_RGB,
		PixelFormat_BGR				= GL_BGR,
		PixelFormat_RGBA			= GL_RGBA,
		PixelFormat_BGRA			= GL_BGRA,
		PixelFormat_Ri				= GL_RED_INTEGER,
		PixelFormat_RGi				= GL_RG_INTEGER,
		PixelFormat_RGBi			= GL_RGB_INTEGER,
		PixelFormat_BGRi			= GL_BGR_INTEGER,
		PixelFormat_RGBAi			= GL_RGBA_INTEGER,
		PixelFormat_BGRAi			= GL_BGRA_INTEGER,
		PixelFormat_StencilIndex	= GL_STENCIL_INDEX,
		PixelFormat_DepthComponent	= GL_DEPTH_COMPONENT,
		PixelFormat_DepthStencil	= GL_DEPTH_STENCIL
	};

	enum PixelType
	{
		PixelType_uByte = GL_UNSIGNED_BYTE, 
		PixelType_Byte = GL_BYTE, 
		PixelType_uShort = GL_UNSIGNED_SHORT, 
		PixelType_Short = GL_SHORT, 
		PixelType_uInt = GL_UNSIGNED_INT, 
		PixelType_Int = GL_INT, 
		PixelType_Float = GL_FLOAT, 
		PixelType_uByte332 = GL_UNSIGNED_BYTE_3_3_2, 
		PixelType_uByte233Rev = GL_UNSIGNED_BYTE_2_3_3_REV, 
		PixelType_uShort565 = GL_UNSIGNED_SHORT_5_6_5, 
		PixelType_uShort565Rev = GL_UNSIGNED_SHORT_5_6_5_REV, 
		PixelType_uShort4444 = GL_UNSIGNED_SHORT_4_4_4_4, 
		PixelType_uShort4444Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV, 
		PixelType_Short5551 = GL_UNSIGNED_SHORT_5_5_5_1, 
		PixelType_uShort1555Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV, 
		PixelType_Int8888 = GL_UNSIGNED_INT_8_8_8_8, 
		PixelType_Int8888Rev = GL_UNSIGNED_INT_8_8_8_8_REV, 
		PixelType_Int1010102 = GL_UNSIGNED_INT_10_10_10_2, 
		PixelType_Int2101010Rev = GL_UNSIGNED_INT_2_10_10_10_REV
	};



	void SetDepthFunction(DepthFunction func);
	void SetCapability(GL::Capabilities cap, bool enabled);
	GLFWwindow* CreateGLWindow(int width, int height, const char* title);
	GLuint CreateFrameBuffer();
	GLuint CreateTexture();
	void BindTexture(GLuint& hwTexture);

	void ClearTexture(int width, int height, PixelFormat pixelFormat, PixelType pixelType);

	void SetTextureParameter(GLuint param, GLuint value);

	void AttachTextureToFramebuffer(GLuint texture, GLuint slot);

	void BindFramebuffer(GLuint frameBuffer);

	void ClearColorBuffer();

	void SetViewport(int bufferWidth, int bufferHeight);

	void ClearColorAndDepthBuffers();

	void ClearDepthBuffer();

}