#ifndef RENDERER
#define RENDERER


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Freetype/ft2build.h"
#include FT_FREETYPE_H

#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>

#include "ProgramPipeline.h"
#include "texture.h"
#include "camera.h"
#include "OSMElement.h"
#include "AABB.h"
#include "Material.h"
#include "GL.h"
#include "GLFW/glfw3.h"

class MainWindow;
class SectorGraphicalData;


#pragma region  FORWARD_DECLARATIONS Renderer
#include "LXSystem.h"
class Serializer2;

class Renderer;
class DisplayOptions;
class PostEffects;
class Camera;


#pragma endregion  FORWARD_DECLARATIONS Renderer

class Framebuffer;
class SunLight;
class BoundingBoxComponent;

class Renderer;
extern Renderer* g_Instance;


enum FramebufferType
{
	FramebufferType_Default = 0,
	FramebufferType_MainColorBuffer,
	FramebufferType_ShadowMap,
	FramebufferType_Picking,
	FramebufferType_PingPong0,
	FramebufferType_PingPong1,
	FramebufferType_GBuffer,
	NBFramebuffers
};

class Material;
class Model;
class Mesh;
class RenderDataManager;
class World;
class Entity;
class DisplayOptions;

struct Light
{
	glm::vec3 m_Position;
	glm::vec3 m_DiffuseColor;
	glm::vec3 m_AmbientColor;
	glm::vec3 m_SpecularColor;
};


struct Text {
    std::string text;
    glm::vec3 position;
    bool projected;
    float scale;

    // std::vector<characterQuadVertices> quads;
};



#pragma region  HEADER Renderer
class Renderer : public LXObject
{
public:
static const int ClassID = 1496194462;
static const LXType Type = LXType_Renderer;
static constexpr const char* ClassName = "Renderer";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

DisplayOptions*& GetDisplayOptions() { return m_DisplayOptions; }; 
void SetDisplayOptions(DisplayOptions* value) { m_DisplayOptions = value; }; 
PostEffects*& GetPostEffects() { return m_PostEffects; }; 
void SetPostEffects(PostEffects* value) { m_PostEffects = value; }; 
Camera*& GetDebugCamera() { return m_DebugCamera; }; 
void SetDebugCamera(Camera* value) { m_DebugCamera = value; }; 
private:
DisplayOptions* m_DisplayOptions = nullptr;
PostEffects* m_PostEffects = nullptr;
Camera* m_DebugCamera = nullptr;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_RendererPIDX
{
PIDX_DisplayOptions,
PIDX_PostEffects,
PIDX_DebugCamera,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
void PostSerialization(bool writing, bool success);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER Renderer
public:
	const unsigned int SHADOW_WIDTH = 800, SHADOW_HEIGHT = 800;

	Renderer::Renderer();

	World* m_World;

    void Initialize();
	void InitGL();
	void InitFramebuffers();
	void InitFreetype();

	void Shutdown();

	glm::ivec2 GetWindowSize();

    // subfunctions
    void InitPipelines();
    void init_pipelines_buildingSides();
    void init_pipelines_filledBuildings();
    void init_pipelines_text();

    void Render(World* world);
    void RenderText(Text t);
    void RenderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color, bool projected);
    void DrawMesh(Mesh* mesh, Material* material);
	void DrawMesh(Mesh* mesh);
    void DrawModel(Entity* entity, Model* model);
    void RenderSky();
    void RenderFPS();
	void RenderMessages();

    void RenderTerrain();
	void RenderEntities(ShaderFamily family, const std::vector<Entity*>& entities);
	void RenderEntities(std::vector<Entity*> entities);
	void RenderDebugModels();
	void RenderDebugModel(Model* model, const glm::mat4& modelToWorld, ProgramPipeline* programPipeline);
	void RenderDebugWays(Model* model, const glm::mat4& modelToWorld, ProgramPipeline* programPipeline, const std::vector<int>& displayFlags, const std::vector<glm::vec3>& wayDebugColors, int selectedWay);
	
	void RenderGrid();
	void RenderAxisGizmo();


	void BeforeWorldRender();
	void AfterWorldRender();

	void BeginFrame(World* world);
	void FinishFrame();

	void RenderShadowMap();
	void RenderOpaque();
	void RenderTextureOverlay();

	void RenderPickingBuffer(bool debugEntities);
	void GetPickingData(glm::vec2 mouseClickPosition, glm::vec4& pickingData);
	void RenderEntityBB(Entity* entity);

	void ApplyEmissiveGlow();

	void RenderHDRFramebuffer();

	void BindFramebuffer(FramebufferType buffer);
	void BindCubemap(int slot, GLuint HWObject);
	void Bind2DTexture(int slot, GLuint HWObject);
	void Bind2DTexture(int slot, Texture* texture);
	void FreeBoundTexture(int slot);
	void FreeBoundTexture();

	void SetUniform(int value, const char* name, GLuint location);
	void SetUniform(float value, const char* name, GLuint location);

	void SetUniform(glm::vec3& value, const char* name, GLuint location);
	void SetUniform(const glm::vec3& value, const char* name, GLuint location);

	void SetUniform(glm::vec2& value, const char* name, GLuint location);
	void SetUniform(const glm::vec2& value, const char* name, GLuint location);

	void SetUniform(glm::mat4x4& value, const char* name, GLuint location);
	void SetUniform(const glm::mat4x4& value, const char* name, GLuint location);

	void SetUniform(const glm::ivec2& value, const char* name, GLuint location);


	void SetVertexUniform(int value, const char* name);
	void SetFragmentUniform(int value, const char* name);
	void SetFragmentUniform(float value, const char* name);
	void SetFragmentUniform(bool value, const char* name);
	void SetFragmentUniform(const std::vector<int>& values, const char* name);
	void SetFragmentUniformArray(const std::vector<glm::vec3>& values, const char* name);

	void SetComputeUniform(int value, const char* name);

	template<typename T>
	void SetComputeUniform(const T& value, const char* name);

	template<typename T>
	void SetVertexUniform(T& value, const char* name);
	template<typename T>
	void SetFragmentUniform(T& value, const char* name);

	void  SetFragmentUniform(glm::vec3& value, const char* name, GLuint location);

	void SetPipeline(ProgramPipeline* pipeline, bool force);
	void SetPipeline(ShaderFamily family);
	void SetPipeline(ProgramPipeline* pipeline);
	void SetLightingUniforms();
	void SetWorldGridUniforms();
	void SetViewUniforms(Camera* cam);
	void SetShadowMapUniforms(Camera* cam);
	void SetDebugUniforms();
	void SetMaterialUniforms(Material* material);
	void SetPostEffectsUniforms();
	void SetSkyUniforms(int skyCubemapSlot);
	void SetDisplayModeUniforms();

	Camera *m_ShadowCamera;
    bool drawBuildingSides;
    bool saveScreenshot;

    float dt, curr_time, fps;

    // window params
	MainWindow* m_Window;


	Framebuffer* GetFramebuffer(FramebufferType type) { return m_Framebuffers[type]; }
	Framebuffer* m_Framebuffers[NBFramebuffers];

	// textures
    std::unordered_map<std::string, Texture*> textureMap;

	std::vector<ProgramPipeline*> m_Pipelines;

    // shaders
	std::vector<ProgramPipeline*> m_Shaders;
    ProgramPipeline* pPipelineLines;
    ProgramPipeline* pPipelineBuildings;
    ProgramPipeline* pPipelineBuildingSides;
    ProgramPipeline* pPipelineGround;
	ProgramPipeline* pPipelinePicking;
	ProgramPipeline* pPipelinePickingCompute;
	ProgramPipeline* pPipelineBlur;
	//ProgramPipeline* pPipelineEnvmap;
	ProgramPipeline* pPipelineScreenSpaceTexture;
    ProgramPipeline* pPipelineText;
    ProgramPipeline* pPipelineNodes;
    ProgramPipeline* pPipelineBasic;
    ProgramPipeline* pPipelineBasicUV;
	ProgramPipeline* pPipelineShadowMap;
	ProgramPipeline* pPipelineUVEdges;
	ProgramPipeline* pPipelineSolidColor;
	ProgramPipeline* pPipelineGrid;
	ProgramPipeline* pPipelineAxisGizmo;
	ProgramPipeline* activePipeline;

    // need to keep those for swapping
    ProgramPipeline::ShaderProgram* pGeometryShader1;
    ProgramPipeline::ShaderProgram* pGeometryShader2;
    ProgramPipeline::ShaderProgram* pGeometryShader3;
    ProgramPipeline::ShaderProgram* pFragmentShader1;
    ProgramPipeline::ShaderProgram* pFragmentShader2;
    ProgramPipeline::ShaderProgram* pFragmentShader3;

	void AddToDebugModels(Model* model);
    std::vector<Model*> m_DebugModels;

    // VBOs
    GLuint glidNodesPositions;
    GLuint glidWaysPositions;
    GLuint glidWaysColors;
    GLuint glidBufferRoadsPositions;
    GLuint glidScreenQuadPositions;
    GLuint glidScreenQuadTexCoords;
    GLuint glidBufferBuildingTriangleVertices;
    GLuint glidBufferBuildingLines;
    GLuint glidBufferBuildingLoopLengths;
    GLuint glidGroundTrianglePositions;
    GLuint glidGroundTriangleUVs;
    GLuint glidGroundTriangleTextureIDs;
    GLuint textVBO;
    GLuint textUvsVBO;
	GLuint SSBO;

    std::unordered_map<OSMElement::ElementType, GLuint > glidWaysNodesPositions;
    std::map<OSMElement::ElementType, bool> displayElementType;
	std::unordered_map<OSMElement::ElementType, glm::vec3> typeColorMap;
    std::unordered_map<OSMElement::ElementType, int> numberOfVerticesToDrawPerElement;
    // Qties of primitives to draw
    unsigned int nbBuildingTriangles;
    unsigned int nbBuildingLines;
    unsigned int nbRoads;
    unsigned int nbWaysVertices;
    unsigned int nbGroundVertices;
    unsigned int nbNodes;

    std::vector<GLint> firstVertexForEachRoad;
    std::vector<GLsizei> nbVerticesForEachRoad;


    //text renderign stuff
    struct Character 
    {
        GLuint     TextureID;  // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        GLuint     Advance;    // Offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;
    std::vector<Text> texts;

	Light m_TestLight[8];

    template<typename T> static void createGLBuffer(GLuint &bufferName, std::vector<T> bufferData) 
    {
//#ifdef __APPLE__
		glGenBuffers(1, &bufferName);
        glBindBuffer(GL_ARRAY_BUFFER, bufferName);
		glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(T), bufferData.data(), GL_DYNAMIC_DRAW /*GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                             GL_MAP_WRITE_BIT*/);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, bufferData.size() * sizeof(T), bufferData.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

#if 0
        glCreateBuffers(1, &bufferName);
        glNamedBufferStorage(bufferName, bufferData.size() * sizeof(T), // nbWaysNodes * vec2 * float
                             NULL,
                             GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                             GL_MAP_WRITE_BIT);
        glNamedBufferSubData(bufferName, 0, bufferData.size() * sizeof(T), bufferData.data());
#endif
    }


    template<typename T> static void createGLBuffer(GLenum target, GLuint &bufferName, std::vector<T> bufferData) 
    {
        glGenBuffers(1, &bufferName);
        glBindBuffer(target, bufferName);
        glBufferData(target, bufferData.size() * sizeof(T), 0, GL_DYNAMIC_DRAW /*GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                             GL_MAP_WRITE_BIT*/);
        glBufferSubData(target, 0, bufferData.size() * sizeof(T), bufferData.data());
        glBindBuffer(target, 0);
}


    template<typename T> 
    void updateGLBuffer(GLuint &bufferName, std::vector<T> bufferData) 
    {
#ifdef __APPLE__
        glBindBuffer(GL_ARRAY_BUFFER, bufferName);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferData.size() * sizeof(T), bufferData.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
        // glNamedBufferStorage(bufferName, bufferData.size() * sizeof(T), // nbWaysNodes * vec2 * float
        //                      NULL,
        //                      GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
        //                      GL_MAP_WRITE_BIT);
        glNamedBufferSubData(bufferName, 0, bufferData.size() * sizeof(T), bufferData.data());
#endif
    }

    static Renderer& GetInstance()
     {
         return *g_Instance;
     }
    
    void setDataSource(RenderDataManager* manager)
    {
        m_RenderDataManager = manager;
    }

	bool m_ShowTestGUI = false;
	bool m_ShowGUI = true;

	void RenderAABB(AABB& aabb);
	void RenderAABB(AABB& aabb, const glm::vec3& color);

	RenderDataManager* GetRenderDataManager() { return m_RenderDataManager; }

private:
	void DrawBoundingBox(BoundingBoxComponent* bb);
	void DrawManipulator(Entity* entity);


	void RenderEditor();


	void RenderSquare(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);

	void HandleScreenshot();

	// C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
    Renderer(Renderer const&)               = delete;
    void operator=(Renderer const&)  = delete;

    RenderDataManager* m_RenderDataManager;
	
	int m_NumLights;

	int m_TempObjectID;

	std::vector<bool> m_ShaderBeenUsedThisFrame;

	FramebufferType m_ColorFramebuffer = FramebufferType_MainColorBuffer;

};

class GPUProfileHolder
{
public:
	GPUProfileHolder(const char* sectionName);
	~GPUProfileHolder();
};

#define lxGPUProfile(s) GPUProfileHolder profile_##s = GPUProfileHolder(#s); 

#endif // RENDERER

