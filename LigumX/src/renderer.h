#ifndef RENDERER
#define RENDERER


#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Freetype/ft2build.h"
#include FT_FREETYPE_H

#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>

#include "program_pipeline.h"
#include "texture.h"
#include "camera.h"
#include "osm_element.h"
#include "imgui_impl_glfw_gl3.h"

#pragma region  FORWARD_DECLARATIONS Renderer
#include "property.h"
class DisplayOptions;
class PostEffects;

#pragma endregion  FORWARD_DECLARATIONS Renderer

class Framebuffer;
class SunLight;

#define FLUSH_ERRORS() outputGLError(__func__, __LINE__);
//#define FLUSH_ERRORS()

enum FramebufferType
{
	FramebufferType_MainColorBuffer,
	FramebufferType_ShadowMap,
	FramebufferType_Picking,
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


class Renderer{

#pragma region  HEADER Renderer
static const int ClassID = 1498036510;
public:
DisplayOptions* GetDisplayOptions() { return m_DisplayOptions; }; 
void SetDisplayOptions(DisplayOptions* value) { m_DisplayOptions = value; };
PostEffects* GetPostEffects() { return m_PostEffects; }; 
void SetPostEffects(PostEffects* value) { m_PostEffects = value; };
const glm::vec2& GetMouseClickPosition() { return m_MouseClickPosition; }; 
void SetMouseClickPosition(glm::vec2 value) { m_MouseClickPosition = value; };
private:
DisplayOptions* m_DisplayOptions;
PostEffects* m_PostEffects;
glm::vec2 m_MouseClickPosition;
public:
static const int g_RendererPropertyCount = 3;
static const ClassPropertyData g_Properties[g_RendererPropertyCount];


#pragma endregion  HEADER Renderer
public:
	const unsigned int SHADOW_WIDTH = 800, SHADOW_HEIGHT = 800;

	World* m_World;

    void Initialize();
	void InitGL();
	void InitFramebuffers();
	void InitFreetype();
	GLuint CreateTexture();

    // subfunctions
    void init_pipelines();
    void init_pipelines_buildingSides();
    void init_pipelines_filledBuildings();
    void init_pipelines_roads();
    void init_pipelines_screenQuad();
    void init_pipelines_text();

    void render(World* world);
    void RenderText(Text t);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, bool projected);
    void DrawMesh(Mesh* mesh, Material* material);
	void DrawMesh(Mesh* mesh);
    void DrawModel(Entity* entity, Model* model);
    static void outputGLError(std::string func, int line);
    void RenderSky();
    void RenderFPS();
    void DrawTerrain();
	void RenderEntities(std::vector<Entity*> entities);

	void BeforeFrame(World* world);
	void RenderShadowMap();
	void RenderOpaque();
	void RenderTextureOverlay();
	void RenderPicking();


	void Bind2DTexture(int slot, GLuint HWObject);
	void BindTexture(GLuint& hwTexture);
	void FreeBoundTexture();

	void SetUniform(int value, const char* name, GLuint location);
	void SetUniform(glm::vec3& value, const char* name, GLuint location);
	void SetUniform(const glm::vec3& value, const char* name, GLuint location);
	void SetUniform(glm::vec2& value, const char* name, GLuint location);
	void SetUniform(glm::mat4x4& value, const char* name, GLuint location);
	void SetUniform(const glm::mat4x4& value, const char* name, GLuint location);
	void SetUniform(float value, const char* name, GLuint location);
	void SetFragmentUniform(int value, const char* name);
	void SetComputeUniform(int value, const char* name);

	template<typename T>
	void SetComputeUniform(T& value, const char* name);

	//void SetVertexUniform(glm::vec3& value, const char* name);
	template<typename T>
	void SetVertexUniform(T& value, const char* name);
	template<typename T>
	void SetFragmentUniform(T& value, const char* name);

	void  SetFragmentUniform(glm::vec3& value, const char* name, GLuint location);


	void SetPipeline(ProgramPipeline* pipeline);
	void SetLightingUniforms();
	void SetViewUniforms(Camera* cam);
	void SetShadowMapUniforms(Camera* cam);
	void SetDebugUniforms();
	void SetMaterialUniforms(Material* material);
	void SetPostEffectsUniforms();

    Camera *camera;
	Camera *m_ShadowCamera;
    bool drawBuildingSides;
    bool saveScreenshot;

    float dt, curr_time, fps;

    // window params
    unsigned int windowWidth;
    unsigned int windowHeight;
    std::string windowTitle;
    GLFWwindow* pWindow;

	Framebuffer* m_Framebuffers[NBFramebuffers];

	// textures
    std::unordered_map<std::string, Texture*> textureMap;

    // shaders
    ProgramPipeline* pPipelineLines;
    ProgramPipeline* pPipelineRoads;
    ProgramPipeline* pPipelineScreenQuad;
    ProgramPipeline* pPipelineBuildings;
    ProgramPipeline* pPipelineBuildingSides;
    ProgramPipeline* pPipelineGround;
	ProgramPipeline* pPipelinePicking;
	ProgramPipeline* pPipelinePickingCompute;
	ProgramPipeline* pPipelineEnvmap;
	ProgramPipeline* pPipelineScreenSpaceTexture;
    ProgramPipeline* pPipelineText;
    ProgramPipeline* pPipelineNodes;
    ProgramPipeline* pPipelineBasic;
    ProgramPipeline* pPipelineBasicUV;
	ProgramPipeline* pPipelineShadowMap;
	ProgramPipeline* activePipeline;
    std::unordered_map<std::string, ProgramPipeline*> ProgramPipelinesMap;

    // need to keep those for swapping
    ProgramPipeline::ShaderProgram* pGeometryShader1;
    ProgramPipeline::ShaderProgram* pGeometryShader2;
    ProgramPipeline::ShaderProgram* pGeometryShader3;
    ProgramPipeline::ShaderProgram* pFragmentShader1;
    ProgramPipeline::ShaderProgram* pFragmentShader2;
    ProgramPipeline::ShaderProgram* pFragmentShader3;

    std::vector<Model*> m_debugModels;

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

	Light m_TestLight;

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
         static Renderer instance; // Guaranteed to be destroyed.
                                   // Instantiated on first use.
         return instance;
     }
    
    void setDataSource(RenderDataManager* manager)
    {
        renderData = manager;
    }

	bool m_ShowTestGUI = false;
	bool m_ShowGUI = true;



private:
    REGISTERCLASS(Renderer);

    Renderer() {};                   // Constructor? (the {} brackets) are needed here.
	void RenderGUI();

	template <typename T>
	void ShowPropertyGridTemplate(T* object, const char* name);

	template<typename T>
	void ShowGenericProperty(T* object, const ClassPropertyData& propertyData);

	void ShowPropertyTemplate(const char* object, const char* name, const LXType& type);

	template<typename T>
	void ShowPropertyGridMacro(T* object, const char* name);

	template<typename T>
	void ShowPropertyValue(T* object, const ClassPropertyData& propertyData);

	void HandleScreenshot();
	void BeginImGUIWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name);
	void EndImGUIWindow();

	void ShowVariableAsText(glm::vec3 variable, const char* variableName);
	void ShowVariableAsText(glm::vec3* variable, const char* variableName);
	void ShowVariableAsText(float variable, const char* variableName);
	void ShowGUIText(const char* text);
	void ShowGUIText(std::string* text);
	void ShowGUIText(std::string* text, const char* variableName);
	void ShowGUIText(const std::string& text);
	void ShowGUIText(const std::string& text, const char* variableName);


	void ShowProperty(bool* value, const char* name);
	void ShowProperty(float* value, const char* name, float min, float max);
	void ShowProperty(glm::vec3* value, const char* name);
	void ShowProperty(std::string* value, const char* name);

	template<typename T>
	void Renderer::ShowPropertyTemplate(T* object, const ClassPropertyData& propertyData);
    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
    Renderer(Renderer const&)               = delete;
    void operator=(Renderer const&)  = delete;

    RenderDataManager* renderData;
	const int pickingBufferSize = 128;

	glm::vec3 m_PickedColor;
	glm::vec2 m_LastMouseClickPosition;

	Entity* m_PickedEntity;
};

#endif // RENDERER

