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

#define FLUSH_ERRORS() outputGLError(__func__, __LINE__);
//#define FLUSH_ERRORS()

class Material;
class Model;
class Mesh;
class RenderDataManager;
class World;
class Entity;

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

public:
    void Initialize();


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
    void DrawModel(Entity* entity, Model* model);
    static void outputGLError(std::string func, int line);
    void RenderSky();
    void RenderFPS();
    void DrawTerrain();
	void RenderEntities(std::vector<Entity*> entities);

	void Bind2DTexture(int slot, GLuint HWObject);
	void FreeBoundTexture();

	void SetUniform(int value, const char* name, GLuint location);
	void SetUniform(glm::vec3& value, const char* name, GLuint location);
	void SetUniform(const glm::vec3& value, const char* name, GLuint location);
	void SetUniform(glm::mat4x4& value, const char* name, GLuint location);
	void SetUniform(const glm::mat4x4& value, const char* name, GLuint location);
	void SetUniform(float value, const char* name, GLuint location);
	void SetFragmentUniform(int value, const char* name);

	//void SetVertexUniform(glm::vec3& value, const char* name);
	template<typename T>
	void SetVertexUniform(T& value, const char* name);
	template<typename T>
	void SetFragmentUniform(T& value, const char* name);

	void  SetFragmentUniform(glm::vec3& value, const char* name, GLuint location);


	//template<typename T>
	//void SetFragmentUniform(T value, const char* name);

	//void SetVertexUniform(glm::mat4x4& value, const char* name);
	void SetPipeline(ProgramPipeline* pipeline);
	void SetLightingUniforms();
	void SetViewUniforms();
	void SetDebugUniforms();
	void SetMaterialUniforms(Material* material);

    Camera *camera;
	bool m_DrawTerrain;
	bool m_DrawSky;
	bool m_WireframeRendering;
	bool m_UseLighting;
	bool m_ShowNormals;
	bool m_ShowSpecular;
	bool m_ShowDiffuse;
	bool m_ShowAmbient;

    bool drawBuildingSides;
    bool drawGround;
    bool showText;
    bool saveScreenshot;

    float dt, curr_time, fps;

    // window params
    unsigned int windowWidth;
    unsigned int windowHeight;
    std::string windowTitle;
    GLFWwindow* pWindow;

    float sunTime;
    float sunOrientation;
    // textures
    GLuint glidTextureScreenRoads; // for implicit definition of the roads.
    GLuint glidFramebuffer;
    Texture* pBuildingTex;
    std::unordered_map<std::string, Texture*> textureMap;

    // shaders
    ProgramPipeline* pPipelineLines;
    ProgramPipeline* pPipelineRoads;
    ProgramPipeline* pPipelineScreenQuad;
    ProgramPipeline* pPipelineBuildings;
    ProgramPipeline* pPipelineBuildingSides;
    ProgramPipeline* pPipelineGround;
    ProgramPipeline* pPipelineEnvmap;
    ProgramPipeline* pPipelineText;
    ProgramPipeline* pPipelineNodes;
    ProgramPipeline* pPipelineBasic;
    ProgramPipeline* pPipelineBasicUV;
    ProgramPipeline* activePipeline;
    std::unordered_map<std::string, ProgramPipeline*> ProgramPipelinesMap;

    // need to keep those for swapping
    ProgramPipeline::ShaderProgram* pGeometryShader1;
    ProgramPipeline::ShaderProgram* pGeometryShader2;
    ProgramPipeline::ShaderProgram* pGeometryShader3;
    ProgramPipeline::ShaderProgram* pFragmentShader1;
    ProgramPipeline::ShaderProgram* pFragmentShader2;
    ProgramPipeline::ShaderProgram* pFragmentShader3;

    std::vector<Mesh*> m_debugMeshes;
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
		FLUSH_ERRORS();
		glGenBuffers(1, &bufferName);
		FLUSH_ERRORS();
        glBindBuffer(GL_ARRAY_BUFFER, bufferName);
		FLUSH_ERRORS();
		glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(T), bufferData.data(), GL_DYNAMIC_DRAW /*GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                             GL_MAP_WRITE_BIT*/);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, bufferData.size() * sizeof(T), bufferData.data());
		FLUSH_ERRORS();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
		FLUSH_ERRORS();

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
	void HandleScreenshot();
	void BeginImGUIWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name);
	void EndImGUIWindow();

	void ShowVariableAsText(glm::vec3 variable, const char* variableName);
	void ShowVariableAsText(float variable, const char* variableName);

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
    Renderer(Renderer const&)               = delete;
    void operator=(Renderer const&)  = delete;

    RenderDataManager* renderData;
};

#endif // RENDERER

