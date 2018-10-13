#include "stdafx.h"
#include "renderer.h"

#include "program_pipeline.h"
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
#include "DisplayOptions.h"

#include "PostEffects.h"

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "World.h"
#include "Entity.h"
#include "Sector.h"
#include "Sunlight.h"
#include "BoundingBoxComponent.h"
#include "Heightfield.h"



#pragma region  CLASS_SOURCE Renderer

#include "Renderer.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Renderer::g_Properties[] = 
{
{ "DisplayOptions", PIDX_DisplayOptions, offsetof(Renderer, m_DisplayOptions), 0, LXType_ObjectPtr, sizeof(DisplayOptions*), LXType_DisplayOptions, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "PostEffects", PIDX_PostEffects, offsetof(Renderer, m_PostEffects), 0, LXType_ObjectPtr, sizeof(PostEffects*), LXType_PostEffects, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugCamera", PIDX_DebugCamera, offsetof(Renderer, m_DebugCamera), 0, LXType_ObjectPtr, sizeof(Camera*), LXType_Camera, true, LXType_None, false, 0, 0, 0, 0,}, 
};
bool Renderer::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
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
	int windowWidth = m_Window->GetSize().x;
	int windowHeight = m_Window->GetSize().y;

	m_Framebuffers[FramebufferType_MainColorBuffer] = new Framebuffer("Main Color Buffer", windowWidth, windowHeight, GLPixelFormat_RGBA16F, GLPixelFormat_RGBA, GLPixelType_Float);
	m_Framebuffers[FramebufferType_MainColorBuffer]->SetHasDepth(true);
	m_Framebuffers[FramebufferType_MainColorBuffer]->SetNumColorTargets(2);
	m_Framebuffers[FramebufferType_MainColorBuffer]->Initialize();

	m_Framebuffers[FramebufferType_ShadowMap] = new Framebuffer("Shadow Map Buffer", SHADOW_WIDTH, SHADOW_HEIGHT, GLPixelFormat_DepthComponent, GLPixelFormat_DepthComponent, GLPixelType_Float);
	m_Framebuffers[FramebufferType_ShadowMap]->SetHasDepth(true);
	m_Framebuffers[FramebufferType_ShadowMap]->SetNumColorTargets(0);
	m_Framebuffers[FramebufferType_ShadowMap]->Initialize();

	m_Framebuffers[FramebufferType_Picking] = new Framebuffer("Picking Buffer", g_Editor->GetPickingBufferSize(), g_Editor->GetPickingBufferSize(), GLPixelFormat_RGBA16F, GLPixelFormat_RGBA, GLPixelType_Float);
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
	return m_Window->GetSize();
}

void APIENTRY OutputGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::stringstream ss;

	ss << "OpenGL debug message : " << message;

	OUTPUT_STRING_LINE(ss.str());
}

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


	m_Window = new MainWindow(75638);

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

	glGenBuffers(1, &SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 12 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);


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
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could nolololt init FreeType Library" << std::endl;

	FT_Face face;
	int success = FT_New_Face(ft, "data/fonts/arial.ttf", 0, &face);
	if (success == FT_SUCCESS)
	{
		PRINT("Loaded Freetype font! yayy");
	}
	else
	{
		PRINT("Failed to load freetype font.");

	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != FT_SUCCESS)
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = 
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

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
	pPipelineBasicUV = new ProgramPipeline("BasicUV");
	pPipelineShadowMap = new ProgramPipeline("ShadowMap");
	pPipelineLines = new ProgramPipeline("general_lines");
	pPipelineGround = new ProgramPipeline("Terrain");
	pPipelineEnvmap = new ProgramPipeline("Envmap");
	pPipelineScreenSpaceTexture = new ProgramPipeline("ScreenSpaceTexture");
	pPipelineNodes = new ProgramPipeline("nodes");
	pPipelinePicking = new ProgramPipeline("picking");
	pPipelinePickingCompute = new ProgramPipeline("picking_compute", true);
	pPipelineUVEdges = new ProgramPipeline("UVEdges");
	pPipelineSolidColor = new ProgramPipeline("SolidColor");
	pPipelineBlur = new ProgramPipeline("blur");
	pPipelineGrid = new ProgramPipeline("Grid");
	pPipelineAxisGizmo = new ProgramPipeline("AxisGizmo");
	GL::OutputErrors();


	m_Pipelines.clear();
	for (int i = 0; i < EnumLength_ShaderFamily; ++i)
	{
		m_Pipelines.push_back(new ProgramPipeline(EnumValues_ShaderFamily[i]));
		m_ShaderBeenUsedThisFrame.push_back(false);
	}

	PRINTSTRING("Successfully built all shaders!");

}



void Renderer::Initialize()
{

	fps = 300.f;
	dt = 0.1f;

	m_ShadowCamera = new Camera();
	m_ShadowCamera->SetProjectionType(ProjectionType_Orthographic);
	m_ShadowCamera->SetPosition(glm::vec3(-16.13f, -5.9f, 20.2f));
	m_ShadowCamera->SetFrontVector(glm::vec3(-0.47f, -0.81f, 0.34f));
	m_ShadowCamera->SetRightVector(normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), m_ShadowCamera->GetFrontVector())));
	m_ShadowCamera->SetUpVector(glm::cross(m_ShadowCamera->GetFrontVector(), m_ShadowCamera->GetRightVector()));
	m_ShadowCamera->translateTo(m_ShadowCamera->GetPosition() - m_ShadowCamera->GetFrontVector() * 10.f);
	m_ShadowCamera->UpdateVPMatrix();

	InitGL();

	InitFreetype();

	InitPipelines();

	g_Editor->Initialize();

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


void Renderer::SetUniform(glm::mat4x4& value, const char* name, GLuint location)
{
	glProgramUniformMatrix4fv(activePipeline->getShader(location)->glidShaderProgram, glGetUniformLocation(activePipeline->getShader(location)->glidShaderProgram, name), 1, false, value_ptr(value));
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
	glProgramUniform1i(prog, glGetUniformLocation(prog, name), value);
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
	glProgramUniform1iv(prog, glGetUniformLocation(prog, name), values.size(), values.data());
}

void Renderer::SetFragmentUniformArray(const std::vector<glm::vec3>& values, const char* name)
{
	GLuint prog = activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	glProgramUniform3fv(prog, glGetUniformLocation(prog, name), values.size() * 3, (const GLfloat*) values.data());
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


void Renderer::SetPipeline(ProgramPipeline* pipeline, bool force)
{
	lxAssert(pipeline != nullptr);
	if (force || activePipeline != pipeline)
	{
		activePipeline = pipeline;
		activePipeline->usePipeline();
	}
}

void Renderer::SetPipeline(ProgramPipeline* pipeline)
{
	SetPipeline(pipeline, false);
}

void Renderer::SetPipeline(ShaderFamily family)
{
	SetPipeline(m_Pipelines[family]);
}

void Renderer::SetDisplayModeUniforms()
{
	SetFragmentUniform((int) m_DisplayOptions->GetDisplayMode(), "g_DisplayMode");
}

void Renderer::SetSkyUniforms(int skyCubemapSlot)
{
	SunLight* sunLight = m_World->GetSunLight();

	SetFragmentUniform(skyCubemapSlot, "g_Skybox");
	BindCubemap(skyCubemapSlot, m_World->GetSunLight()->GetSkybox()->GetHWObject());

	SetFragmentUniform(m_World->GetSunLight()->GetUseSkybox(), "g_UseSkybox");
	SetFragmentUniform(sunLight->GetOrientation(), "sunOrientation");
	SetFragmentUniform(sunLight->GetTime(), "sunTime");
}

void Renderer::SetLightingUniforms()
{
	bool useSkyLighting = m_DisplayOptions->GetUseSkyLighting();
	SetFragmentUniform(useSkyLighting,	"g_UseSkyLighting");
	SetFragmentUniform(m_DisplayOptions->GetRenderShadows(),	"g_UseShadows");

	if (useSkyLighting)
	{
		SetFragmentUniform(m_TestLight[0].m_Position, "g_DirectionalLight.m_Direction");
		SetFragmentUniform(m_TestLight[0].m_DiffuseColor, "g_DirectionalLight.m_DiffuseColor");
		SetFragmentUniform(m_TestLight[0].m_AmbientColor, "g_DirectionalLight.m_AmbientColor");
		SetFragmentUniform(m_TestLight[0].m_SpecularColor, "g_DirectionalLight.m_SpecularColor");
	}
	else
	{
		for (int i = 0; i < m_NumLights; ++i)
		{
			std::string index = std::to_string(i);
			SetFragmentUniform(m_TestLight[i].m_Position,		("g_PointLight[" + index + "].m_Position").c_str()	);
			SetFragmentUniform(m_TestLight[i].m_DiffuseColor,	("g_PointLight[" + index + "].m_DiffuseColor").c_str());
			SetFragmentUniform(m_TestLight[i].m_AmbientColor,	("g_PointLight[" + index + "].m_AmbientColor").c_str());
			SetFragmentUniform(m_TestLight[i].m_SpecularColor,	("g_PointLight[" + index + "].m_SpecularColor").c_str());
		}
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

	SetVertexUniform(cam->GetViewProjectionMatrix(), "g_LightProjectionMatrix");
}

void Renderer::SetWorldGridUniforms()
{
	SetFragmentUniform(g_EngineSettings->GetWorldScale(), "g_WorldScale");

	SectorTool* sectorTool = (SectorTool*) g_Editor->GetTools()[EEditorTool_SectorTool];
	SetFragmentUniform(sectorTool->GetSectorGridColor(), "g_SectorGridColor");
}

void Renderer::SetViewUniforms(Camera* cam)
{
	SetVertexUniform(cam->GetViewProjectionMatrix(), "vpMat");
	SetVertexUniform(cam->GetViewMatrix(),			"g_WorldToViewMatrix");
	SetVertexUniform(cam->GetViewMatrixNoTranslation(), "g_WorldToViewMatrixNoTranslation");
	SetVertexUniform(glm::mat4(glm::mat3(cam->GetViewMatrix())), "g_WorldToViewMatrixRotationOnly");
	SetVertexUniform(cam->GetProjectionMatrix(), "g_ProjectionMatrix");

	SetFragmentUniform(cam->GetViewMatrixInverse(), "g_ViewToWorldMatrix");
	SetFragmentUniform(cam->GetViewProjectionMatrixInverse(), "g_ViewProjectionMatrixInverse");
	SetFragmentUniform(cam->GetViewMatrixInverse(), "g_ViewMatrixInverse");
	SetFragmentUniform(cam->GetProjectionMatrixInverse(), "g_ProjectionMatrixInverse");
	SetFragmentUniform(cam->GetPosition(),	"g_CameraPosition");
	SetFragmentUniform(cam->GetNearPlane(),	"g_CameraNearPlane");
	SetFragmentUniform(cam->GetFarPlane(), "g_CameraFarPlane");
	SetFragmentUniform(cam->GetFrontVector(),	"g_CameraLookAt");
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
	for (int i = 0; i < model->m_meshes.size(); ++i)
	{

		Material* material = model->GetMaterials()[i];
		if (!material->GetEnabled())
		{
			continue;
		}

		SetPipeline(material->GetShaderFamily());

		SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

		if (!m_ShaderBeenUsedThisFrame[material->GetShaderFamily()])
		{
			SetLightingUniforms();
			SetViewUniforms(m_DebugCamera);
			SetShadowMapUniforms(m_ShadowCamera);
			SetSkyUniforms(3);
			SetDisplayModeUniforms();

			SetPostEffectsUniforms();
			SetDebugUniforms();
			m_ShaderBeenUsedThisFrame[material->GetShaderFamily()] = true;
		}

		DrawMesh(model->m_meshes[i], material);

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
	glBindVertexArray(mesh->m_VAO);

	if (mesh->GetUsesIndexBuffer())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetGPUBuffers().glidIndexBuffer);

		GL::DrawElements(mesh->m_PrimitiveMode, (int) mesh->m_buffers.indexBuffer.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(mesh->m_PrimitiveMode, 0, (int) mesh->m_buffers.m_VertexPositions.size());
		GL::OutputErrors();
	}

	g_EngineStats->AddTo_NumDrawCalls(1);
}


void Renderer::DrawMesh(Mesh* mesh, Material* material)
{
  glBindVertexArray(mesh->m_VAO);

  SetMaterialUniforms(material);

  if (mesh->GetWireframeRendering())
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

  if (mesh->GetWireframeRendering())
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

}

void Renderer::RenderTerrain()
{
	if (!m_DisplayOptions->GetRenderTerrain())
	{
		return;
	}


	SetPipeline(m_Pipelines[ShaderFamily_Terrain]);

	SetLightingUniforms();
	SetViewUniforms(m_DebugCamera);
	SetShadowMapUniforms(m_ShadowCamera);
	SetSkyUniforms(3);
	SetDisplayModeUniforms();

	SetPostEffectsUniforms();
	SetDebugUniforms();

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	Material* terrainMaterial = nullptr;

	SetFragmentUniform(4, "g_DirtTexture");
	Bind2DTexture(4, g_DefaultTextureHolder->GetDirtTexture());

	SetFragmentUniform(5, "g_SandTexture");
	Bind2DTexture(5, g_DefaultTextureHolder->GetSandTexture());

	SetFragmentUniform(6, "g_WoodTexture");
	Bind2DTexture(6, g_DefaultTextureHolder->GetWoodTexture());



	for (int i = 0; i < g_RenderDataManager->GetVisibleSectors().size(); ++i)
	{
		Sector* sector = g_RenderDataManager->GetVisibleSectors()[i];

		if (sector)
		{
			Entity* entity = sector->GetTerrainPatchEntity();

			if (entity)
			{
				SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");
				terrainMaterial = entity->GetModel()->GetMaterials()[0];

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

void Renderer::RenderShadowMap()
{
	if (!m_DisplayOptions->GetRenderShadows())
	{
		return;
	}

	GL::SetViewport(SHADOW_WIDTH, SHADOW_HEIGHT);
	
	BindFramebuffer(FramebufferType_ShadowMap);
	
	GL::ClearDepthBuffer();

	SetPipeline(pPipelineShadowMap);

	glm::vec3 pos = glm::normalize(m_World->GetSunLight()->GetSunDirection());

	m_TestLight[0].m_Position = pos;
	SetLightingUniforms();

	m_ShadowCamera->SetPosition(glm::vec3(0, 20, 1) + pos * 15.f);
	m_ShadowCamera->SetFrontVector(pos);
	m_ShadowCamera->SetRightVector(normalize(glm::cross(glm::vec3(0, 0, 1), m_ShadowCamera->GetFrontVector())));
	m_ShadowCamera->SetUpVector(normalize(glm::cross(m_ShadowCamera->GetFrontVector(), m_ShadowCamera->GetRightVector())));
	m_ShadowCamera->UpdateVPMatrix();

	SetViewUniforms(m_ShadowCamera);
	SetPostEffectsUniforms();
	SetDebugUniforms();

	RenderEntities(g_RenderDataManager->GetVisibleEntities());

	BindFramebuffer(FramebufferType_Default);
}

void Renderer::RenderOpaque()
{
	if (!m_DisplayOptions->GetRenderOpaque())
	{
		return;
	}

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	RenderEntities(ShaderFamily_Basic, g_RenderDataManager->GetVisibleEntities());

	if (g_Editor->GetOptions()->GetDebugDisplay())
	{
		RenderEntities(ShaderFamily_Basic, m_World->GetDebugEntities());
	}
}

void Renderer::RenderTextureOverlay()
{
	if (!m_DisplayOptions->GetRenderTextureOverlay())
	{
		return;
	}

	GL::SetViewport(300, 300);

	SetPipeline(pPipelineScreenSpaceTexture);

	SetPostEffectsUniforms();

	SetFragmentUniform(0, "g_MainTexture");
	Bind2DTexture(0, m_Framebuffers[FramebufferType_Picking]->GetColorTexture(0));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Renderer::RenderHDRFramebuffer()
{
	BindFramebuffer(FramebufferType_Default);
	GL::SetViewport(m_Window->GetSize());
	GL::ClearColorAndDepthBuffers();

	SetPipeline(pPipelineScreenSpaceTexture);

	SetPostEffectsUniforms();

	SetFragmentUniform(0, "g_MainTexture");
	Bind2DTexture(0, m_Framebuffers[FramebufferType_MainColorBuffer]->GetColorTexture(0));

	SetFragmentUniform(1, "g_GlowTexture");
	Bind2DTexture(1, m_Framebuffers[FramebufferType_PingPong1]->GetColorTexture(0));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::GetPickingData(glm::vec2 mouseClickPosition, glm::vec4& pickingData)
{
	SetPipeline(pPipelinePickingCompute);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	SetComputeUniform(1, "g_PickingBuffer");
	Bind2DTexture(1, m_Framebuffers[FramebufferType_Picking]->GetColorTexture(0));

	SetComputeUniform(2, "g_DepthBuffer");
	Bind2DTexture(2, m_Framebuffers[FramebufferType_Picking]->GetDepthTexture());


	SetComputeUniform(glm::vec2(m_Window->GetSize()), "g_WindowSize");
	SetComputeUniform(mouseClickPosition, "g_MousePosition");

	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

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
	int vpWidth = m_Framebuffers[FramebufferType_Picking]->GetWidth();
	int vpHeight = m_Framebuffers[FramebufferType_Picking]->GetHeight();
	GL::SetViewport(vpWidth, vpHeight);
	
	SetPipeline(pPipelinePicking);
	
	BindFramebuffer(FramebufferType_Picking);
	GL::ClearColorAndDepthBuffers();
	
	SetViewUniforms(m_DebugCamera);


	SetVertexUniform(1, "g_UseHeightfield");

	for (Sector* sector : g_RenderDataManager->GetVisibleSectors())
	{
		Entity* entity = sector->GetTerrainPatchEntity();
		SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
		SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

		SetVertexUniform(3, "g_HeightfieldTexture");
		Bind2DTexture(3, sector->GetHeightfield()->GetHeightDataTexture()->GetHWObject());

		for (int i = 0; i < entity->GetModel()->m_meshes.size(); ++i)
		{
			DrawMesh(entity->GetModel()->m_meshes[i]);
		}
	}
	
	SetVertexUniform(0, "g_UseHeightfield");

	for (Entity* entity : g_RenderDataManager->GetVisibleEntities())
	{
		SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
		SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");
	
		for (int i = 0; i < entity->GetModel()->m_meshes.size(); ++i)
		{
			DrawMesh(entity->GetModel()->m_meshes[i]);
		}
	}
	
	if (debugEntities)
	{
		for (Entity* entity : m_World->GetDebugEntities())
		{
			SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
			SetVertexUniform(entity->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

			for (int i = 0; i < entity->GetModel()->m_meshes.size(); ++i)
			{
				DrawMesh(entity->GetModel()->m_meshes[i]);
			}
		}
	}


	BindFramebuffer(FramebufferType_Default);
}

void Renderer::BeginFrame(World* world)
{
	m_DebugCamera->handlePresetNewFrame(m_Window->pWindow);

	m_DebugCamera->UpdateVPMatrix();

	g_EngineStats->ResetFrame();

	m_World = world;
	GL::g_CheckGLErrors = m_DisplayOptions->GetOutputGLErrors();

	m_NumLights = 0;

	for (Entity* entity : m_World->GetEntities())
	{
		if (entity->GetIsLight())
		{
			m_TestLight[m_NumLights].m_Position			= entity->GetPosition();
			m_TestLight[m_NumLights].m_DiffuseColor		= entity->GetModel()->GetMaterials()[0]->GetDiffuseColor();
			m_TestLight[m_NumLights].m_SpecularColor	= m_TestLight[m_NumLights].m_DiffuseColor;
			m_TestLight[m_NumLights].m_AmbientColor		= m_TestLight[m_NumLights].m_DiffuseColor;
		
			m_NumLights++;
		}
	}

	for (int i = 0; i < EnumLength_ShaderFamily; ++i)
	{
		m_ShaderBeenUsedThisFrame[i] = false;;
	}

	g_Editor->ApplyTool();

}

void Renderer::ApplyEmissiveGlow()
{
	if (!m_PostEffects->GetEmissiveGlowEnabled())
	{
		return;
	}

	SetPipeline(pPipelineBlur);


	bool horizontal = true;
	unsigned int amount = 10;
	for (unsigned int i = 0; i < amount; i++)
	{
		FramebufferType fb = horizontal ? FramebufferType_PingPong0 : FramebufferType_PingPong1;
		FramebufferType fb2 = fb == FramebufferType_PingPong0 ? FramebufferType_PingPong1 : FramebufferType_PingPong0;

		BindFramebuffer(fb);

		SetFragmentUniform(horizontal, "horizontal");

		GLuint sourceTexture = (i == 0 ? m_Framebuffers[FramebufferType_MainColorBuffer]->GetColorTexture(1) : m_Framebuffers[fb2]->GetColorTexture(0));
		Bind2DTexture(0, sourceTexture);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		horizontal = !horizontal;
	}

	BindFramebuffer(FramebufferType_Default);
}

void Renderer::BeforeWorldRender()
{
	BindFramebuffer(FramebufferType_MainColorBuffer);

	GL::SetViewport(m_Window->GetSize());

	GL::ClearColorAndDepthBuffers();

	GL::OutputErrors();
}


void Renderer::RenderGrid()
{
	GL::DepthWriteEnabled(false);

	SetPipeline(pPipelineGrid);

	GL::SetCapability(GL::Capabilities::Blend, true);
	GL::SetBlendMode(GL::SrcAlpha, GL::BlendMode::OneMinusSrcAlpha);

	SetWorldGridUniforms();
	SetViewUniforms(m_DebugCamera);
	SetFragmentUniform(glm::vec2(m_Window->GetSize()), "g_WindowSize");

	Mesh* mesh = g_DefaultObjects->DefaultQuadMesh;
	DrawMesh(mesh);

	GL::DepthWriteEnabled(true);

	GL::SetCapability(GL::Capabilities::Blend, false);
}

void Renderer::RenderAxisGizmo()
{
	SetPipeline(pPipelineAxisGizmo);

	SetViewUniforms(m_DebugCamera);

	GL::DrawArrays(GL::PrimitiveMode::Lines, 0, 6);
}


void Renderer::RenderDebugModel(Model* model, const glm::mat4& modelToWorld, ProgramPipeline* programPipeline)
{
	SetPipeline(programPipeline);

	for (int i = 0; i < model->m_meshes.size(); ++i)
	{
		SetVertexUniform(modelToWorld, "g_ModelToWorldMatrix");

		Material* material = model->GetMaterials()[i];

		SetViewUniforms(m_DebugCamera);

		DrawMesh(model->m_meshes[i], material);
	}
}

void Renderer::RenderDebugWays(Model* model, const glm::mat4& modelToWorld, ProgramPipeline* programPipeline, const std::vector<int>& displayFlags, const std::vector<glm::vec3>& wayDebugColors, int selectedWay)
{
	
	SetPipeline(programPipeline);

	GL::SetCapability(GL::Capabilities::Blend, true);

	for (int i = 0; i < model->m_meshes.size(); ++i)
	{
		SetVertexUniform(modelToWorld, "g_ModelToWorldMatrix");

		Material* material = model->GetMaterials()[i];

		SetViewUniforms(m_DebugCamera);

		SetFragmentUniform(selectedWay, "g_SelectedWayIndex");
		SetFragmentUniform(displayFlags, "g_DisplayFlags");
		SetFragmentUniformArray(wayDebugColors, "g_WayDebugColors");

		DrawMesh(model->m_meshes[i], material);
	}

	GL::SetCapability(GL::Capabilities::Blend, false);
}

void Renderer::RenderDebugModels()
{
	g_Editor->RenderTools();

	if (m_DisplayOptions->GetDisplayDebugModels())
	{
		SetPipeline(pPipelineBasic);
		for (Model* model : m_DebugModels)
		{
			// TODO : right now this is for buildings but they should create their own entity
			for (int i = 0; i < model->m_meshes.size(); ++i)
			{
				SetVertexUniform(glm::mat4(1.0), "g_ModelToWorldMatrix");

				Material* material = model->GetMaterials()[i];

				SetViewUniforms(m_DebugCamera);

				if (!material->GetEnabled())
				{
					continue;
				}

				DrawMesh(model->m_meshes[i], material);
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
	RenderDebugModels();

	ApplyEmissiveGlow();
}

void Renderer::FinishFrame()
{
	g_RenderDataManager->Update();

	HandleScreenshot();
	
	glfwSwapBuffers(m_Window->GetHWObject());
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

	SetPipeline(pPipelineUVEdges);

	glm::mat4 modelToWorldMatrix = mat4(1.0f);

	glm::vec3 translation = aabb.GetStartPoint() + aabb.GetScale() / 2.f;
	modelToWorldMatrix = glm::translate(modelToWorldMatrix, translation);
	modelToWorldMatrix = glm::scale(modelToWorldMatrix, aabb.GetScale());

	SetVertexUniform(modelToWorldMatrix, "g_ModelToWorldMatrix");
	SetFragmentUniform(color, "g_Color");

	SetViewUniforms(m_DebugCamera);

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

	SetPipeline(pPipelineUVEdges);

	SetViewUniforms(m_DebugCamera);
	SetVertexUniform(bb->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	GL::SetCapability(GL::Blend, false);
}

void Renderer::DrawManipulator(Entity* entity)
{
	GL::SetCapability(GL::Blend, true);
	GL::SetBlendMode(GL::SrcAlpha, GL::OneMinusSrcAlpha);

	SetPipeline(pPipelineSolidColor);

	SetViewUniforms(m_DebugCamera);
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
	if (!g_Editor->GetOptions()->GetEnabled())
	{
		return;
	}

	g_Editor->Render();

	RenderTextureOverlay();

	RenderFPS();

	GL::OutputErrors();

	RenderMessages();
}

void Renderer::render(World* world)
{
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

		DrawModel(entity, entity->GetModel());
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

		DrawModel(entity, entity->GetModel());
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
	float fontSize = g_EngineSettings->GetMessagesFontSize();
	for (const S2DMessage& message : g_RenderDataManager->Get2DMessages())
	{
		RenderText(message.m_Message, message.m_ScreenPosition.x, message.m_ScreenPosition.y, fontSize, message.m_Color, false);

		GL::OutputErrors();

	}
	if (!g_Editor->GetOptions()->GetDisplayMessages() || g_RenderDataManager->GetTimedMessages().size() == 0)
	{
		return;
	}

	GL::SetViewport(m_Window->GetSize());

	glm::vec2 startingPosition = g_EngineSettings->GetMessagesStartingPosition();
	float heightOffset = g_EngineSettings->GetMessagesPixelsOffset();

	int numMessages = g_RenderDataManager->GetTimedMessages().size();
	startingPosition.y += heightOffset * (numMessages - 1);

	for (const TimedMessage& message : g_RenderDataManager->GetTimedMessages())
	{
		RenderText(message.m_Message, startingPosition.x, startingPosition.y, fontSize, glm::vec3(0.5, 0.8f, 0.2f), false);

		startingPosition.y -= heightOffset;

		GL::OutputErrors();
	}


}

void Renderer::RenderFPS()
{
	if (!m_DisplayOptions->GetShowFPS())
	{
		return;
	}

	SetPipeline(pPipelineText);

	GL::SetViewport(m_Window->GetSize());

	float smoothing = 0.99f; // larger=more smoothing
	fps = (fps * smoothing) + (1.f / dt * (1.f - smoothing));
	std::stringstream fpsString;
	fpsString << 1.f / (fps / 1000.f);
	fpsString << " ms/frame";

	static volatile float xPos = 1150.f;
	static volatile float yPos = 950.f;
	RenderText(fpsString.str(), xPos, yPos, 0.3f, glm::vec3(0.5, 0.8f, 0.2f), false);

	float new_time = (float) glfwGetTime();
	dt = new_time - curr_time;
	curr_time = new_time;
}

void Renderer::RenderSky()
{
	if (!m_DisplayOptions->GetDrawSky())
	{
		return;
	}

	SetPipeline(pPipelineEnvmap);

	GLuint fragProg = pPipelineEnvmap->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	float pi = 3.141592654f;
	glm::vec2 viewAngles = glm::vec2(m_DebugCamera->totalViewAngleY*pi, m_DebugCamera->aspectRatio*m_DebugCamera->totalViewAngleY*glm::pi<float>()) / 180.0f;
	SetFragmentUniform(viewAngles, "viewAngles");

	SetFragmentUniform(2.f * (glm::vec2) m_Window->GetSize(), "windowSize");

	SetSkyUniforms(0);

	SetViewUniforms(m_DebugCamera);

	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	GL::ClearDepthBuffer();
}


void Renderer::RenderText(Text t)
{
	GL::SetCapability(GL::CullFace, true);
	GL::SetCapability(GL::Blend, true);

	GL::SetBlendMode(GL::SrcAlpha, GL::OneMinusSrcAlpha);

   // Activate corresponding render state
   pPipelineText->usePipeline();
   GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
   glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
   glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
   if (t.projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(m_DebugCamera->GetViewProjectionMatrix()));
   else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f)));

   glActiveTexture(GL_TEXTURE0);

   // Iterate through all characters
   std::string::const_iterator c;
   for (c = t.text.begin(); c != t.text.end(); c++)
   {
       Character ch = Characters[*c];

       GLfloat xpos = t.position.x + ch.Bearing.x * t.scale;
       GLfloat ypos = t.position.y - (ch.Size.y - ch.Bearing.y) * t.scale;

       GLfloat w = ch.Size.x * t.scale;
       GLfloat h = ch.Size.y * t.scale;
       // Update VBO for each character
       GLfloat vertices[6][3] = {
           { xpos,     ypos + h,  0.0001f },
           { xpos,     ypos,      0.0001f },
           { xpos + w, ypos,      0.0001f },

           { xpos,     ypos + h,  0.0001f },
           { xpos + w, ypos,      0.0001f },
           { xpos + w, ypos + h,  0.0001f }
       };
       GLfloat uvs[6][2] = {
           {0.0, 0.0},
           {0.0, 1.0},
           {1.0, 1.0},

           {0.0, 0.0},
           {1.0, 1.0},
           {1.0, 0.0}

       };
       // Render glyph texture over quad
       glBindTexture(GL_TEXTURE_2D, ch.TextureID);
       // Update content of VBO memory
       glBindBuffer(GL_ARRAY_BUFFER, textVBO);
       glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

       glBindBuffer(GL_ARRAY_BUFFER, textUvsVBO);
       glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uvs), uvs);

       glBindBuffer(GL_ARRAY_BUFFER, 0);
       // Render quad
       glDrawArrays(GL_TRIANGLES, 0, 6);

       // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
       t.position.x += (ch.Advance >> 6) * t.scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
   }
   glBindVertexArray(0);
   glBindTexture(GL_TEXTURE_2D, 0);

   GL::SetCapability(GL::Blend, false);
   GL::SetCapability(GL::CullFace, false);
}

void Renderer::RenderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color, bool projected)
{
	GL::SetCapability(GL::Capabilities::Blend,		true);
	GL::SetCapability(GL::Capabilities::CullFace,	true);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // todo : rework how we handle gfx hw stuff here
   SetPipeline(pPipelineText, true);

   GLuint prog = activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram;

   SetFragmentUniform(color, "g_TextColor");
   if (projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(m_DebugCamera->GetViewProjectionMatrix()));
   else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, (float)m_Window->GetSize().x, 0.0f, (float)m_Window->GetSize().y)));

   glActiveTexture(GL_TEXTURE0);

   // Iterate through all characters
   std::string::const_iterator c;
   for (c = text.begin(); c != text.end(); c++)
   {
       Character ch = Characters[*c];

       GLfloat xpos = x + ch.Bearing.x * scale;
       GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

       GLfloat w = ch.Size.x * scale;
       GLfloat h = ch.Size.y * scale;

       // Update VBO for each character
       GLfloat vertices[6][3] = {
           { xpos,     ypos + h,  0.0001f },
           { xpos,     ypos,      0.0001f },
           { xpos + w, ypos,      0.0001f },

           { xpos,     ypos + h,  0.0001f },
           { xpos + w, ypos,      0.0001f },
           { xpos + w, ypos + h,  0.0001f }
       };
       GLfloat uvs[6][2] = {
           {0.0, 0.0},
           {0.0, 1.0},
           {1.0, 1.0},

           {0.0, 0.0},
           {1.0, 1.0},
           {1.0, 0.0}

       };
       // Render glyph texture over quad
       glBindTexture(GL_TEXTURE_2D, ch.TextureID);
       // Update content of VBO memory
       glBindBuffer(GL_ARRAY_BUFFER, textVBO);
       glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

       glBindBuffer(GL_ARRAY_BUFFER, textUvsVBO);
       glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uvs), uvs);

       glBindBuffer(GL_ARRAY_BUFFER, 0);
       // Render quad
       glDrawArrays(GL_TRIANGLES, 0, 6);

       // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
       x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
   }

   glBindVertexArray(0);
   FreeBoundTexture();

   GL::SetCapability(GL::Capabilities::Blend,		false);
   GL::SetCapability(GL::Capabilities::CullFace,	false);

}

void Renderer::AddToDebugModels(Model* model)
{
	lxAssert(model != nullptr);
	m_DebugModels.push_back(model);
}
