#include "stdafx.h"

#include "renderer.h"
#include "program_pipeline.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "World.h"
#include "Entity.h"
#include "RenderDataManager.h"
#include "DisplayOptions.h"
#include "Editor.h"
#include "EditorOptions.h"
#include "Settings.h"
#include "PostEffects.h"
#include "Sunlight.h"
#include "DefaultObjects.h"
#include "GL.h"
#include "Framebuffer.h"
#include "Sector.h"
#include "BoundingBoxComponent.h"
#include "EngineStats.h"
#include "EngineSettings.h"

#pragma region  CLASS_SOURCE Renderer

#include "Renderer.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Renderer::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(Renderer, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(Renderer, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "DisplayOptions", PIDX_DisplayOptions, offsetof(Renderer, m_DisplayOptions), 0, LXType_DisplayOptions, true, LXType_None, 0, 0, 0, }, 
{ "PostEffects", PIDX_PostEffects, offsetof(Renderer, m_PostEffects), 0, LXType_PostEffects, true, LXType_None, 0, 0, 0, }, 
{ "DebugCamera", PIDX_DebugCamera, offsetof(Renderer, m_DebugCamera), 0, LXType_Camera, true, LXType_None, 0, 0, 0, }, 
};
bool Renderer::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing);
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
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

void Renderer::InitFramebuffers()
{
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

	BindFramebuffer(FramebufferType_Default);
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

	// Create GLFW window
	pWindow = GL::CreateGLWindow(windowWidth, windowHeight, windowTitle.c_str());

	glfwSetWindowPos(pWindow, -1180, 50);
	glfwMakeContextCurrent(pWindow);
	if (pWindow == NULL)
	{
		cerr << "Failed to open GLFW window.\n";
		glfwTerminate();
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		//return -1;
		exit(1);
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
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
#ifdef __APPLE__
			static_cast<GLuint>(face->glyph->advance.x)
#else
			face->glyph->advance.x
#endif
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

}

void Renderer::PostSerialization(bool writing)
{
	if (!writing)
	{
		Initialize();
	}
}

void Renderer::Initialize()
{
	windowWidth = 1100;
	windowHeight = 880;
	windowTitle = "LigumX";
	fps = 300;
	dt = 0.1;

	m_ShadowCamera = new Camera();
	m_ShadowCamera->SetProjectionType(ProjectionType_Orthographic);
	m_ShadowCamera->SetPosition(glm::vec3(-16.13, -5.9, 20.2));
	m_ShadowCamera->SetFrontVector(glm::vec3(-0.47, -0.81, 0.34));
	m_ShadowCamera->SetRightVector(normalize(glm::cross(glm::vec3(0, 0, 1), m_ShadowCamera->GetFrontVector())));
	m_ShadowCamera->SetUpVector(glm::cross(m_ShadowCamera->GetFrontVector(), m_ShadowCamera->GetRightVector()));
	m_ShadowCamera->translateTo(m_ShadowCamera->GetPosition() - m_ShadowCamera->GetFrontVector() * 10.f);
	m_ShadowCamera->updateVPMatrix();

	InitGL();

	InitFreetype();

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
void Renderer::SetComputeUniform(T& value, const char* name)
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


void Renderer::SetPipeline(ProgramPipeline* pipeline)
{
	activePipeline = pipeline;
	activePipeline->usePipeline();
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

void Renderer::SetViewUniforms(Camera* cam)
{
	SetVertexUniform(cam->GetViewProjectionMatrix(), "vpMat");
	SetVertexUniform(cam->GetViewMatrix(),			"g_WorldToViewMatrix");
	SetVertexUniform(glm::mat4(glm::mat3(cam->GetViewMatrix())), "g_WorldToViewMatrixRotationOnly");
	SetFragmentUniform(glm::inverse(cam->GetViewMatrix()), "g_ViewToWorldMatrix");
	SetVertexUniform(cam->GetProjectionMatrix(), "g_ProjectionMatrix");
	SetFragmentUniform(glm::inverse(cam->GetProjectionMatrix()), "g_ProjectionMatrixInverse");

	SetFragmentUniform(cam->GetPosition(),	"g_CameraPosition");
	SetFragmentUniform(cam->GetNearPlane(),	"g_CameraNearPlane");
	SetFragmentUniform(cam->GetFarPlane(),	"g_CameraFarPlane");
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
	SetFragmentUniform(normalsEnabled,	"g_DebugNormalsEnabled");
	SetFragmentUniform(linearizeDepth, "g_DebugLinearizeDepth");
	//SetFragmentUniform(gammaCorrection, "g_GammaCorrection");

}

void Renderer::DrawModel(Entity* entity, Model* model)
{
	for (int i = 0; i < model->m_meshes.size(); ++i)
	{
		SetVertexUniform(entity->m_ModelToWorldMatrix, "g_ModelToWorldMatrix");

		Material* material = model->GetMaterials()[i];

		if (!material->GetEnabled())
		{
			continue;
		}

		DrawMesh(model->m_meshes[i], material);
	}
}

GLuint slots[] =
{
	GL_TEXTURE0,
	GL_TEXTURE1,
	GL_TEXTURE2,
	GL_TEXTURE3,
	GL_TEXTURE4
};

void Renderer::Bind2DTexture(int slot, GLuint HWObject)
{
	GLuint theSlot = slots[slot];
	glActiveTexture(theSlot);
	glBindTexture(GL_TEXTURE_2D, HWObject);
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

void Renderer::DrawMesh(Mesh* mesh)
{
	glBindVertexArray(mesh->m_VAO);

	if (mesh->m_usesIndexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_VBOs.glidIndexBuffer);

		GL::DrawElements(GL_TRIANGLES, mesh->m_buffers.indexBuffer.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(mesh->m_renderingMode, 0, mesh->m_buffers.vertexPositions.size());
		FLUSH_ERRORS();
	}

	g_EngineStats->AddToNumDrawCalls(1);
}


void Renderer::DrawMesh(Mesh* mesh, Material* material)
{
  glBindVertexArray(mesh->m_VAO);

  SetMaterialUniforms(material);

  if (mesh->m_wireframeRendering)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (mesh->m_pointRendering)
  {
    glEnable(GL_PROGRAM_POINT_SIZE);
  }

  DrawMesh(mesh);

  if (mesh->m_pointRendering)
  {
    glDisable(GL_PROGRAM_POINT_SIZE);
  }

  if (mesh->m_wireframeRendering)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  //FreeBoundTexture();
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

	SetPostEffectsUniforms();
	SetDebugUniforms();

	SetFragmentUniform(1, "g_SplatMapTexture");
	Bind2DTexture(1, g_Editor->m_SplatMapTexture->GetHWObject());


	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	Material* terrainMaterial = nullptr;
	Entity* terrainEntity = nullptr;
	for (Entity* entity : m_World->GetEntities())
	{
		// todo : fix this awful hack to get terrain material

		Material* cmpMaterial = entity->GetModel()->GetMaterials()[0];
		if (cmpMaterial && cmpMaterial->GetShaderFamily() == ShaderFamily_Terrain)
		{
			terrainEntity = entity;
			terrainMaterial = cmpMaterial;
			break;
		}
	}

	//RenderEntities(ShaderFamily_Terrain, m_World->GetEntities());

	SetVertexUniform(terrainEntity->m_ModelToWorldMatrix, "g_ModelToWorldMatrix");

	DrawMesh(g_DefaultObjects->DefaultTerrainMesh, terrainMaterial);
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
	m_ShadowCamera->updateVPMatrix();

	SetViewUniforms(m_ShadowCamera);
	SetPostEffectsUniforms();
	SetDebugUniforms();

	RenderEntities(m_World->GetEntities());

	BindFramebuffer(FramebufferType_Default);
}

void Renderer::RenderOpaque()
{
	if (!m_DisplayOptions->GetRenderOpaque())
	{
		return;
	}

	SetPipeline(m_Pipelines[ShaderFamily_Basic]);

	SetLightingUniforms();
	SetViewUniforms(m_DebugCamera);
	SetShadowMapUniforms(m_ShadowCamera);
	SetSkyUniforms(3);

	SetPostEffectsUniforms();
	SetDebugUniforms();

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	RenderEntities(ShaderFamily_Basic, m_World->GetEntities());

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
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetPipeline(pPipelineScreenSpaceTexture);

	SetFragmentUniform(0, "g_Texture");
	//Bind2DTexture(0, m_Framebuffers[FramebufferType_ShadowMap]->GetTexture());
	//Bind2DTexture(0, m_Framebuffers[FramebufferType_Picking]->GetColorTexture());
	Bind2DTexture(0, m_Framebuffers[FramebufferType_Picking]->GetColorTexture(0));
	//Bind2DTexture(0, m_World->m_Entities[0]->getModel()->m_materialList[3]->GetDiffuseTexture()->GetHWObject());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Renderer::RenderHDRFramebuffer()
{
	BindFramebuffer(FramebufferType_Default);
	GL::SetViewport(windowWidth, windowHeight);
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


	SetComputeUniform(glm::vec2(windowWidth, windowHeight), "g_WindowSize");
	SetComputeUniform(mouseClickPosition, "g_MousePosition");

	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

	float output;
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
	
	for (Entity* entity : m_World->GetEntities())
	{
		SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
		SetVertexUniform(entity->m_ModelToWorldMatrix, "g_ModelToWorldMatrix");
	
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
			SetVertexUniform(entity->m_ModelToWorldMatrix, "g_ModelToWorldMatrix");

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
	g_EngineStats->ResetFrame();

	m_World = world;
	GL::g_CheckGLErrors = m_DisplayOptions->GetOutputGLErrors();

	m_DebugCamera->handlePresetNewFrame(pWindow);

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

}

void Renderer::ApplyEmissiveGlow()
{
	if (!m_PostEffects->GetEmissiveGlowEnabled())
	{
		return;
	}

	SetPipeline(pPipelineBlur);


	bool horizontal = true;
	int amount = 10;
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
	GL::SetViewport(windowWidth, windowHeight);
	GL::ClearColorAndDepthBuffers();
}
//
//void Renderer::RenderSquare(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color)
//{
//	SetPipeline(pP);
//
//	for (Model* model : m_DebugModels)
//	{
//		for (int i = 0; i < model->m_meshes.size(); ++i)
//		{
//			SetVertexUniform(glm::mat4(1.0), "g_ModelToWorldMatrix");
//
//			Material* material = model->GetMaterials()[i];
//
//			SetViewUniforms(m_DebugCamera);
//
//			DrawMesh(model->m_meshes[i], material);
//		}
//	}
//
//}

void Renderer::RenderDebugModels()
{
	SetPipeline(pPipelineLines);

	for (Model* model : m_DebugModels)
	{
		for (int i = 0; i < model->m_meshes.size(); ++i)
		{
			SetVertexUniform(glm::mat4(1.0), "g_ModelToWorldMatrix");

			Material* material = model->GetMaterials()[i];

			SetViewUniforms(m_DebugCamera);

			DrawMesh(model->m_meshes[i], material);
		}
	}

	for (Sector* sector : m_World->sectors)
	{
		AABB bb;
		const float& worldScale = g_EngineSettings->GetWorldScale();

		const glm::vec2& cornerPos = sector->GetWorldPosition();
		glm::vec2 centerPos = cornerPos + glm::vec2(worldScale) / 2.f;
		bb.SetOffset(glm::vec3(centerPos, 0));
		//bb.SetOffset(glm::vec3(0, 0, 0));

		bb.SetScale(glm::vec3(worldScale,  worldScale, 1));

		RenderAABB(bb);
	}
}


void Renderer::AfterWorldRender()
{
	RenderDebugModels();

	ApplyEmissiveGlow();
}

void Renderer::FinishFrame()
{
	HandleScreenshot();
	
	glfwSwapBuffers(pWindow);
}

void Renderer::RenderAABB(AABB& aabb)
{
	GL::SetCapability(GL::Capabilities::Blend, true);
	SetPipeline(pPipelineUVEdges);

	glm::mat4 modelToWorldMatrix = mat4(1.0f);

	glm::vec3 translation = aabb.GetOffset();
	modelToWorldMatrix = glm::translate(modelToWorldMatrix, translation);
	modelToWorldMatrix = glm::scale(modelToWorldMatrix, aabb.GetScale());

	SetVertexUniform(modelToWorldMatrix, "g_ModelToWorldMatrix");
	
	SetViewUniforms(m_DebugCamera);

	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);
	GL::SetCapability(GL::Capabilities::Blend, false);
}

void Renderer::DrawBoundingBox(BoundingBoxComponent* bb)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetPipeline(pPipelineUVEdges);

	SetViewUniforms(m_DebugCamera);
	SetVertexUniform(bb->GetModelToWorldMatrix(), "g_ModelToWorldMatrix");

	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	glDisable(GL_BLEND);
}

void Renderer::DrawManipulator(Entity* entity)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetPipeline(pPipelineSolidColor);

	SetViewUniforms(m_DebugCamera);
	SetVertexUniform(entity->m_ModelToWorldMatrix, "g_ModelToWorldMatrix");
	SetFragmentUniform(glm::vec3(1,0,0), "g_InputColor");


	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	glDisable(GL_BLEND);
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

	GL::SetViewport(windowWidth, windowHeight);

	//RenderImgui();

	RenderFPS();

	RenderTextureOverlay();
}

void Renderer::render(World* world)
{
	BeginFrame(world);

	//g_Editor->RenderPicking();

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

void Renderer::RenderEntities(ShaderFamily family, std::vector<Entity*> entities)
{
	for (Entity* entity : entities)
	{
		if (!entity->GetVisible())
		{
			continue;
		}

		if (entity->GetModel()->GetMaterials()[0]->GetShaderFamily() != family)
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

void Renderer::RenderFPS()
{
	if (!m_DisplayOptions->GetShowFPS())
	{
		return;
	}

	std::stringstream fpsString;
	float smoothing = 0.99; // larger=more smoothing
	fps = (fps * smoothing) + (1.f/dt * (1.0-smoothing));
	fpsString << 1.f / (fps / 1000.f);
	fpsString << " ms/frame";
	RenderText(fpsString.str(), 695.0f, 775.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f), false);

	float new_time = glfwGetTime();
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

	SetFragmentUniform(2.f * glm::vec2(windowWidth, windowHeight), "windowSize");

	SetSkyUniforms(0);

	SetViewUniforms(m_DebugCamera);

	Mesh* mesh = g_DefaultObjects->DefaultCubeMesh;
	DrawMesh(mesh);

	GL::ClearDepthBuffer();
}


void Renderer::RenderText(Text t)
{
     glEnable(GL_CULL_FACE);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

   glDisable(GL_BLEND);
   glDisable(GL_CULL_FACE);

}

void Renderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, bool projected)
{
	GL::SetCapability(GL::Capabilities::Blend,		true);
	GL::SetCapability(GL::Capabilities::CullFace,	true);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Activate corresponding render state
   pPipelineText->usePipeline();
   GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
   glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
   glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
   if (projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(m_DebugCamera->GetViewProjectionMatrix()));
   else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f)));

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



void Renderer::outputGLError(std::string func, int line)
{
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) 
  {
    string error;

    switch(err) 
    {
      case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
      case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
      case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
      case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
    }
    std::stringstream ss;

    ss << func << " : " << line;
    PRINTSTRING(ss.str());
    PRINTSTRING(error.c_str());
    err=glGetError();    
  }
}
