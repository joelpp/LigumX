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
#include "Settings.h"
#include "PostEffects.h"
#include "Sunlight.h"
#include "GL.h"
#include "Framebuffer.h"

#pragma region  CLASS_SOURCE Renderer
#include "Renderer.h"
#include <cstddef>
const ClassPropertyData Renderer::g_Properties[] = 
{
{ "DisplayOptions", offsetof(Renderer, m_DisplayOptions), 0, LXType_DisplayOptions, true, LXType_None,  }, 
{ "PostEffects", offsetof(Renderer, m_PostEffects), 0, LXType_PostEffects, true, LXType_None,  }, 
{ "MouseClickPosition", offsetof(Renderer, m_MouseClickPosition), 0, LXType_glmvec2, false, LXType_None,  }, 
};

#pragma endregion  CLASS_SOURCE Renderer
using namespace glm;
using namespace std;

#define CHAR_BUFFER_SIZE 64

#define FT_SUCCESS 0


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

	glfwSetWindowPos(pWindow, 700, 200);
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

	m_Framebuffers[FramebufferType_ShadowMap] = new Framebuffer("Shadow Map Buffer", SHADOW_WIDTH, SHADOW_HEIGHT, GL::PixelFormat_DepthComponent, GL::PixelType_Float);
	m_Framebuffers[FramebufferType_ShadowMap]->m_Attachment = GL_DEPTH_ATTACHMENT;
	m_Framebuffers[FramebufferType_ShadowMap]->Initialize();

	m_Framebuffers[FramebufferType_Picking] = new Framebuffer("Picking Buffer", pickingBufferSize, pickingBufferSize, GL::PixelFormat_RGB, GL::PixelType_uByte);
	m_Framebuffers[FramebufferType_Picking]->m_Attachment = GL_COLOR_ATTACHMENT0;
	m_Framebuffers[FramebufferType_Picking]->Initialize();

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glEnable(GL_CULL_FACE);

	glGenBuffers(1, &SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 12 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

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
	FLUSH_ERRORS();
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

void Renderer::Initialize()
{
	// TODO : add default constructors D: and serialization for gen files...
	m_DisplayOptions = new DisplayOptions();

	m_DisplayOptions->SetDrawTerrain(true);
	m_DisplayOptions->SetDrawSky(true);
	m_DisplayOptions->SetUseLighting(true);
	m_DisplayOptions->SetShowDiffuse(true);
	m_DisplayOptions->SetShowSpecular(true);
	m_DisplayOptions->SetShowNormals(false);
	m_DisplayOptions->SetShowAmbient(true);
	m_DisplayOptions->SetShowFPS(true);
	m_DisplayOptions->SetRenderOpaque(true);

	m_PostEffects = new PostEffects();
	m_PostEffects->SetGammaExponent(2.2f);

    windowWidth = 800;
    windowHeight = 800;
    windowTitle = "LigumX";
    fps = 300;
    dt = 0.1;

	InitGL();

	InitFreetype();

	// todo : put this in startup options?
    std::string texturePath = "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/";

	ImGui_ImplGlfwGL3_Init(pWindow, true);
   
	m_TestLight.m_DiffuseColor = glm::vec3(0.88, 0.56, 0.76);
	m_TestLight.m_SpecularColor = glm::vec3(0.56, 0.88, 0.76);
	m_TestLight.m_AmbientColor = glm::vec3(0.1, 0.1, 0.1);
	m_TestLight.m_Position = glm::vec3(-0.25, 7, 13);

	m_ShadowCamera = new Camera();
	m_ShadowCamera->SetProjectionType(ProjectionType_Orthographic);
	//m_ShadowCamera->SetProjectionType(ProjectionType_Perspective);
	//-16.13, -5.9, 20.2
	//-0.47, -0.81, 0.34
	m_ShadowCamera->SetPosition(glm::vec3(-16.13, -5.9, 20.2));
	//m_ShadowCamera->translateTo(glm::vec3(-16.13, -5.9, 20.2));
	m_ShadowCamera->frontVec = glm::vec3(-0.47, -0.81, 0.34);
	m_ShadowCamera->rightVec = normalize(glm::cross(glm::vec3(0,0,1), m_ShadowCamera->frontVec));
	m_ShadowCamera->upVec = glm::cross(m_ShadowCamera->frontVec, m_ShadowCamera->rightVec);
	m_ShadowCamera->translateTo(m_ShadowCamera->GetPosition() - m_ShadowCamera->frontVec * 10.f);
	m_ShadowCamera->updateVPMatrix();
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

void Renderer::SetLightingUniforms()
{
	SetFragmentUniform(m_TestLight.m_Position,		"g_PointLight.m_Position");
	SetFragmentUniform(m_TestLight.m_DiffuseColor,  "g_PointLight.m_DiffuseColor");
	SetFragmentUniform(m_TestLight.m_AmbientColor,  "g_PointLight.m_AmbientColor");
	SetFragmentUniform(m_TestLight.m_SpecularColor, "g_PointLight.m_SpecularColor");
	
	SetFragmentUniform(m_TestLight.m_Position,		"g_DirectionalLight.m_Direction");
	SetFragmentUniform(m_TestLight.m_DiffuseColor,	"g_DirectionalLight.m_DiffuseColor");
	SetFragmentUniform(m_TestLight.m_AmbientColor,	"g_DirectionalLight.m_AmbientColor");
	SetFragmentUniform(m_TestLight.m_SpecularColor, "g_DirectionalLight.m_SpecularColor");
}

void Renderer::SetMaterialUniforms(Material* material)
{
	SetFragmentUniform(material->GetAmbientColor(),		"g_Material.ambient");
	SetFragmentUniform(material->GetDiffuseColor(),		"g_Material.diffuse");
	SetFragmentUniform(material->GetSpecularColor(),	"g_Material.specular");
	SetFragmentUniform(material->GetShininess(),		"g_Material.shininess");

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
}

void Renderer::SetPostEffectsUniforms()
{
	int gamma = m_PostEffects->GetGammaCorrectionEnabled() ? 1 : 0;
	SetFragmentUniform(gamma, "g_GammaCorrectionEnabled");
	float val = m_PostEffects->GetGammaExponent();
	SetFragmentUniform(val, "g_GammaCorrectionExponent");

}

void Renderer::SetShadowMapUniforms(Camera* cam)
{
	SetFragmentUniform(2, "g_DepthMapTexture");
	Bind2DTexture(2, m_Framebuffers[FramebufferType_ShadowMap]->GetTexture());

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
	GL_TEXTURE2
};

void Renderer::Bind2DTexture(int slot, GLuint HWObject)
{
	GLuint theSlot = slots[slot];
	glActiveTexture(theSlot);
	glBindTexture(GL_TEXTURE_2D, HWObject);
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
		glDrawElements(GL_TRIANGLES, mesh->m_buffers.indexBuffer.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(mesh->m_renderingMode, 0, mesh->m_buffers.vertexPositions.size());
		FLUSH_ERRORS();
	}
}


void Renderer::DrawMesh(Mesh* mesh, Material* material)
{
  glBindVertexArray(mesh->m_VAO);

  FLUSH_ERRORS();

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

  FreeBoundTexture();
}

void Renderer::DrawTerrain()
{
	if (!m_DisplayOptions->GetDrawTerrain())
	{
		return;
	}

    std::vector<TerrainRenderingJob>& renderList = renderData->terrainRenderingJobs;
    
    if (!renderList.size())
    {
        return;
    }
    pPipelineBasicUV->usePipeline();
    ProgramPipeline* activePipeline = pPipelineBasicUV;
    
    Mesh* terrainMesh = renderData->terrainMesh();
    glProgramUniformMatrix4fv(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->GetViewProjectionMatrix()));
    glProgramUniformMatrix4fv(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "modelMatrix"), 1, false, value_ptr(glm::mat4(1.0)));
    
    glBindVertexArray(terrainMesh->m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainMesh->m_VBOs.glidIndexBuffer);

	if (m_DisplayOptions->GetWireframeRendering())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

    for (TerrainRenderingJob& job : renderList)
    {
        glProgramUniform2f(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "offset"), job.start[0], job.start[1]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, job.buffer);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureMap["grass"]->glidTexture);

        //glProgramUniform1i(activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram,glGetUniformLocation(activePipeline->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "g_DiffuseTexture"), textureMap["grass"]->glidTexture);

		glDrawElements(terrainMesh->m_renderingMode, terrainMesh->m_buffers.indexBuffer.size(), GL_UNSIGNED_INT, 0);
    }

	if (m_DisplayOptions->GetWireframeRendering())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

void Renderer::BeginImGUIWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name)
{
	ImGui::SetNextWindowSize(ImVec2(xSize, ySize), ImGuiSetCond_FirstUseEver);

	if (!ImGui::Begin(name, &open, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
}
void Renderer::EndImGUIWindow()
{
	ImGui::End();
}

//const char* toCharPtr(glm::vec3 value, const char* variableName)
//{
//	char textBuffer[CHAR_BUFFER_SIZE];
//	sprintf(textBuffer, );
//	return textBuffer;
//}

void Renderer::ShowVariableAsText(glm::vec3 variable, const char* variableName)
{
	ImGui::Text("%s : %f, %f, %f", variableName, variable.x, variable.y, variable.z);
}

void Renderer::ShowVariableAsText(glm::vec3* variable, const char* variableName)
{
	ImGui::Text("%s : %f, %f, %f", variableName, variable->x, variable->y, variable->z);
}

void Renderer::ShowGUIText(const char* text)
{
	ImGui::Text("%s", text);
}

void Renderer::ShowGUIText(const std::string& text)
{
	ImGui::Text("%s", text.c_str());
}

void Renderer::ShowGUIText(std::string* text)
{
	ImGui::Text("%s", text->c_str());
}

void Renderer::ShowGUIText(std::string* text, const char* variableName)
{
	ImGui::Text("%s : %s", variableName, text->c_str());
}

void Renderer::ShowGUIText(const std::string& text, const char* variableName)
{
	ImGui::Text("%s : %s", variableName, text.c_str());
}


void Renderer::ShowVariableAsText(float variable, const char* variableName)
{
	ImGui::Text("%s : %f", variableName, variable);
}

//template<typename T>
//T* GetPropertyValue(T* baseValue, int offset)
//{
//	return (T *)(##object) + data.m_Offset / sizeof(T);
//}

#define GetProperty(type, object, data) (##type *)(##object) + data.m_Offset / sizeof(##type)

#define GetPropertyChar(type, object, data) (((char*)(##object)) + data.m_Offset)

#define GetPropertyPtrChar(type, object, data) (char**)(((char*)(##object)) + data.m_Offset)

#define GetPropertyVector(type, object, data) (##type *)(((char*)(##object)) + data.m_Offset); /*/ (2 * sizeof(##type))*/;

#define GetPropertyPtr(type, object, data) *((##type **)(##object) + data.m_Offset / sizeof(##type*))

#define ShowCheckbox(data, object) ImGui::Checkbox(data.m_Name, ((bool*)##object + data.m_Offset));

#define ShowFloatSlider(data, object, min, max) ImGui::SliderFloat(data.m_Name, (float*)(##object)+ data.m_Offset / 4, min, max, "%.3f");

#define ShowVec3(data, object) ShowVariableAsText(GetProperty(glm::vec3, object, data), data.m_Name); 

#define ShowString(data, object) ShowGUIText((std::string*)(##object) + data.m_Offset / sizeof(std::string), data.m_Name); 

#define ShowModel(data, object) ShowGUIText( "hey" + (GetProperty(Model, ##object, data))->GetName(), data.m_Name); 


#define ShowPropertyGrid(objectPtr, displayname) \
{ \
    ImGui::PushID(#displayname); \
	if (ImGui::CollapsingHeader(##displayname)) \
	{ \
		if (objectPtr == nullptr) \
		{ \
			ShowGUIText("Object is null."); \
		} \
		else \
		{ \
			for (const ClassPropertyData& propertyData : objectPtr->g_Properties) \
			{ \
				switch (propertyData.m_Type) \
				{ \
				case LXType_bool: \
				{ \
					ShowCheckbox(propertyData, ##objectPtr); \
					break; \
				} \
				case LXType_float: \
				{ \
					ShowFloatSlider(propertyData, ##objectPtr, -10, 20); \
					break; \
				} \
				case LXType_glmvec3: \
				{ \
					ShowVec3(propertyData, ##objectPtr); \
					break; \
				} \
				case LXType_stdstring: \
				{ \
					ShowString(propertyData, ##objectPtr); \
					break; \
				} \
				case LXType_Model: \
				{ \
					/*ShowPropertyGridMacro(objectPtr->GetModel(), "Model")*/ \
					break; \
				} \
				default: \
				{ \
					break; \
				} \
				  \
				} \
			} \
		} \
	} \
    ImGui::PopID(); \
} \

//Model* model = GetProperty(Model, ##objectPtr, data); \
//					ShowPropertyGridMacro<Model>(model, "Model"); \

template<typename T>
void Renderer::ShowPropertyGridMacro(T* object, const char* name)
{
	ShowPropertyGrid(object, name);
}

void Renderer::ShowProperty(bool* value, const char* name)
{
	ImGui::Checkbox(name, value);
}


void Renderer::ShowProperty(float* value, const char* name, float min, float max)
{
	ImGui::SliderFloat(name, value, min, max);
}

void Renderer::ShowProperty(glm::vec3* value, const char* name)
{
	ShowVariableAsText(value, name);
}

void Renderer::ShowProperty(std::string* value, const char* name)
{
	ShowGUIText(value, name);
}

//void Renderer::ShowProperty(Material* value, const char* name)
//{
//	ShowGUIText(value, name);
//}

//template<typename T>
//void Renderer::ShowPropertyValue(T* object, const ClassPropertyData& propertyData)
//{
//
//}

void Renderer::ShowPropertyTemplate(const char* ptr, const char* name, const LXType& type, const LXType& associatedType)
{
	switch (type)
	{
		case LXType_bool:
		{
			ShowProperty((bool*) ptr, name);
			break;
		}
		case LXType_float:
		{
			ShowProperty((float*) ptr, name, -20, 20);
			break;
		}
		case LXType_glmvec3:
		{
			ShowProperty((glm::vec3*) ptr, name);
			break;
		}
		case LXType_stdstring:
		{
			ShowProperty((std::string*) ptr, name);
			break;
		}
		case LXType_Model:
		{
			ShowPropertyGridTemplate<Model>((Model*) ptr, name);
			break;
		}
		case LXType_Material:
		{
			ShowPropertyGridTemplate<Material>((Material*)ptr, name);
			break;
		}
		//case LXType_stdvector:
		//{
		//	std::vector<char*>* v = (std::vector<char*>*) ptr;

		//	for (int i = 0; i < v->size(); ++i)
		//	{
		//		char displayName[100];
		//		sprintf(displayName, "%s[%d]", name, i);
		//		ShowPropertyTemplate((*v)[i], displayName, associatedType, LXType_None);
		//	}
		//	break;
		//}
		default:
		{
			break;
		}
	}
}
/*
template<typename T>
void Renderer::ShowPropertyTemplate(T* object, const ClassPropertyData& propertyData)
{
	switch (propertyData.m_Type)
	{
		case LXType_bool:
		{
			char* ptr = GetPropertyChar(float, object, propertyData);

			ShowProperty((bool*)ptr, propertyData.m_Name);
			break;
		}
		//case LXType_float:
		//{
		//	ShowProperty(GetProperty(float, object, propertyData), propertyData.m_Name, -20.f, 20.f);
		//	break;
		//}
		//case LXType_glmvec3:
		//{
		//	ShowProperty(GetProperty(glm::vec3, object, propertyData), propertyData.m_Name);
		//	break;
		//}
		//case LXType_stdstring:
		//{
		//	ShowProperty(GetProperty(std::string, object, propertyData), propertyData.m_Name);
		//	break;
		//}
		//case LXType_Model:
		//{
		//	Model* model = GetPropertyPtr(Model, object, propertyData);
		//	ShowPropertyGridTemplate<Model>(model, propertyData.m_Name);

		//	//int i = 0;
		//	//// todo : refactor this into proper std vector support
		//	//for (Material* material : model->GetMaterials())
		//	//{
		//	//	ShowPropertyGridTemplate<Material>(material, ("Material #" + std::to_string(i++)).c_str());
		//	//}
		//	break;
		//}
		//case LXType_stdvector:
		//{
		//	std::vector<Material*>* v = GetPropertyVector(std::vector<Material*>, object, propertyData);

		//	for (int i = 0; i < v->size(); ++i)
		//	{
		//		Material* mat = (*v)[i];
		//		ShowPropertyGridTemplate<Material>(mat, ("Material #" + std::to_string(i)).c_str());
		//	}

		//	break;
		//}
		default:
		{
			break;
		}
	}
}
*/

template<typename T>
void Renderer::ShowGenericProperty(T* object, const ClassPropertyData& propertyData)
{
	char* ptr = 0;

	if (propertyData.IsAPointer)
	{
		ptr = *(GetPropertyPtrChar(float, object, propertyData));
	}
	else
	{
		ptr = GetPropertyChar(float, object, propertyData);
	}

	if (propertyData.m_Type == LXType_stdvector)
	{
		std::vector<char*>* v = (std::vector<char*>*) ptr;

		for (int i = 0; i < v->size(); ++i)
		{
			char displayName[100];
			sprintf(displayName, "%s[%d]", propertyData.m_Name, i);
			ShowPropertyTemplate((*v)[i], displayName, propertyData.m_AssociatedType, LXType_None);
		}
	}
	else
	{
		ShowPropertyTemplate(ptr, propertyData.m_Name, propertyData.m_Type, LXType_None);
	}
}

template<typename T> 
void Renderer::ShowPropertyGridTemplate(T* object, const char* name)
{
	ImGui::PushID(name); 

	if (ImGui::TreeNode(name))
	{
		if (object == nullptr)
		{
			ShowGUIText("Object is null.");
		} 
		else 
		{
			for (const ClassPropertyData& propertyData : object->g_Properties)
			{
				ShowGenericProperty(object, propertyData);
			} 
		} 
		ImGui::TreePop();
	}

    ImGui::PopID(); 
}


void Renderer::RenderGUI()
{
	ImGui_ImplGlfwGL3_NewFrame();
	if (m_ShowTestGUI)
	{
		ImGui::ShowTestWindow();
	}

	if (m_ShowGUI)
	{
		BeginImGUIWindow(1000, 700, 0, 0, "Settings");
		ImGui::Checkbox("Test GUI", &m_ShowTestGUI);

		// todo : have a mapping from lxtype to display names and such
		// or at least keep the display name as a generated static string in each gen'd class
		//ShowPropertyGridMacro<DisplayOptions>(m_DisplayOptions,	"Display options");

		//ShowPropertyGridMacro<DisplayOptions>(m_DisplayOptions, "Display options");

		//ShowPropertyGrid(m_PostEffects,		"Post Effects");
		//ShowPropertyGrid(camera,			"Camera");
		//ShowPropertyGrid(m_ShadowCamera,	"Shadow Camera");
		//ShowPropertyGrid(m_World->GetSunLight(), "SunLight");
		//ShowPropertyGridMacro<Entity>(m_PickedEntity, "Entity");

		ShowPropertyGridTemplate<DisplayOptions>(m_DisplayOptions, "Display options");
		ShowPropertyGridTemplate<PostEffects>(m_PostEffects, "Post Effects");
		ShowPropertyGridTemplate<Camera>(camera, "Camera");
		ShowPropertyGridTemplate<Entity>(m_PickedEntity, "Entity");


		if (ImGui::CollapsingHeader("Light"))
		{
			ImGui::ColorEdit3("Diffuse", &(m_TestLight.m_DiffuseColor[0]));
			ImGui::ColorEdit3("Specular", &(m_TestLight.m_SpecularColor[0]));
			ImGui::ColorEdit3("Ambient", &(m_TestLight.m_AmbientColor[0]));
			ImGui::SliderFloat3("Position", &(m_TestLight.m_Position[0]), -30.f, 30.f);
		}
		ShowVariableAsText(m_PickedColor[0], "Picking");

		EndImGUIWindow();
	}

	ImGui::Render();
}

void Renderer::RenderShadowMap()
{
	GL::SetViewport(SHADOW_WIDTH, SHADOW_HEIGHT);
	GL::BindFramebuffer(m_Framebuffers[FramebufferType_ShadowMap]->GetHWObject());
	GL::ClearDepthBuffer();

	SetPipeline(pPipelineShadowMap);

	glm::vec3 pos = glm::normalize(m_World->GetSunLight()->GetSunDirection());

	m_TestLight.m_Position = pos;
	SetLightingUniforms();

	m_ShadowCamera->SetPosition(glm::vec3(0, 20, 1) + pos * 15.f);
	m_ShadowCamera->frontVec = pos;
	m_ShadowCamera->rightVec = normalize(glm::cross(glm::vec3(0, 0, 1), m_ShadowCamera->frontVec));
	m_ShadowCamera->upVec = normalize(glm::cross(m_ShadowCamera->frontVec, m_ShadowCamera->rightVec));
	m_ShadowCamera->updateVPMatrix();
	SetViewUniforms(m_ShadowCamera);
	SetPostEffectsUniforms();
	SetDebugUniforms();

	RenderEntities(m_World->m_Entities);

	GL::BindFramebuffer(0);
}

void Renderer::RenderOpaque()
{
	if (!m_DisplayOptions->GetRenderOpaque())
	{
		return;
	}

	SetPipeline(pPipelineBasic);

	SetLightingUniforms();
	SetViewUniforms(camera);
	//SetViewUniforms(m_ShadowCamera);
	SetShadowMapUniforms(m_ShadowCamera);

	SetPostEffectsUniforms();
	SetDebugUniforms();

	RenderEntities(m_World->m_Entities);
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
	Bind2DTexture(0, m_Framebuffers[FramebufferType_Picking]->GetTexture());
	//Bind2DTexture(0, m_World->m_Entities[0]->getModel()->m_materialList[3]->GetDiffuseTexture()->GetHWObject());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool fuzzyEquals(float a, float b, float tolerance)
{
	return fabs(a - b) < tolerance;
}

void Renderer::RenderPicking()
{
	GL::SetViewport(pickingBufferSize, pickingBufferSize);

	SetPipeline(pPipelinePicking);

	GL::BindFramebuffer(m_Framebuffers[FramebufferType_Picking]->GetHWObject());
	GL::ClearColorBuffer();

	SetViewUniforms(camera);

	for (Entity* entity : m_World->m_Entities)
	{
		SetFragmentUniform(entity->GetPickingID(), "g_PickingID");
		SetVertexUniform(entity->m_ModelToWorldMatrix, "g_ModelToWorldMatrix");

		for (int i = 0; i < entity->GetModel()->m_meshes.size(); ++i)
		{
			DrawMesh(entity->GetModel()->m_meshes[i]);
		}
	}
	
	GL::BindFramebuffer(0);

	SetPipeline(pPipelinePickingCompute);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	SetComputeUniform(1, "g_PickingBuffer");
	Bind2DTexture(1, m_Framebuffers[FramebufferType_Picking]->GetHWObject());
	
	SetComputeUniform(m_MouseClickPosition, "g_MousePosition");

	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

	float output;
	GLfloat *ptr;
	ptr = (GLfloat *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	output = *ptr;

	m_PickedColor[0] = output;

	if (m_LastMouseClickPosition != m_MouseClickPosition)
	{
		// search for picked entity
		for (Entity* entity : m_World->m_Entities)
		{
			// todo : proper int rendertarget; how does depth work then? do we care?
			if (fuzzyEquals(output, entity->GetPickingID(), 0.005f))
			{
				m_PickedEntity = entity;
				break;
			}
		}

		// Update last click position
		m_LastMouseClickPosition = m_MouseClickPosition;
	}


	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}


/**
 * [Renderer::render description]
 * @param camera [description]
 */
void Renderer::render(World* world)
{
	m_World = world;
	RenderPicking();
	RenderShadowMap();

	GL::SetViewport(windowWidth, windowHeight);
	GL::BindFramebuffer(0);
	GL::ClearColorAndDepthBuffers();

	RenderSky(world->GetSunLight());

	//DrawTerrain();

	RenderOpaque();

	RenderGUI();

	HandleScreenshot();

	RenderFPS();

	RenderTextureOverlay();



	FLUSH_ERRORS();
}

void Renderer::RenderEntities(std::vector<Entity*> entities)
{
	for (Entity* entity : entities)
	{
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

void Renderer::RenderSky(SunLight* sunLight)
{
	if (!m_DisplayOptions->GetDrawSky())
	{
		return;
	}

	SetPipeline(pPipelineEnvmap);

	GLuint fragProg = pPipelineEnvmap->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
	float pi = 3.141592654f;
	glm::vec2 viewAngles = glm::vec2(camera->totalViewAngleY*pi, camera->aspectRatio*camera->totalViewAngleY*glm::pi<float>()) / 180.0f;
	SetFragmentUniform(viewAngles, "viewAngles");

	SetFragmentUniform(2.f * glm::vec2(windowWidth, windowHeight), "windowSize");
	SetFragmentUniform(sunLight->GetOrientation(), "sunOrientation");
	SetFragmentUniform(sunLight->GetTime(), "sunTime");
	SetFragmentUniform(-camera->frontVec, "viewDir");
	SetFragmentUniform(camera->rightVec, "viewRight");
	SetFragmentUniform(camera->upVec, "viewUp");
	SetFragmentUniform(camera->GetNearPlane() ,"viewNear");

	SetViewUniforms(camera);

	Mesh* mesh = g_DefaultMeshes->DefaultCubeMesh;
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
   if (t.projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(camera->GetViewProjectionMatrix()));
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
   glEnable(GL_CULL_FACE);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // Activate corresponding render state
   pPipelineText->usePipeline();
   GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
   glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
   glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
   if (projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(camera->GetViewProjectionMatrix()));
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
   glBindTexture(GL_TEXTURE_2D, 0);

   glDisable(GL_BLEND);
   glDisable(GL_CULL_FACE);

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
