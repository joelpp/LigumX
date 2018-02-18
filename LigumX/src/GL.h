#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#pragma region  FORWARD_DECLARATIONS GL
#include "property.h"

class GL;

enum GLPixelType
{
GLPixelType_uByte = GL_UNSIGNED_BYTE,
GLPixelType_Byte = GL_BYTE,
GLPixelType_uShort = GL_UNSIGNED_SHORT,
GLPixelType_Short = GL_SHORT,
GLPixelType_uInt = GL_UNSIGNED_INT,
GLPixelType_Int = GL_INT,
GLPixelType_Float = GL_FLOAT,
GLPixelType_uByte332 = GL_UNSIGNED_BYTE_3_3_2,
GLPixelType_uByte233Rev = GL_UNSIGNED_BYTE_2_3_3_REV,
GLPixelType_uShort565 = GL_UNSIGNED_SHORT_5_6_5,
GLPixelType_uShort565Rev = GL_UNSIGNED_SHORT_5_6_5_REV,
GLPixelType_uShort4444 = GL_UNSIGNED_SHORT_4_4_4_4,
GLPixelType_uShort4444Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
GLPixelType_Short5551 = GL_UNSIGNED_SHORT_5_5_5_1,
GLPixelType_uShort1555Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
GLPixelType_Int8888 = GL_UNSIGNED_INT_8_8_8_8,
GLPixelType_Int8888Rev = GL_UNSIGNED_INT_8_8_8_8_REV,
GLPixelType_Int1010102 = GL_UNSIGNED_INT_10_10_10_2,
GLPixelType_Int2101010Rev = GL_UNSIGNED_INT_2_10_10_10_REV,
};

extern const std::string EnumValues_GLPixelType[19];
extern const GLPixelType Indirection_GLPixelType[19];
const int EnumLength_GLPixelType = 19;

enum GLPixelFormat
{
GLPixelFormat_RED = GL_RED,
GLPixelFormat_RG = GL_RG,
GLPixelFormat_RGB = GL_RGB,
GLPixelFormat_BGR = GL_BGR,
GLPixelFormat_RGBA = GL_RGBA,
GLPixelFormat_BGRA = GL_BGRA,
GLPixelFormat_Ri = GL_RED_INTEGER,
GLPixelFormat_RGi = GL_RG_INTEGER,
GLPixelFormat_RGBi = GL_RGB_INTEGER,
GLPixelFormat_BGRi = GL_BGR_INTEGER,
GLPixelFormat_RGBAi = GL_RGBA_INTEGER,
GLPixelFormat_BGRAi = GL_BGRA_INTEGER,
GLPixelFormat_StencilIndex = GL_STENCIL_INDEX,
GLPixelFormat_DepthComponent = GL_DEPTH_COMPONENT,
GLPixelFormat_DepthStencil = GL_DEPTH_STENCIL,
GLPixelFormat_RGBA16F = GL_RGBA16F,
GLPixelFormat_R32F = GL_R32F,
};

extern const std::string EnumValues_GLPixelFormat[17];
extern const GLPixelFormat Indirection_GLPixelFormat[17];
const int EnumLength_GLPixelFormat = 17;


#pragma endregion  FORWARD_DECLARATIONS GL
class GL
{
#pragma region  HEADER GL
public:
static const int ClassID = 1657862494;
static const LXType Type = LXType_GL;
static constexpr const char* ClassName = "GL";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
private:
int m_ObjectID;
std::string m_Name;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_GLPIDX
{
PIDX_ObjectID,
PIDX_Name,
};
bool Serialize(bool writing);

#pragma endregion  HEADER GL
public:

	enum PrimitiveMode
	{
		Points					= GL_POINTS,
		LineStrip				= GL_LINE_STRIP,
		LineLoop				= GL_LINE_LOOP,
		Lines					= GL_LINES,
		LineStripAdjacency		= GL_LINE_STRIP_ADJACENCY,
		LinesAdjacency			= GL_LINES_ADJACENCY,
		TriangleStrip			= GL_TRIANGLE_STRIP,
		TriangleFan				= GL_TRIANGLE_FAN,
		Triangles				= GL_TRIANGLES,
		TriangleStripAdjacency	= GL_TRIANGLE_STRIP_ADJACENCY,
		TrianglesAdjacency		= GL_TRIANGLES_ADJACENCY,
	};


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

	enum BlendMode
	{
		Zero					= GL_ZERO,
		One						= GL_ONE,
		SrcColor				= GL_SRC_COLOR,
		OneMinusSrcColor		= GL_ONE_MINUS_SRC_COLOR,
		DstColor				= GL_DST_COLOR,
		OneMinusDstColor		= GL_ONE_MINUS_DST_COLOR,
		SrcAlpha				= GL_SRC_ALPHA,
		OneMinusSrcAlpha		= GL_ONE_MINUS_SRC_ALPHA,
		DstAlpha				= GL_DST_ALPHA,
		OneMinusDstAlpha		= GL_ONE_MINUS_DST_ALPHA,
		ConstantColor			= GL_CONSTANT_COLOR,
		OneMinusConstantColor	= GL_ONE_MINUS_CONSTANT_COLOR,
		ConstantAlpha			= GL_CONSTANT_ALPHA,
		OneMinusConstantAlpha	= GL_ONE_MINUS_CONSTANT_ALPHA,
		SrcAlphaSaturate		= GL_SRC_ALPHA_SATURATE,
		Src1Color				= GL_SRC1_COLOR,
		OneMinusSrc1Color		= GL_ONE_MINUS_SRC1_COLOR,
		Src1Alpha				= GL_SRC1_ALPHA,
		OneMinusSrc1Alpha		= GL_ONE_MINUS_SRC1_ALPHA
	};



	static void SetDepthFunction(DepthFunction func);

	static void SetCapability(GL::Capabilities cap, bool enabled);

	static void SetBlendMode(GL::BlendMode srcFunc, GL::BlendMode dstFunc);
	static void SetBlendMode(GLuint target, GL::BlendMode srcFunc, GL::BlendMode dstFunc);

	static void DepthWriteEnabled(bool enabled);



	static GLFWwindow* CreateGLWindow(int width, int height, const char* title);
	static GLuint CreateFrameBuffer();
	static GLuint CreateTexture();


	static void BindTexture(GLuint& hwTexture);

	static void ClearTexture(int width, int height, GLPixelFormat internalPixelFormat, GLPixelFormat pixelFormat, GLPixelType pixelType);

	static void SetTextureParameter(GLuint param, GLuint value);

	static void AttachTextureToFramebuffer(GLuint texture, GLuint slot);

	static void BindFramebuffer(GLuint frameBuffer);

	static void ClearColorBuffer();

	static void SetViewport(int bufferWidth, int bufferHeight);
	static void SetViewport(glm::vec2 size);
	static void SetViewport(glm::ivec2 size);

	static void ClearColorAndDepthBuffers();

	static void ClearDepthBuffer();

	static void DrawElements(PrimitiveMode primitiveType, int numIndices, GLuint indicesType, int firstIndex);
	static void DrawArrays(PrimitiveMode mode, int startingIndex, int count);

	static void OutputErrors();

	static bool g_CheckGLErrors;

};
