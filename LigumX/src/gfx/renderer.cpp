
#include "renderer.h"

#include "ProgramPipeline.h"
#include "RenderDataManager.h"
#include "GL.h"
#include "MainWindow.h"

#include "SectorGraphicalData.h"
#include "DefaultObjects.h"
#include "DefaultTextureHolder.h"
#include "Framebuffer.h"

#include "TerrainTool.h"
#include "SectorTool.h"
#include "Editor.h"
#include "EditorOptions.h"

#include "Settings.h"
#include "LXError.h"
#include "EngineStats.h"
#include "EngineSettings.h"

#include "PostEffects.h"

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "World.h"
#include "Entity.h"
#include "Visual.h"
#include "OSMElementComponent.h"
#include "Sector.h"
#include "Sunlight.h"
#include "BoundingBoxComponent.h"
#include "Heightfield.h"

#include "inputhandler.h" // todo jpp for data inspector, clean if not needed anymore
#include "RenderdocManager.h" // todo jpp for data inspector, clean if not needed anymore

#pragma region  CLASS_SOURCE Renderer

#include "Renderer.h"
#include "serializer.h"
#include "DisplayOptions.h"
#include "PostEffects.h"
#include "LightingOptions.h"
#include "Camera.h"
#include "MainWindow.h"
const ClassPropertyData Renderer::g_Properties[] = 
{
{ "DisplayOptions", PIDX_DisplayOptions, offsetof(Renderer, m_DisplayOptions), 0, LXType_ObjectPtr, sizeof(DisplayOptions*), LXType_DisplayOptions, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "PostEffects", PIDX_PostEffects, offsetof(Renderer, m_PostEffects), 0, LXType_ObjectPtr, sizeof(PostEffects*), LXType_PostEffects, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "LightingOptions", PIDX_LightingOptions, offsetof(Renderer, m_LightingOptions), 0, LXType_ObjectPtr, sizeof(LightingOptions*), LXType_LightingOptions, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugCamera", PIDX_DebugCamera, offsetof(Renderer, m_DebugCamera), 0, LXType_ObjectPtr, sizeof(Camera*), LXType_Camera, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "ActiveCamera", PIDX_ActiveCamera, offsetof(Renderer, m_ActiveCamera), 0, LXType_ObjectPtr, sizeof(Camera*), LXType_Camera, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "MainWindow", PIDX_MainWindow, offsetof(Renderer, m_MainWindow), 0, LXType_ObjectPtr, sizeof(MainWindow*), LXType_MainWindow, true, LXType_None, false, 0, 0, 0, 0,}, 
};
void Renderer::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeObjectPtr(g_Properties[PIDX_DisplayOptions], m_DisplayOptions);
	serializer.SerializeObjectPtr(g_Properties[PIDX_PostEffects], m_PostEffects);
	serializer.SerializeObjectPtr(g_Properties[PIDX_LightingOptions], m_LightingOptions);
	serializer.SerializeObjectPtr(g_Properties[PIDX_DebugCamera], m_DebugCamera);
	serializer.SerializeObjectPtr(g_Properties[PIDX_MainWindow], m_MainWindow);
}
bool Renderer::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool Renderer::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_DisplayOptions], &m_DisplayOptions  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_PostEffects], &m_PostEffects  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_LightingOptions], &m_LightingOptions  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_DebugCamera], &m_DebugCamera  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_ActiveCamera], &m_ActiveCamera  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_MainWindow], &m_MainWindow  );
	return true;
}
void Renderer::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Renderer* other = (Renderer*) otherObj;
	other->SetDisplayOptions(m_DisplayOptions);
	other->SetPostEffects(m_PostEffects);
	other->SetLightingOptions(m_LightingOptions);
	other->SetDebugCamera(m_DebugCamera);
	other->SetActiveCamera(m_ActiveCamera);
	other->SetMainWindow(m_MainWindow);
}
const char* Renderer::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Renderer

Renderer* g_Instance;

using namespace glm;
using namespace std;

#define CHAR_BUFFER_SIZE 64

#define FT_SUCCESS 0

Renderer::Renderer()
{
	g_Instance = this;
	SetObjectID(g_ObjectManager->GetNewObjectID());
}

void Renderer::InitFramebuffers()
{
	int windowWidth = m_MainWindow->GetSize().x;
	int windowHeight = m_MainWindow->GetSize().y;

	m_Framebuffers[FramebufferType_MainColorBuffer] = new Framebuffer("Main Color Buffer", windowWidth, windowHeight, GLPixelFormat_RGBA16F, GLPixelFormat_RGBA, GLPixelType_Float);
	m_Framebuffers[FramebufferType_MainColorBuffer]->SetHasDepth(true);
	m_Framebuffers[FramebufferType_MainColorBuffer]->SetNumColorTargets(2);
	m_Framebuffers[FramebufferType_MainColorBuffer]->Initialize();

	m_Framebuffers[FramebufferType_ShadowMap] = new Framebuffer("Shadow Map Buffer", SHADOW_WIDTH, SHADOW_HEIGHT, GLPixelFormat_DepthComponent, GLPixelFormat_DepthComponent, GLPixelType_Float);
	m_Framebuffers[FramebufferType_ShadowMap]->SetHasDepth(true);
	m_Framebuffers[FramebufferType_ShadowMap]->SetNumColorTargets(0);
	m_Framebuffers[FramebufferType_ShadowMap]->Initialize();

	m_Framebuffers[FramebufferType_Picking] = new Framebuffer("Picking Buffer", g_EngineSettings->GetPickingBufferSize(), g_EngineSettings->GetPickingBufferSize(), GLPixelFormat_RGBA16F, GLPixelFormat_RGBA, GLPixelType_Float);
	m_Framebuffers[FramebufferType_Picking]->SetHasDepth(true);
	m_Framebuffers[FramebufferType_Picking]->SetNumColorTargets(1);
	m_Framebuffers[FramebufferType_Picking]->Initialize();

	m_Framebuffers[FramebufferType_PingPong0] = new Framebuffer("Ping pong 0", windowWidth, windowHeight, GLPixelFormat_RGBA16F, GLPixelFormat_RGBA, GLPixelType_Float);
	m_Framebuffers[FramebufferType_PingPong0]->SetHasDepth(false);
	m_Framebuffers[FramebufferType_PingPong0]->SetNumColorTargets(1);
	m_Framebuffers[FramebufferType_PingPong0]->Initialize();

	m_Framebuffers[FramebufferType_PingPong1] = new Framebuffer("Ping pong 1", windowWidth, windowHeight, GLPixelFormat_RGBA16F, GLPixelFormat_RGBA, GLPixelType_Float);
	m_Framebuffers[FramebufferType_PingPong1]->SetHasDepth(false);
	m_Framebuffers[FramebufferType_PingPong1]->SetNumColorTargets(1);
	m_Framebuffers[FramebufferType_PingPong1]->Initialize();

	m_Framebuffers[FramebufferType_GBuffer] = new Framebuffer("GBuffer", windowWidth, windowHeight, GLPixelFormat_RGBA16F, GLPixelFormat_RGBA, GLPixelType_Float);
	m_Framebuffers[FramebufferType_GBuffer]->SetHasDepth(true);
	m_Framebuffers[FramebufferType_GBuffer]->SetNumColorTargets(4);
	m_Framebuffers[FramebufferType_GBuffer]->Initialize();

	BindFramebuffer(FramebufferType_Default);
}

glm::ivec2 Renderer::GetWindowSize()
{
	return m_MainWindow->GetSize();
}

void APIENTRY OutputGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::stringstream ss;

	ss << "OpenGL debug message : " << message;

	OUTPUT_STRING_LINE(ss.str());
}

struct TextInstance_VertexData
{
	glm::vec2 m_Position;
	float m_Scale;
	int m_TextureCharIndex;
};

constexpr int g_MaxNumTextQuads = 1024;

void Renderer::InitGL()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		PRINT("Failed to initialize GLFW.\n");
		return;
	}
	else {
		PRINT("Initialized GLFW.");
	}

	m_MainWindow->Initialize();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		//return -1;
		exit(1);
	}

	bool enableGLDebugOutput = true;
	if (enableGLDebugOutput)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OutputGLDebugMessageCallback, nullptr);
	}

	// GL Settings
	glfwSwapInterval(0);

	GL::SetCapability(GL::DepthTest, true);

	GL::SetDepthFunction(GL::Depth_Less);

	InitFramebuffers();

	GL::SetCapability(GL::CullFace, true);
	GL::SetCapability(GL::Dither,	true);
	GL::SetCapability(GL::TextureCubemapSeamless,	true);

	glGenBuffers(1, &m_PickingSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_PickingSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 12 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	glGenBuffers(1, &m_DataInspectorSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DataInspectorSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 64 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	glGenBuffers(1, &m_TextInstanceSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TextInstanceSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(TextInstance_VertexData) * g_MaxNumTextQuads, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

}

void Renderer::BindFramebuffer(FramebufferType buffer)
{
	if (buffer == FramebufferType_Default)
	{
		GL::BindFramebuffer(0);
		return;
	}
	GL::BindFramebuffer(m_Framebuffers[buffer]->GetHWObject());
}

void Renderer::InitFreetype()
{
	//FT_Library ft;
	//if (FT_Init_FreeType(&ft))
	//	std::cout << "ERROR::FREETYPE: Could nolololt init FreeType Library" << std::endl;

	//FT_Face face;
	//int success = FT_New_Face(ft, "data/fonts/arial.ttf", 0, &face);
	//if (success == FT_SUCCESS)
	//{
	//	PRINT("Loaded Freetype font! yayy");
	//}
	//else
	//{
	//	PRINT("Failed to load freetype font.");

	//}

	//FT_Set_Pixel_Sizes(face, 0, 48);

	//GLuint fontTexture = LX_LIMITS_INT_MAX;

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	//for (GLubyte c = 0; c < 128; c++)
	//{
	//	// Load character glyph
	//	if (FT_Load_Char(face, c, FT_LOAD_RENDER) != FT_SUCCESS)
	//	{
	//		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	//		continue;
	//	}
	//	// Generate texture
	//	GLuint texture;
	//	glGenTextures(1, &texture);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	glTexImage2D(
	//		GL_TEXTURE_2D,
	//		0,
	//		GL_RED,
	//		face->glyph->bitmap.width,
	//		face->glyph->bitmap.rows,
	//		0,
	//		GL_RED,
	//		GL_UNSIGNED_BYTE,
	//		face->glyph->bitmap.buffer
	//	);

	//	// Set texture options
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	glBindTexture(GL_TEXTURE_2D, 0);


	//	if (fontTexture == LX_LIMITS_INT_MAX)
	//	{
	//		glGenTextures(1, &fontTexture);
	//		glBindTexture(GL_TEXTURE_2D, fontTexture);
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width * 128, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//		glBindTexture(GL_TEXTURE_2D, 0);
	//	}
	//	glBindTexture(GL_TEXTURE_2D, fontTexture);
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, c * face->glyph->bitmap.width, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	// Now store character for later use
	//	Character character = 
	//	{
	//		texture,
	//		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	//		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	//		static_cast<GLuint>(face->glyph->advance.x)
	//	};
	//	Characters.insert(std::pair<GLchar, Character>(c, character));
	//}
	//FT_Done_Face(face);
	//FT_Done_FreeType(ft);

	//m_FontTexture = fontTexture;

}

void Renderer::PostSerialization(bool writing, bool success)
{
	if (!writing)
	{
		Initialize();
	}
}


void Renderer::InitPipelines()
{

	// todo : instead parse through the shaders folder and init automatically families

	init_pipelines_text();

	pPipelineBasic = new ProgramPipeline("Basic");
	pPipelineShadowMap = new ProgramPipeline("ShadowMap");
	pPipelineLines = new ProgramPipeline("general_lines");
	pPipelineGround = new ProgramPipeline("Terrain");
	//pPipelineEnvmap = new ProgramPipeline("Envmap");
	pPipelineScreenSpaceTexture = new ProgramPipeline("ScreenSpaceTexture");
	//pPipelineNodes = new ProgramPipeline("nodes"); // todo jpp revive this
	pPipelinePicking = new ProgramPipeline("picking");
	pPipelinePickingCompute = new ProgramPipeline("picking_compute", true);
	pPipelineUVEdges = new ProgramPipeline("UVEdges");
	pPipelineSolidColor = new ProgramPipeline("SolidColor");
	pPipelineBlur = new ProgramPipeline("blur");
	pPipelineGrid = new ProgramPipeline("Grid");
	//pPipelineAxisGizmo = new ProgramPipeline("AxisGizmo");
	GL::OutputErrors();


	m_Pipelines.clear();
	for (int i = 0; i < EnumLength_ShaderFamily; ++i)
	{
		m_Pipelines.push_back(new ProgramPipeline(EnumValues_ShaderFamily[i]));
		m_ShaderBeenUsedThisFrame.push_back(false);
	}

	PRINT("Successfully built all shaders!");

}



void Renderer::Initialize()
{

	fps = 300.f;
	dt = 0.1f;

	m_ShadowCamera = new Camera();
	m_ShadowCamera->SetProjectionType(ProjectionType_Orthographic);
	m_ShadowCamera->SetPosition(glm::vec3(-16.13f, -5.9f, 20.2f));
	m_ShadowCamera->SetFrontVector(glm::vec3(-0.47f, -0.81f, 0.34f));
	m_ShadowCamera->SetOrthoBorders(64.f);

	glm::vec3 up = (m_ShadowCamera->GetFrontVector().z > 0.99f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 0, 1);
	m_ShadowCamera->SetRightVector(normalize(glm::cross(up, m_ShadowCamera->GetFrontVector())));
	m_ShadowCamera->SetUpVector(glm::cross(m_ShadowCamera->GetFrontVector(), m_ShadowCamera->GetRightVector()));
	m_ShadowCamera->translateTo(m_ShadowCamera->GetPosition() - m_ShadowCamera->GetFrontVector() * 10.f);
	m_ShadowCamera->UpdateVPMatrix();

	InitGL();

	InitFreetype();

	InitPipelines();
}

void Renderer::Shutdown()
{
	Serialize(true);
}

void Renderer::SetUniform(float value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform1f(prog, glGetUniformLocation(prog, name), value);
}

void Renderer::SetUniform(int value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform1i(prog, glGetUniformLocation(prog, name), value);
}

void Renderer::SetUniform(glm::vec3& value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform3f(prog, glGetUniformLocation(prog, name), value.x, value.y, value.z);
}

void Renderer::SetUniform(const glm::vec2& value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform2f(prog, glGetUniformLocation(prog, name), value.x, value.y);
}

void Renderer::SetUniform(const glm::ivec2& value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform2i(prog, glGetUniformLocation(prog, name), value.x, value.y);
}


void Renderer::SetUniform(glm::vec2& value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform2f(prog, glGetUniformLocation(prog, name), value.x, value.y);
}

void Renderer::SetUniform(const glm::vec3& value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform3f(prog, glGetUniformLocation(prog, name), value.x, value.y, value.z);
}

void Renderer::SetUniform(glm::vec4& value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform4f(prog, glGetUniformLocation(prog, name), value.x, value.y, value.z, value.w);
}

void Renderer::SetUniform(const glm::vec4& value, const char* name, GLuint location)
{
	GLuint prog = activePipeline->getShader(location)->glidShaderProgram;
	glProgramUniform4f(prog, glGetUniformLocation(prog, name), value.x, value.y, value.z, value.w);
}

void Renderer::SetUniform(glm::mat4x4& value, const char* name, GLuint location)
{
	GLuint uniformLocation = glGetUniformLocation(activePipeline->getShader(location)->glidShaderProgram, name);
	glProgramUniformMatrix4fv(activePipeline->getShader(location)->glidShaderProgram, uniformLocation, 1, false, value_ptr(value));
}
void Renderer::SetUniform(const glm::mat4x4& value, const char* name, GLuint location)
{
	glProgramUniformMatrix4fv(activePipeline->getShader(location)->glidShaderProgram, glGetUniformLocation(activePipeline->getShader(location)->glidShaderProgram, name), 1, false, value_ptr(value));
}

template<typename T>
void Renderer::SetVertexUniform(T& value, const char* name)
{
	SetUniform(value, name, GL_VERTEX_SHADER);
}

template<typename T>
void Renderer::SetFragmentUniform(T& value, const char* name)
{
	SetUniform(value, name, GL_FRAGMENT_SHADER);
}

template<typename T>
void Renderer::SetComputeUniform(const T& value, const char* name)
{
	SetUniform(value, name, GL_COMPUTE_SHADER);
}


void Renderer::SetVertexUniform(int value, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
	glProgramUniform1i(prog, glGetUniformLocation(prog, name), value);
}

void Renderer::SetFragmentUniform(int value, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	GLuint uniformLocation = glGetUniformLocation(prog, name);
	glProgramUniform1i(prog, uniformLocation, value);
}

void Renderer::SetFragmentUniform(float value, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	glProgramUniform1f(prog, glGetUniformLocation(prog, name), value);
}

void Renderer::SetFragmentUniform(bool value, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	glProgramUniform1i(prog, glGetUniformLocation(prog, name), (value ? 1 : 0));
}

void Renderer::SetFragmentUniform(const std::vector<int>& values, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	glProgramUniform1iv(prog, glGetUniformLocation(prog, name), (GLsizei)values.size(), values.data());
}

void Renderer::SetFragmentUniformArray(const std::vector<glm::vec3>& values, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	glProgramUniform3fv(prog, glGetUniformLocation(prog, name), (GLsizei)values.size() * 3, (const GLfloat*) values.data());
}

void Renderer::SetComputeUniform(int value, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_COMPUTE_SHADER)->glidShaderProgram;
	glProgramUniform1i(prog, glGetUniformLocation(prog, name), value);
}

//template<typename T>
//void Renderer::SetFragmentUniform(T value, const char* name)
//{
//	SetUniform(value, name, GL_FRAGMENT_SHADER);
//}
//
//void Renderer::SetVertexUniform(glm::mat4x4& value, const char* name)
//{
//	SetUniform(value, name, GL_VERTEX_SHADER);
//}
//
//void Renderer::SetVertexUniform(glm::vec3& value, const char* name)
//{
//	SetUniform(value, name, GL_VERTEX_SHADER);
//}


bool Renderer::SetPipeline(ProgramPipeline* pipeline, bool force)
{
	bool shaderValid = pipeline->m_IsValid;
	if (!shaderValid && !force)
	{
		return false;
	}

	lxAssert(pipeline != nullptr);
	if (force || activePipeline != pipeline)
	{
		activePipeline = pipeline;
		activePipeline->usePipeline();
	}

	return true;
}

bool Renderer::SetPipeline(ProgramPipeline* pipeline)
{
	return SetPipeline(pipeline, false);
}

bool Renderer::SetPipeline(ShaderFamily family)
{
	return SetPipeline(m_Pipelines[family]);
}

void Renderer::SetDisplayModeUniforms()
{
	SetFragmentUniform((int) m_DisplayOptions->GetDisplayMode(), "g_DisplayMode");
}

void Renderer::SetSkyUniforms(int skyCubemapSlot)
{
	if (m_World)
	{
		SunLight* sunLight = m_World->GetSunLight();

		SetFragmentUniform(skyCubemapSlot, "g_Skybox");
		BindCubemap(skyCubemapSlot, m_World->GetSunLight()->GetSkybox()->GetHWObject());

		SetFragmentUniform(m_World->GetSunLight()->GetUseSkybox(), "g_UseSkybox");
		SetFragmentUniform(sunLight->GetOrientation(), "sunOrientation");
		SetFragmentUniform(sunLight->GetTime(), "sunTime");
	}

}

void Renderer::SetUniformDesc(GLuint shader, GFXUniformDescription* uniformDesc, GLfloat* data)
{
	if (uniformDesc->GetLocation() == LX_LIMITS_UINT_MAX)
	{
		return;
	}

	LXType type = (LXType)uniformDesc->GetType();
	switch (type)
	{
		case LXType_bool:
		{
			int val = ((*data) ? 1 : 0);
			glProgramUniform1i(shader, (GLuint)uniformDesc->GetLocation(), val);
			break;
		}
		case LXType_glmmat4:
		{
			glProgramUniformMatrix4fv(shader, (GLuint)uniformDesc->GetLocation(), 1, false, data);
			break;
		}
		case LXType_glmvec3:
		{
			glProgramUniform3f(shader, (GLuint)uniformDesc->GetLocation(), *data, *(data + 1), *(data + 2));
			break;
		}
		case LXType_float:
		{
			glProgramUniform1f(shader, (GLuint)uniformDesc->GetLocation(), *data);
			break;
		}
		default:
		{
			lxAssert0();
		}
	}
}

void Renderer::SetUniformDescF(GFXUniformDescription* uniformDesc, GFXShaderStage stage, const char* name, GLfloat* data)
{
	GLenum glStage = (stage == GFXShaderStage_Vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	if (uniformDesc)
	{
		SetUniformDesc(activePipeline->getShader(glStage)->glidShaderProgram, uniformDesc, data);
	}
}

void Renderer::SetUniformDesc(GFXUniformGroup* uniformGroup, GFXShaderStage stage, const char* name, const bool& data)
{
	GFXUniformDescription* uniformDesc = uniformGroup->GetUniformDescription(stage, name);
	lxAssert(uniformDesc->GetType() == LXType_bool);

	GLfloat v = data ? 1 : 0;
	SetUniformDescF(uniformDesc, stage, name, &v);
}

void Renderer::SetUniformDesc(GFXUniformGroup* uniformGroup, GFXShaderStage stage, const char* name, const glm::vec3& data)
{
	GFXUniformDescription* uniformDesc = uniformGroup->GetUniformDescription(stage, name);
	lxAssert(uniformDesc->GetType() == LXType_glmvec3);
	SetUniformDescF(uniformDesc, stage, name, (GLfloat*)glm::value_ptr(data));
}

void Renderer::SetUniformDesc(GFXUniformGroup* uniformGroup, GFXShaderStage stage, const char* name, const glm::mat4& data)
{
	GFXUniformDescription* uniformDesc = uniformGroup->GetUniformDescription(stage, name);
	lxAssert(uniformDesc->GetType() == LXType_glmmat4);
	SetUniformDescF(uniformDesc, stage, name, (GLfloat*)glm::value_ptr(data));
}

void Renderer::SetUniformDesc(GFXUniformGroup* uniformGroup, GFXShaderStage stage, const char* name, const float& data)
{
	GFXUniformDescription* uniformDesc = uniformGroup->GetUniformDescription(stage, name);
	lxAssert(uniformDesc->GetType() == LXType_float);
	SetUniformDescF(uniformDesc, stage, name, (GLfloat*)&data);
}


void Renderer::SetLightingOptionsUniforms()
{
	GFXUniformGroup* group = activePipeline->GetUniformGroup("LightingOptions");
	if (group)
	{
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableDynamicLights", m_LightingOptions->GetEnableDynamicLights());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableSunlight", m_LightingOptions->GetEnableSunlight());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableAmbientLighting", m_LightingOptions->GetEnableAmbientLighting());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableDiffuseComponent", m_LightingOptions->GetEnableDiffuseComponent());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableSpecularComponent", m_LightingOptions->GetEnableSpecularComponent());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableReflection", m_LightingOptions->GetEnableReflection());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableSunShadow", m_LightingOptions->GetEnableSunShadow());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableDynamicShadows", m_LightingOptions->GetEnableDynamicShadows());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableReplacementAlbedo", m_LightingOptions->GetEnableReplacementAlbedo());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_ReplacementAlbedo", m_LightingOptions->GetReplacementAlbedo());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableReplacementMetallic", m_LightingOptions->GetEnableReplacementMetallic());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_ReplacementMetallic", m_LightingOptions->GetReplacementMetallic());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_EnableReplacementRoughness", m_LightingOptions->GetEnableReplacementRoughness());
		SetUniformDesc(group, GFXShaderStage_Fragment, "g_ReplacementRoughness", m_LightingOptions->GetReplacementRoughness());
	}
	else
	{
		SetFragmentUniform(m_LightingOptions->GetEnableDynamicLights(), "g_EnableDynamicLights");
		SetFragmentUniform(m_LightingOptions->GetEnableSunlight(), "g_EnableSunlight");
		SetFragmentUniform(m_LightingOptions->GetEnableAmbientLighting(), "g_EnableAmbientLighting");
		SetFragmentUniform(m_LightingOptions->GetEnableDiffuseComponent(), "g_EnableDiffuseComponent");
		SetFragmentUniform(m_LightingOptions->GetEnableSpecularComponent(), "g_EnableSpecularComponent");
		SetFragmentUniform(m_LightingOptions->GetEnableReflection(), "g_EnableReflection");
		SetFragmentUniform(m_LightingOptions->GetEnableSunShadow(), "g_EnableSunShadow");
		SetFragmentUniform(m_LightingOptions->GetEnableDynamicShadows(), "g_EnableDynamicShadows");
		SetFragmentUniform(m_LightingOptions->GetEnableReplacementAlbedo(), "g_EnableReplacementAlbedo");
		SetFragmentUniform(m_LightingOptions->GetReplacementAlbedo(), "g_ReplacementAlbedo");
		SetFragmentUniform(m_LightingOptions->GetEnableReplacementMetallic(), "g_EnableReplacementMetallic");
		SetFragmentUniform(m_LightingOptions->GetReplacementMetallic(), "g_ReplacementMetallic");
		SetFragmentUniform(m_LightingOptions->GetEnableReplacementRoughness(), "g_EnableReplacementRoughness");
		SetFragmentUniform(m_LightingOptions->GetReplacementRoughness(), "g_ReplacementRoughness");
	}

}
void Renderer::SetLightingUniforms()
{

	SetFragmentUniform(m_SkyLight.m_Position, "g_DirectionalLight.m_Direction");
	SetFragmentUniform(m_SkyLight.m_DiffuseColor, "g_DirectionalLight.m_DiffuseColor");
	SetFragmentUniform(m_SkyLight.m_AmbientColor, "g_DirectionalLight.m_AmbientColor");
	SetFragmentUniform(m_SkyLight.m_SpecularColor, "g_DirectionalLight.m_SpecularColor");

	for (int i = 0; i < m_NumLights; ++i)
	{
		std::string index = std::to_string(i);
		SetFragmentUniform(m_TestLight[i].m_Position,		("g_PointLight[" + index + "].m_Position").c_str()	);
		SetFragmentUniform(m_TestLight[i].m_DiffuseColor,	("g_PointLight[" + index + "].m_DiffuseColor").c_str());
		SetFragmentUniform(m_TestLight[i].m_AmbientColor,	("g_PointLight[" + index + "].m_AmbientColor").c_str());
		SetFragmentUniform(m_TestLight[i].m_SpecularColor,	("g_PointLight[" + index + "].m_SpecularColor").c_str());
	}

	SetFragmentUniform(m_NumLights, "g_NumLights");


}

void Renderer::SetMaterialUniforms(Material* material)
{
	switch (material->GetShaderFamily())
	{
		
		case ShaderFamily_SolidColor:
		{
			SetFragmentUniform(material->GetDiffuseColor(), "g_Material.m_DiffuseColor");
			break;
		}
		case ShaderFamily_Basic:
		{
			SetFragmentUniform(material->GetAmbientColor(), "g_Material.m_AmbientColor");
			SetFragmentUniform(material->GetDiffuseColor(), "g_Material.m_DiffuseColor");
			SetFragmentUniform(material->GetSpecularColor(), "g_Material.m_SpecularColor");
			SetFragmentUniform(material->GetDiffuseTextureEnabled(), "g_Material.m_DiffuseTextureEnabled");
			SetFragmentUniform(material->GetSpecularTextureEnabled(), "g_Material.m_SpecularTextureEnabled");
			SetFragmentUniform(material->GetShininess(), "g_Material.m_Shininess");
			SetFragmentUniform(material->GetUnlit(), "g_Material.m_Unlit");
			SetFragmentUniform(material->GetEmissiveFactor(), "g_Material.m_EmissiveFactor");
			SetFragmentUniform(material->GetIsGlass(), "g_Material.m_IsGlass");
			SetFragmentUniform(material->GetRefractionIndex(), "g_Material.m_RefractionIndex");
			SetFragmentUniform(material->GetReflectEnvironment(), "g_Material.m_ReflectEnvironment");
			SetFragmentUniform(material->GetMetallic(), "g_Material.m_Metallic");
			SetFragmentUniform(material->GetRoughness(), "g_Material.m_Roughness");
			SetFragmentUniform(material->GetAO(), "g_Material.m_AO");
			SetFragmentUniform(material->GetIsPBR(), "g_Material.m_IsPBR");
			SetFragmentUniform(material->GetUVScale(), "g_Material.m_UVScale");

			if (material->GetDiffuseTexture())
			{
				SetFragmentUniform(0, "g_Material.m_DiffuseTexture");
				Bind2DTexture(0, material->GetDiffuseTexture()->GetHWObject());
			}

			if (material->GetSpecularTexture())
			{
				SetFragmentUniform(1, "g_Material.m_SpecularTexture");
				Bind2DTexture(1, material->GetSpecularTexture()->GetHWObject());
			}

			int blinnPhongshading = m_DisplayOptions->GetBlinnPhongShading() ? 1 : 0;
			SetFragmentUniform(blinnPhongshading, "g_BlinnPhongShading");

			break;
		}
		case ShaderFamily_Terrain:
		{
			//SetVertexUniform(0, "heightfieldTexture");
			//Bind2DTexture(0, material->GetHeightfieldTexture()->GetHWObject());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->GetHeightfieldTexture()->GetHWObject());

			//SetFragmentUniform(0, "heightfieldTexture");
			//Bind2DTexture(0, material->GetHeightfieldTexture()->GetHWObject());
		}
		default:
		{
			break;
		}
	}

}

void Renderer::SetPostEffectsUniforms()
{
	int gamma = m_PostEffects->GetGammaCorrectionEnabled() ? 1 : 0;
	SetFragmentUniform(gamma, "g_GammaCorrectionEnabled");
	float val = m_PostEffects->GetGammaExponent();
	SetFragmentUniform(val, "g_GammaCorrectionExponent");

	SetFragmentUniform(m_PostEffects->GetToneMappingEnabled(), "g_ToneMappingEnabled");
}


void Renderer::SetShadowMapUniforms(Camera* cam)
{
	SetFragmentUniform(2, "g_DepthMapTexture");
	Bind2DTexture(2, m_Framebuffers[FramebufferType_ShadowMap]->GetDepthTexture());

	GFXUniformGroup* uniformGroup = activePipeline->GetUniformGroup("ShadowMap");
	SetUniformDesc(uniformGroup, GFXShaderStage_Vertex, "g_LightProjectionMatrix", cam->GetViewProjectionMatrix());

}

void Renderer::SetWorldGridUniforms()
{
	SetFragmentUniform(g_EngineSettings->GetWorldScale(), "g_WorldScale");

	SectorTool* sectorTool = (SectorTool*) g_Editor->GetTools()[EEditorTool_SectorTool];
	SetFragmentUniform(sectorTool->GetSectorGridColor(), "g_SectorGridColor");
}

void Renderer::SetViewUniforms(Camera* cam)
{
	GFXUniformGroup* group = activePipeline->GetUniformGroup("View");

	SetUniformDesc(group, GFXShaderStage_Vertex, "g_WorldToViewMatrix", cam->GetViewMatrix());
	SetUniformDesc(group, GFXShaderStage_Vertex, "g_WorldToViewMatrixNoTranslation", cam->GetViewMatrixNoTranslation());
	SetUniformDesc(group, GFXShaderStage_Vertex, "g_WorldToViewMatrixRotationOnly", glm::mat4(glm::mat3(cam->GetViewMatrix())));
	SetUniformDesc(group, GFXShaderStage_Vertex, "g_ProjectionMatrix", cam->GetProjectionMatrix());

	SetUniformDesc(group, GFXShaderStage_Fragment, "g_CameraPosition", cam->GetPosition());
	SetUniformDesc(group, GFXShaderStage_Fragment, "g_ViewToWorldMatrix", cam->GetViewMatrixInverse());
	SetUniformDesc(group, GFXShaderStage_Fragment, "g_ViewProjectionMatrixInverse", cam->GetViewProjectionMatrixInverse());
	SetUniformDesc(group, GFXShaderStage_Fragment, "g_ViewMatrixInverse", cam->GetViewMatrixInverse());
	SetUniformDesc(group, GFXShaderStage_Fragment, "g_ProjectionMatrixInverse", cam->GetProjectionMatrixInverse());
	SetUniformDesc(group, GFXShaderStage_Fragment, "g_CameraNearPlane", cam->GetNearPlane());
	SetUniformDesc(group, GFXShaderStage_Fragment, "g_CameraFarPlane", cam->GetFarPlane());
	SetUniformDesc(group, GFXShaderStage_Fragment, "g_CameraLookAt", cam->GetFrontVector());
}

void Renderer::SetDebugUniforms()
{
	int useLighting =		m_DisplayOptions->GetUseLighting() ? 1 : 0;
	int diffuseEnabled =	m_DisplayOptions->GetShowDiffuse() ? 1 : 0;
	int specularEnabled =	m_DisplayOptions->GetShowSpecular() ? 1 : 0;
	int ambientEnabled =	m_DisplayOptions->GetShowAmbient() ? 1 : 0;
	int normalsEnabled =	m_DisplayOptions->GetShowNormals() ? 1 : 0;
	int depthEnabled =		m_DisplayOptions->GetShowDepth() ? 1 : 0;
	int linearizeDepth =	m_DisplayOptions->GetLinearizeDepth() ? 1 : 0;
	//int gammaCorrection =	m_DisplayOptions->GetGammaCorrection() ? 1 : 0;

	SetFragmentUniform(useLighting,		"g_UseLighting");
	SetFragmentUniform(diffuseEnabled,	"g_DebugDiffuseEnabled");
	SetFragmentUniform(ambientEnabled,	"g_DebugAmbientEnabled");
	SetFragmentUniform(specularEnabled, "g_DebugSpecularEnabled");
	SetFragmentUniform(depthEnabled,	"g_DebugDepthEnabled");
	SetFragmentUniform(normalsEnabled, "g_DebugNormalsEnabled");
	SetFragmentUniform(m_DisplayOptions->GetShowUVs(),	"g_DebugUVsEnabled");
	SetFragmentUniform(linearizeDepth, "g_DebugLinearizeDepth");
	//SetFragmentUniform(gammaCorrection, "g_GammaCorrection");

}

void Renderer::DrawModel(Entity* entity, Model* model)
{
	for (int i = 0; i < model->GetMeshes().size(); ++i)
	{
		Material* material = nullptr;
		if (model->GetMaterials().size() > i)
		{
			material = model->GetMaterials()[i];
		}
		else if (model->GetMaterials().size() > 0)
		{
			material = model->GetMaterials()[0];
		}
		else
		{
			lxAssert0();
		}
		if (!material->GetEnabled())
		{
			continue;
		}

		if (!SetPipeline(material->GetShaderFamily()))
		{
			continue;
		}


		SetFragmentUniform(m_DisplayOptions->GetDebugVec4(), "g_DebugVec4");
		SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

		//if (!m_ShaderBeenUsedThisFrame[material->GetShaderFamily()])
		{
			SetLightingUniforms();
			SetLightingOptionsUniforms();

			SetViewUniforms(m_ActiveCamera);
			SetShadowMapUniforms(m_ShadowCamera);
			SetSkyUniforms(3);
			SetDisplayModeUniforms();

			SetPostEffectsUniforms();
			SetDebugUniforms();
			m_ShaderBeenUsedThisFrame[material->GetShaderFamily()] = true;
		}

		DrawMesh(model->GetMeshes()[i], material);

	}

	GL::OutputErrors();
}

GLuint slots[] =
{
	GL_TEXTURE0,
	GL_TEXTURE1,
	GL_TEXTURE2,
	GL_TEXTURE3,
	GL_TEXTURE4,
	GL_TEXTURE5,
	GL_TEXTURE6,
	GL_TEXTURE7,
	GL_TEXTURE8
};

void Renderer::Bind2DTexture(int slot, GLuint HWObject)
{
	GLuint theSlot = slots[slot];
	glActiveTexture(theSlot);
	glBindTexture(GL_TEXTURE_2D, HWObject);
}

void Renderer::Bind2DTexture(int slot, Texture* texture)
{
	Bind2DTexture(slot, texture->GetHWObject());
}

void Renderer::BindCubemap(int slot, GLuint HWObject)
{
	GLuint theSlot = slots[slot];
	glActiveTexture(theSlot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, HWObject);
}


void Renderer::FreeBoundTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::FreeBoundTexture(int slot)
{
	glActiveTexture(slot);
	FreeBoundTexture();
	glActiveTexture(0);
}

void Renderer::DrawMesh(Mesh* mesh)
{
	// todo jpp probably kills perf


	glBindVertexArray(mesh->m_VAO);

	if (mesh->GetUsesIndexBuffer())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetGPUBuffers().glidIndexBuffer);

		GL::DrawElements(mesh->m_PrimitiveMode, (int) mesh->m_buffers.GetIndexBuffer().size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(mesh->m_PrimitiveMode, 0, (int) mesh->m_buffers.GetVertexPositions().size());
		GL::OutputErrors();
	}

	g_EngineStats->AddTo_NumDrawCalls(1);
}


void Renderer::DrawMesh(Mesh* mesh, Material* material)
{
  glBindVertexArray(mesh->m_VAO);

  SetMaterialUniforms(material);

  //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, DATAINSPECTOR_BINDPOS, m_DataInspectorSSBO);
  //SetFragmentUniform((int)(g_InputHandler->GetMousePosition().x), "g_MouseX");
  //SetFragmentUniform((int)(g_InputHandler->GetMousePosition().y), "g_MouseY");

  bool wireframe = mesh->GetWireframeRendering() || m_DisplayOptions->GetWireframeRendering();
  if (wireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (mesh->GetPointRendering())
  {
    glEnable(GL_PROGRAM_POINT_SIZE);
  }

  DrawMesh(mesh);

  if (mesh->GetPointRendering())
  {
    glDisable(GL_PROGRAM_POINT_SIZE);
  }

  if (wireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

}

void Renderer::RenderTerrain()
{
	lxGPUProfile(RenderTerrain);

	if (!m_DisplayOptions->GetRenderTerrain())
	{
		return;
	}


	if (!SetPipeline(m_Pipelines[ShaderFamily_Terrain]))
	{
		return;
	}

	SetLightingUniforms();
	SetLightingOptionsUniforms();

	SetViewUniforms(m_ActiveCamera);
	SetShadowMapUniforms(m_ShadowCamera);
	SetSkyUniforms(3);
	SetDisplayModeUniforms();

	SetPostEffectsUniforms();
	SetDebugUniforms();

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	SetFragmentUniform(4, "g_DirtTexture");
	Bind2DTexture(4, g_Editor->GetDefaultTextureHolder()->GetDirtTexture());

	SetFragmentUniform(5, "g_SandTexture");
	Bind2DTexture(5, g_Editor->GetDefaultTextureHolder()->GetSandTexture());

	SetFragmentUniform(6, "g_WoodTexture");
	Bind2DTexture(6, g_Editor->GetDefaultTextureHolder()->GetWoodTexture());



	for (int i = 0; i < g_RenderDataManager->GetVisibleSectors().size(); ++i)
	{
		Sector* sector = g_RenderDataManager->GetVisibleSectors()[i];

		if (sector)
		{
			Entity* entity = sector->GetTerrainPatchEntity();

			if (entity)
			{
				SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

				Visual* visual = entity->GetComponent<Visual>();
				if (visual)
				{
					Material* terrainMaterial = visual->GetModel()->GetMaterials()[0];

					SetFragmentUniform(7, "g_AlbedoTexture");
					Bind2DTexture(7, sector->GetGraphicalData()->GetAlbedoTexture()->GetHWObject());

					SetFragmentUniform(1, "g_SplatMapTexture");
					Bind2DTexture(1, sector->GetSplatMapTexture()->GetHWObject());

					SetFragmentUniform(3, "g_HeightfieldTexture");
					Bind2DTexture(3, terrainMaterial->GetHeightfieldTexture()->GetHWObject());

					DrawMesh(g_DefaultObjects->DefaultTerrainMesh, terrainMaterial);
				}

			}
		}
	}



}

void Renderer::RenderShadowMap()
{
	lxGPUProfile(RenderShadowMap);

	if (!m_DisplayOptions->GetRenderShadows() || !m_World)
	{
		return;
	}

	GL::SetViewport(SHADOW_WIDTH, SHADOW_HEIGHT);
	
	BindFramebuffer(FramebufferType_ShadowMap);
	
	GL::ClearDepthBuffer();
	GL::SetDepthFunction(GL::Depth_Less);
	GL::SetCapability(GL::CullFace, true);

	if (!SetPipeline(pPipelineShadowMap))
	{
		return;
	}

	glm::vec3 pos = glm::normalize(m_World->GetSunLight()->GetSunDirection());

	m_SkyLight.m_Position = pos;
	SetLightingUniforms();

	m_ShadowCamera->SetPosition(glm::vec3(0, 20, 1) + pos * 100.f);
	m_ShadowCamera->SetFrontVector(pos);

	glm::vec3 up = (m_ShadowCamera->GetFrontVector().z > 0.99f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 0, 1);
	m_ShadowCamera->SetRightVector(normalize(glm::cross(up, m_ShadowCamera->GetFrontVector())));
	m_ShadowCamera->SetUpVector(glm::cross(m_ShadowCamera->GetFrontVector(), m_ShadowCamera->GetRightVector()));

	m_ShadowCamera->UpdateVPMatrix();

	SetViewUniforms(m_ShadowCamera);
	SetPostEffectsUniforms();
	SetDebugUniforms();

	for (Entity* entity : g_RenderDataManager->GetVisibleEntities())
	{
		if (!entity->GetVisible())
		{
			continue;
		}

		SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

		Visual* visual = entity->GetComponent<Visual>();
		if (visual)
		{
			Model* model = visual->GetModel();
			for (int i = 0; i < model->GetMeshes().size(); ++i)
			{
				glBindVertexArray(model->GetMeshes()[i]->m_VAO);
				DrawMesh(model->GetMeshes()[i]);
			}

			GL::OutputErrors();
		}

	}


	BindFramebuffer(FramebufferType_Default);
}

void Renderer::RenderOpaque()
{
	lxGPUProfile(RenderOpaque);

	
	GL::SetDepthFunction(GL::Depth_Less);
	GL::SetCapability(GL::CullFace, false);

	GL::GPUMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (m_DisplayOptions->GetRenderOpaque())
	{
		RenderEntities(ShaderFamily_Basic, g_RenderDataManager->GetVisibleEntities());

		if (g_Editor->GetOptions()->GetDebugDisplay() && m_World)
		{
			RenderEntities(ShaderFamily_Basic, m_World->GetDebugEntities());
		}
	}

	if (m_DisplayOptions->GetShowDebugIcons() && m_World)
	{
		SetPipeline(ShaderFamily_Basic);

		for (Entity* entity : m_World->GetEntities()) // todo jpp : only do visible but hard for now with separate model loading etc.
		{
			glm::mat4 toWorld = entity->GetModelToWorldMatrix();
			glm::vec3 invScale = 1.f / entity->GetScale();
			toWorld = glm::scale(toWorld, invScale);
			SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
			SetVertexUniform(toWorld, "g_ModelToWorldMatrix");
			DrawMesh(g_DefaultObjects->DefaultCubeMesh);
		}
	}

}

void Renderer::RenderTextureOverlay()
{
	if (!m_DisplayOptions->GetRenderTextureOverlay())
	{
		return;
	}

	GL::SetViewport(300, 300);

	if (!SetPipeline(pPipelineScreenSpaceTexture))
	{
		return;
	}

	SetPostEffectsUniforms();

	SetFragmentUniform(0, "g_MainTexture");
	Bind2DTexture(0, m_Framebuffers[FramebufferType_Picking]->GetColorTexture(0));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Renderer::RenderHDRFramebuffer()
{
	lxGPUProfile(RenderHDRFramebuffer);

	BindFramebuffer(FramebufferType_Default);
	GL::SetViewport(m_MainWindow->GetSize());
	GL::ClearColorAndDepthBuffers();

	if (!SetPipeline(pPipelineScreenSpaceTexture))
	{
		return;
	}

	SetPostEffectsUniforms();

	SetFragmentUniform(0, "g_MainTexture");
	Bind2DTexture(0, m_Framebuffers[m_ColorFramebuffer]->GetColorTexture(0));

	SetFragmentUniform(1, "g_GlowTexture");
	Bind2DTexture(1, m_Framebuffers[FramebufferType_PingPong1]->GetColorTexture(0));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::GetPickingData(glm::vec2 mouseClickPosition, glm::vec4& pickingData)
{
	if (!SetPipeline(pPipelinePickingCompute))
	{
		return;
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_PickingSSBO);

	SetComputeUniform(1, "g_PickingBuffer");
	Bind2DTexture(1, m_Framebuffers[FramebufferType_Picking]->GetColorTexture(0));

	SetComputeUniform(2, "g_DepthBuffer");
	Bind2DTexture(2, m_Framebuffers[FramebufferType_Picking]->GetDepthTexture());


	SetComputeUniform(glm::vec2(m_MainWindow->GetSize()), "g_WindowSize");
	SetComputeUniform(mouseClickPosition, "g_MouseClickPosition");

	glDispatchCompute(1, 1, 1);
	GL::GPUMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_PickingSSBO);

	GLfloat *ptr;
	ptr = (GLfloat *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	for (int i = 0; i < 4; ++i)
	{
		pickingData[i] = *ptr++;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


}

void Renderer::RenderPickingBuffer(bool debugEntities)
{
	lxGPUProfile(RenderPickingBuffer);

	int vpWidth = m_Framebuffers[FramebufferType_Picking]->GetWidth();
	int vpHeight = m_Framebuffers[FramebufferType_Picking]->GetHeight();
	GL::SetViewport(vpWidth, vpHeight);
	
	if (!SetPipeline(pPipelinePicking))
	{
		return;
	}
	
	BindFramebuffer(FramebufferType_Picking);
	glClearColor(0, 0, 0, 0);
	GL::ClearColorAndDepthBuffers();
	
	SetViewUniforms(m_ActiveCamera);


	SetVertexUniform(1, "g_UseHeightfield");

	for (Sector* sector : g_RenderDataManager->GetVisibleSectors())
	{
		Entity* entity = sector->GetTerrainPatchEntity();
		SetFragmentUniform(entity->GetPickingID(), "g_PickingID");

		SetVertexUniform(3, "g_HeightfieldTexture");
		Bind2DTexture(3, sector->GetHeightfield()->GetHeightDataTexture()->GetHWObject());
		
		Visual* visual = entity->GetComponent<Visual>();
		if (visual && visual->GetModel())
		{
			for (int i = 0; i < visual->GetModel()->GetMeshes().size(); ++i)
			{
				DrawMesh(visual->GetModel()->GetMeshes()[i]);
			}
		}
	}
	
	SetVertexUniform(0, "g_UseHeightfield");

	for (Entity* entity : g_RenderDataManager->GetVisibleEntities())
	{
		SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
		SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");
	
		Visual* visual = entity->GetComponent<Visual>();
		if (visual)
		{
			for (int i = 0; i < visual->GetModel()->GetMeshes().size(); ++i)
			{
				DrawMesh(visual->GetModel()->GetMeshes()[i]);
			}
		}

	}
	
	if (debugEntities)
	{
		for (Entity* entity : m_World->GetDebugEntities())
		{
			SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
			SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

			Visual* visual = entity->GetComponent<Visual>();
			if (visual)
			{
				for (int i = 0; i < visual->GetModel()->GetMeshes().size(); ++i)
				{
					DrawMesh(visual->GetModel()->GetMeshes()[i]);
				}
			}
		}
	}

	if (m_DisplayOptions->GetShowDebugIcons() && m_World)
	{
		for (Entity* entity : m_World->GetEntities()) // todo jpp : only do visible but hard for now with separate model loading etc.
		{
			glm::mat4 toWorld = entity->GetModelToWorldMatrix();
			glm::vec3 invScale = 1.f / entity->GetScale();
			toWorld = glm::scale(toWorld, invScale);
			SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
			SetVertexUniform(toWorld, "g_ModelToWorldMatrix");
			DrawMesh(g_DefaultObjects->DefaultCubeMesh);
		}
	}

	BindFramebuffer(FramebufferType_Default);
}

void Renderer::BeginFrame(World* world)
{
	m_ActiveCamera = m_DebugCamera;
	if (m_DisplayOptions->GetDebugShadowCamera())
	{
		m_ActiveCamera = m_ShadowCamera;
	}
	m_ActiveCamera->handlePresetNewFrame(m_MainWindow->pWindow);
	m_ActiveCamera->UpdateVPMatrix();

	g_EngineStats->ResetFrame();

	m_World = world;
	GL::g_CheckGLErrors = m_DisplayOptions->GetOutputGLErrors();

	m_NumLights = 0;

	if (m_World)
	{
		for (Entity* entity : m_World->GetEntities())
		{

			if (entity->GetIsLight())
			{
				glm::vec3 diffuseColor = glm::vec3(1, 1, 1);
				Visual* visual = entity->GetComponent<Visual>();

				if (visual && visual->GetModel())
				{
					diffuseColor = visual->GetModel()->GetMaterials()[0]->GetDiffuseColor();
				}

				m_TestLight[m_NumLights].m_Position = entity->GetPosition();
				m_TestLight[m_NumLights].m_DiffuseColor = diffuseColor;
				m_TestLight[m_NumLights].m_SpecularColor = m_TestLight[m_NumLights].m_DiffuseColor;
				m_TestLight[m_NumLights].m_AmbientColor = m_TestLight[m_NumLights].m_DiffuseColor;

				m_NumLights++;
			}
		}
	}

	for (int i = 0; i < EnumLength_ShaderFamily; ++i)
	{
		m_ShaderBeenUsedThisFrame[i] = false;;
	}

	g_Editor->ApplyTool();

	m_ColorFramebuffer = m_DisplayOptions->GetDeferredRendering() ? FramebufferType_GBuffer : FramebufferType_MainColorBuffer;
}

void Renderer::ApplyEmissiveGlow()
{
	lxGPUProfile(ApplyEmissiveGlow);

	if (!m_PostEffects->GetEmissiveGlowEnabled())
	{
		return;
	}

	if (!SetPipeline(pPipelineBlur))
	{
		return;
	}


	bool horizontal = true;
	unsigned int amount = 10;
	for (unsigned int i = 0; i < amount; i++)
	{
		FramebufferType fb = horizontal ? FramebufferType_PingPong0 : FramebufferType_PingPong1;
		FramebufferType fb2 = fb == FramebufferType_PingPong0 ? FramebufferType_PingPong1 : FramebufferType_PingPong0;

		BindFramebuffer(fb);

		SetFragmentUniform(horizontal, "horizontal");

		GLuint sourceTexture = (i == 0 ? m_Framebuffers[m_ColorFramebuffer]->GetColorTexture(1) : m_Framebuffers[fb2]->GetColorTexture(0));
		Bind2DTexture(0, sourceTexture);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		horizontal = !horizontal;
	}

	BindFramebuffer(FramebufferType_Default);
}

void Renderer::BeforeWorldRender()
{
	lxGPUProfile(BeforeWorldRender);

	BindFramebuffer(m_ColorFramebuffer);

	GL::SetViewport(m_MainWindow->GetSize());

	const glm::vec3& clearColor = m_DisplayOptions->GetClearColor();
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
	GL::ClearColorAndDepthBuffers();

	GL::OutputErrors();
}


void Renderer::RenderGrid()
{
	lxGPUProfile(RenderGrid);

	GL::DepthWriteEnabled(false);

	if (!SetPipeline(pPipelineGrid))
	{
		return;
	}

	GL::SetCapability(GL::Capabilities::Blend, true);
	GL::SetCapability(GL::Capabilities::DepthTest, false);
	GL::SetBlendMode(GL::SrcAlpha, GL::BlendMode::OneMinusSrcAlpha);

	SetWorldGridUniforms();
	SetViewUniforms(m_ActiveCamera);
	SetFragmentUniform(glm::vec2(m_MainWindow->GetSize()), "g_WindowSize");

	GL::GPUMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);

	SetFragmentUniform(0, "g_DepthTexture");
	Bind2DTexture(0, m_Framebuffers[FramebufferType_MainColorBuffer]->GetDepthTexture());

	Mesh* mesh = g_DefaultObjects->DefaultQuadMesh;
	DrawMesh(mesh);

	GL::DepthWriteEnabled(true);

	GL::SetCapability(GL::Capabilities::Blend, false);
	GL::SetCapability(GL::Capabilities::DepthTest, true);

}

void Renderer::RenderAxisGizmo()
{
	lxGPUProfile(RenderAxisGizmo);

	if (!SetPipeline(pPipelineAxisGizmo))
	{
		return;
	}

	SetViewUniforms(m_ActiveCamera);


	GL::DrawArrays(GL::PrimitiveMode::Lines, 0, 6);
}


void Renderer::RenderDebugModel(Model* model, const glm::mat4& modelToWorld, ProgramPipeline* programPipeline)
{
	if (!SetPipeline(programPipeline))
	{
		return;
	}

	for (int i = 0; i < model->GetMeshes().size(); ++i)
	{
		SetVertexUniform(modelToWorld, "g_ModelToWorldMatrix");

		Material* material = model->GetMaterials()[i];

		SetViewUniforms(m_ActiveCamera);

		DrawMesh(model->GetMeshes()[i], material);
	}
}

void Renderer::RenderDebugWays(const glm::mat4& modelToWorld, ProgramPipeline* programPipeline, const std::vector<int>& displayFlags, const std::vector<glm::vec3>& wayDebugColors, Entity* selectedEntity)
{
	lxGPUProfile(RenderDebugWays);

	if (!SetPipeline(programPipeline))
	{
		return;
	}

	GL::SetCapability(GL::Capabilities::Blend, true);

	// todo jpp : wrap this in some sort of debug display
	for (int e = 0; e < m_World->GetEntities().size(); ++e)
	{
		Entity* entity = m_World->GetEntities()[e];

		if (entity)
		{
			bool isSelected = (entity == selectedEntity);

			OSMElementComponent* osmElementComponent = entity->GetComponent<OSMElementComponent>();
			if (osmElementComponent)
			{
				Model* wayModel = osmElementComponent->GetDebugWayModel();

				if (wayModel)
				{
					for (int i = 0; i < wayModel->GetMeshes().size(); ++i)
					{
						SetVertexUniform(modelToWorld, "g_ModelToWorldMatrix");

						Material* material = wayModel->GetMaterials()[i];

						SetViewUniforms(m_ActiveCamera);

						SetFragmentUniform(isSelected, "g_IsSelected");
						SetFragmentUniform(displayFlags, "g_DisplayFlags");
						SetFragmentUniformArray(wayDebugColors, "g_WayDebugColors");

						DrawMesh(wayModel->GetMeshes()[i]);
					}
				}
			}
		}
	}



	GL::SetCapability(GL::Capabilities::Blend, false);
}

void Renderer::RenderDebugModels()
{
	lxGPUProfile(RenderDebugModels);

	g_Editor->RenderTools();

	if (m_DisplayOptions->GetDisplayDebugModels())
	{
		SetPipeline(pPipelineBasic);
		for (Model* model : m_DebugModels)
		{
			// TODO : right now this is for buildings but they should create their own entity
			for (int i = 0; i < model->GetMeshes().size(); ++i)
			{
				SetVertexUniform(glm::mat4(1.0), "g_ModelToWorldMatrix");

				Material* material = model->GetMaterials()[i];

				SetViewUniforms(m_ActiveCamera);

				if (!material->GetEnabled())
				{
					continue;
				}

				DrawMesh(model->GetMeshes()[i], material);
			}

		}

		for (AABBJob& aabb : m_RenderDataManager->GetAABBJobs())
		{
			RenderAABB(aabb.m_AABB, aabb.m_Color);
		}

		for (Entity* entity : m_RenderDataManager->GetEntityAABBJobs())
		{
			RenderEntityBB(entity);
		}
	}


	m_RenderDataManager->ClearAABBJobs();
}


void Renderer::AfterWorldRender()
{
	lxGPUProfile(AfterWorldRender);

	RenderDebugModels();

	ApplyEmissiveGlow();
}

void Renderer::DataInspector_FinishFrame()
{
	GL::GPUMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, DATAINSPECTOR_BINDPOS, m_DataInspectorSSBO);

	GLfloat *ptr;
	ptr = (GLfloat *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	for (int i = 0; i < DATAINSPECTOR_NUMVALUES; ++i)
	{
		m_DataInspectorValues[i] = *ptr++;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, DATAINSPECTOR_BINDPOS, 0);
}

void Renderer::FinishFrame()
{
	lxGPUProfile(FinishFrame);

	DataInspector_FinishFrame();

	g_RenderDataManager->Update();

	HandleScreenshot();
	
	glfwSwapBuffers(m_MainWindow->GetHWObject());
}

void Renderer::RenderAABB(AABB& aabb)
{
	glm::vec3 color(1, 1, 1);
	RenderAABB(aabb, color);
}

void Renderer::RenderAABB(AABB& aabb, const glm::vec3& color)
{
	GL::SetCapability(GL::Capabilities::Blend, true);
	GL::SetCapability(GL::Capabilities::DepthTest, true);
	glDepthMask(GL_FALSE);

	if (!SetPipeline(pPipelineUVEdges))
	{
		return;
	}

	glm::mat4 modelToWorldMatrix = mat4(1.0f);

	glm::vec3 translation = aabb.GetStartPoint();
	modelToWorldMatrix = glm::translate(modelToWorldMatrix, translation);
	modelToWorldMatrix = glm::scale(modelToWorldMatrix, aabb.GetScale());

	SetVertexUniform(modelToWorldMatrix, "g_ModelToWorldMatrix");
	SetFragmentUniform(color, "g_Color");

	SetViewUniforms(m_ActiveCamera);

	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	GL::SetCapability(GL::Capabilities::Blend, false);
	glDepthMask(true);
}

void Renderer::DrawBoundingBox(BoundingBoxComponent* bb)
{
	GL::SetCapability(GL::Blend, true);
	GL::SetBlendMode(GL::SrcAlpha, GL::OneMinusSrcAlpha);
	GL::SetBlendMode(GL::SrcAlpha, GL::OneMinusSrcAlpha);

	if (!SetPipeline(pPipelineUVEdges))
	{
		return;
	}

	SetViewUniforms(m_ActiveCamera);
	//SetVertexUniform(bb->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

	if (bb->GetParentEntity())
	{
		SetVertexUniform(bb->GetParentEntity()->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

		Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
		DrawMesh(mesh);
	}

	GL::SetCapability(GL::Blend, false);
}

void Renderer::DrawManipulator(Entity* entity)
{
	GL::SetCapability(GL::Blend, true);
	GL::SetBlendMode(GL::SrcAlpha, GL::OneMinusSrcAlpha);

	if (!SetPipeline(pPipelineSolidColor))
	{
		return;
	}

	SetViewUniforms(m_ActiveCamera);
	SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");
	SetFragmentUniform(glm::vec3(1,0,0), "g_InputColor");


	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	GL::SetCapability(GL::Blend, false);
}


void Renderer::RenderEntityBB(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	BoundingBoxComponent* bb = (BoundingBoxComponent*)entity->GetComponent<BoundingBoxComponent>();
	if (bb)
	{
		DrawBoundingBox(bb);
	}
}

void Renderer::RenderEditor()
{
	lxGPUProfile(RenderEditor);

	RenderFPS();
	RenderMessages();

	if (!g_Editor->GetOptions()->GetEnabled())
	{
		return;
	}

	g_Editor->Render();

	RenderTextureOverlay();


	GL::OutputErrors();

}

void Renderer::Render(World* world)
{
	lxGPUProfile(RenderFrame);

	BeginFrame(world);

	RenderShadowMap();

	BeforeWorldRender();

	RenderSky();

	RenderOpaque();

	RenderTerrain();

	AfterWorldRender();

	RenderHDRFramebuffer();

	RenderEditor();

	FinishFrame();
}

void Renderer::RenderEntities(ShaderFamily family, const std::vector<Entity*>& entities)
{
	for (Entity* entity : entities)
	{
		if (!entity->GetVisible())
		{
			continue;
		}

		Visual* visual = entity->GetComponent<Visual>();
		if (visual && visual->GetModel())
		{
			DrawModel(entity, visual->GetModel());
		}
	}
}

void Renderer::RenderEntities(std::vector<Entity*> entities)
{
	for (Entity* entity : entities)
	{
		if (!entity->GetVisible())
		{
			continue;
		}

		Visual* visual = entity->GetComponent<Visual>();
		if (visual && visual->GetModel())
		{
			DrawModel(entity, visual->GetModel());
		}
	}
}


void Renderer::HandleScreenshot()
{
	// screenshot
	//    if(saveScreenshot) {
	//        static unsigned int frameCount = 0;
	//        ++frameCount;
	//        std::stringstream filename;
	//        filename.clear();
	//        filename << "../output/frame" << std::setfill('0') << std::setw(5) << frameCount << ".png";

	//        BYTE* pixels = new BYTE[4 * windowWidth * windowHeight];
	//        glReadPixels(0, 0, windowWidth, windowHeight, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	//        FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, 4 * windowWidth, 32, 0x0000FF, 0xFF0000, 0x00FF00, false);
	//        if(FreeImage_Save(FIF_BMP, image, filename.str().c_str(), 0)) {
	//            std::cout << "screenshot '" << filename.str() << "' taken." << std::endl;
	//        } else {
	//            std::cout << "screenshot failed. Did you create the 'protoEngine/output' directory?" << std::endl;
	//        }
	//        FreeImage_Unload(image);
	//        delete [] pixels;
	//    }
	//    
}

void Renderer::RenderMessages()
{
	lxGPUProfile(RenderMessages);

	float fontSize = g_EngineSettings->GetMessagesFontSize();

	glm::ivec2 windowSize = m_MainWindow->GetSize();

	for (const S2DMessage& message : g_RenderDataManager->Get2DMessages())
	{
		glm::vec2 clipPos = glm::vec2(message.m_ScreenPosition) / glm::vec2(windowSize);
		clipPos.y = 1.f - clipPos.y;
		clipPos *= 2.f;
		clipPos -= glm::vec2(1.f, 1.f);

		RenderText(message.m_Message, clipPos.x, clipPos.y, fontSize, message.m_Color, false);

		GL::OutputErrors();

	}
	if (!g_Editor->GetOptions()->GetDisplayMessages() || g_RenderDataManager->GetTimedMessages().size() == 0)
	{
		return;
	}

	GL::SetViewport(m_MainWindow->GetSize());


	int numQuads = 0;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_TextInstanceSSBO);
	TextInstance_VertexData *ptr;
	ptr = (TextInstance_VertexData *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < g_RenderDataManager->GetTimedMessages().size(); ++i)
	{
		const TimedMessage& message = g_RenderDataManager->GetTimedMessages()[i];

		for (int j = 0; j < message.m_Message.size(); ++j)
		{
			glm::vec2 pos2(message.m_Position.x + j * message.m_Scale, message.m_Position.y);
			ptr[numQuads].m_Position = pos2;
			ptr[numQuads].m_Scale = message.m_Scale;

			const char& c = message.m_Message[j];
			int textureCharIndex = toupper(c) - 32;
			ptr[numQuads].m_TextureCharIndex = textureCharIndex;
			numQuads++;

			if (numQuads == g_MaxNumTextQuads)
			{
				break;
			}
		}
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	RenderTextBatch(numQuads, glm::vec3(1.f, 1.f, 1.f));

	GL::OutputErrors();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);

}

void Renderer::RenderFPS()
{
	lxGPUProfile(RenderFPS);

	if (!m_DisplayOptions->GetShowFPS())
	{
		return;
	}

	if (!SetPipeline(pPipelineText))
	{
		return;
	}

	GL::SetViewport(m_MainWindow->GetSize());

	float smoothing = 0.99f; // larger=more smoothing
	fps = (fps * smoothing) + (1.f / dt * (1.f - smoothing));
	//std::stringstream fpsString;
	//fpsString << 1.f / (fps / 1000.f);
	//fpsString << " ms/frame";

	LXString fpsString = lxFormat("%.3f ms", 1.f / (fps / 1000.f));

	glm::vec2 pos = m_DisplayOptions->GetFPSDisplayPosition();
	float scale = m_DisplayOptions->GetFPSDisplayScale();

	RenderText(fpsString, pos.x, pos.y, scale, glm::vec3(1.f, 1.f, 1.f), false);

	float new_time = (float) glfwGetTime();
	dt = new_time - curr_time;
	curr_time = new_time;
}

void Renderer::RenderSky()
{
	lxGPUProfile(RenderSky);

	if (!m_DisplayOptions->GetDrawSky())
	{
		return;
	}

	ProgramPipeline* envMapShader = m_Pipelines[ShaderFamily_Envmap];
	if (!SetPipeline(envMapShader))
	{
		return;
	}	
	
	GL::ClearDepthBuffer();
	GL::SetDepthFunction(GL::Depth_Always);
	GL::SetCapability(GL::CullFace, false);

	GLuint fragProg = envMapShader->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	float pi = 3.141592654f;
	glm::vec2 viewAngles = glm::vec2(m_ActiveCamera->GetFOVY() * pi, m_ActiveCamera->aspectRatio * m_ActiveCamera->GetFOVY() * pi) / 180.0f;
	SetFragmentUniform(viewAngles, "viewAngles");

	SetFragmentUniform(2.f * (glm::vec2) m_MainWindow->GetSize(), "windowSize");

	SetSkyUniforms(0);

	SetViewUniforms(m_ActiveCamera);

	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	GL::ClearDepthBuffer();
}


void Renderer::RenderText(const Text& t)
{
	RenderText(t.text, t.position.x, t.position.y, t.scale, glm::vec3(1.f, 1.f, 1.f), t.projected);
	//GL::SetCapability(GL::CullFace, true);
	//GL::SetCapability(GL::Blend, true);

	//GL::SetBlendMode(GL::SrcAlpha, GL::OneMinusSrcAlpha);

 //  // Activate corresponding render state
 //  pPipelineText->usePipeline();
 //  GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
 //  glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
 //  glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
 //  if (t.projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(m_ActiveCamera->GetViewProjectionMatrix()));
 //  else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f)));

 //  glActiveTexture(GL_TEXTURE0);

 //  // Iterate through all characters
 //  std::string::const_iterator c;
 //  for (c = t.text.begin(); c != t.text.end(); c++)
 //  {
 //      Character ch = Characters[*c];

 //      GLfloat xpos = t.position.x + ch.Bearing.x * t.scale;
 //      GLfloat ypos = t.position.y - (ch.Size.y - ch.Bearing.y) * t.scale;

 //      GLfloat w = ch.Size.x * t.scale;
 //      GLfloat h = ch.Size.y * t.scale;
 //      // Update VBO for each character
 //      GLfloat vertices[6][3] = {
 //          { xpos,     ypos + h,  0.0001f },
 //          { xpos,     ypos,      0.0001f },
 //          { xpos + w, ypos,      0.0001f },

 //          { xpos,     ypos + h,  0.0001f },
 //          { xpos + w, ypos,      0.0001f },
 //          { xpos + w, ypos + h,  0.0001f }
 //      };
 //      GLfloat uvs[6][2] = {
 //          {0.0, 0.0},
 //          {0.0, 1.0},
 //          {1.0, 1.0},

 //          {0.0, 0.0},
 //          {1.0, 1.0},
 //          {1.0, 0.0}

 //      };
 //      // Render glyph texture over quad
 //      glBindTexture(GL_TEXTURE_2D, ch.TextureID);
 //      // Update content of VBO memory
 //      glBindBuffer(GL_ARRAY_BUFFER, textVBO);
 //      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

 //      glBindBuffer(GL_ARRAY_BUFFER, textUvsVBO);
 //      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uvs), uvs);

 //      glBindBuffer(GL_ARRAY_BUFFER, 0);
 //      // Render quad
 //      glDrawArrays(GL_TRIANGLES, 0, 6);

 //      // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
 //      t.position.x += (ch.Advance >> 6) * t.scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
 //  }
 //  glBindVertexArray(0);
 //  glBindTexture(GL_TEXTURE_2D, 0);

 //  GL::SetCapability(GL::Blend, false);
 //  GL::SetCapability(GL::CullFace, false);
}

void Renderer::RenderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color, bool projected)
{
	GL::SetCapability(GL::Capabilities::Blend,		true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL::SetCapability(GL::Capabilities::CullFace,	false);
	GL::SetDepthFunction(GL::DepthFunction::Depth_Always);

	// todo : rework how we handle gfx hw stuff here
	if (!SetPipeline(pPipelineText, true))
	{
		return;
	}

	GLuint prog = activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram;

	SetFragmentUniform(color, "g_TextColor");
	if (projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(m_ActiveCamera->GetViewProjectionMatrix()));
	else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, (float)m_MainWindow->GetSize().x, 0.0f, (float)m_MainWindow->GetSize().y)));

	glActiveTexture(GL_TEXTURE0);

	// Iterate through all characters
	Mesh* mesh = g_DefaultObjects->DefaultQuadMesh;
	glBindVertexArray(mesh->m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetGPUBuffers().glidIndexBuffer);

	Texture* fontTexture = g_ObjectManager->FindObjectByID<Texture>(897003, true);
	glBindTexture(GL_TEXTURE_2D, fontTexture->GetHWObject());

	int numQuads = glm::min(100, (int)text.size());

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_TextInstanceSSBO);
	TextInstance_VertexData *ptr;
	ptr = (TextInstance_VertexData *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	glm::vec2 baseClipPosition = glm::vec2(x, y);

	int lineIndex = 0;

	float maxClipX = (1.f - scale);
	for (int i = 0; i < numQuads; ++i)
	{
		glm::vec2 pos2 = baseClipPosition + glm::vec2(lineIndex * scale, 0.f);
		
		lineIndex++;

		if (pos2.x > maxClipX)
		{
			baseClipPosition -= glm::vec2(0, scale); // lower on screen == lower in clip space
			lineIndex = 0;
			pos2 = baseClipPosition + glm::vec2(lineIndex * scale, 0.f);
		}

		ptr[i].m_Position = pos2;
		ptr[i].m_Scale = scale;

		char c = text[i];
		int textureCharIndex = toupper(c) - 32;
		ptr[i].m_TextureCharIndex = textureCharIndex;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
	
	glDrawElementsInstanced(GL_TRIANGLES, (int)mesh->m_buffers.GetIndexBuffer().size(), GL_UNSIGNED_INT, 0, numQuads);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	glBindVertexArray(0);
	FreeBoundTexture();

	GL::SetCapability(GL::Capabilities::Blend,		false);
	GL::SetCapability(GL::Capabilities::CullFace,	false);

}

void Renderer::RenderTextBatch(int numQuads, const glm::vec3& color)
{
	GL::SetCapability(GL::Capabilities::Blend, true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL::SetCapability(GL::Capabilities::CullFace, false);
	GL::SetDepthFunction(GL::DepthFunction::Depth_Always);

	// todo : rework how we handle gfx hw stuff here
	if (!SetPipeline(pPipelineText, true))
	{
		return;
	}

	SetFragmentUniform(color, "g_TextColor");

	// Iterate through all characters
	Mesh* mesh = g_DefaultObjects->DefaultQuadMesh;
	glBindVertexArray(mesh->m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetGPUBuffers().glidIndexBuffer);

	glActiveTexture(GL_TEXTURE0);
	Texture* fontTexture = g_ObjectManager->FindObjectByID<Texture>(897003, true);
	glBindTexture(GL_TEXTURE_2D, fontTexture->GetHWObject());

	glDrawElementsInstanced(GL_TRIANGLES, (int)mesh->m_buffers.GetIndexBuffer().size(), GL_UNSIGNED_INT, 0, numQuads);

	glBindVertexArray(0);
	FreeBoundTexture();

	GL::SetCapability(GL::Capabilities::Blend, false);
	GL::SetCapability(GL::Capabilities::CullFace, false);
}



void Renderer::AddToDebugModels(Model* model)
{
	lxAssert(model != nullptr);
	m_DebugModels.push_back(model);
}


// -----------------------------------------------------------------------------

GPUProfileHolder::GPUProfileHolder(const char* sectionName)
{
	if (RenderdocManager::GetInstance().GetIsEnabled())
	{
		glPushDebugGroupKHR(GL_DEBUG_SOURCE_APPLICATION, 0, -1, sectionName);
	}
}

GPUProfileHolder::~GPUProfileHolder()
{
	if (RenderdocManager::GetInstance().GetIsEnabled())
	{
		glPopDebugGroupKHR();
	}
}