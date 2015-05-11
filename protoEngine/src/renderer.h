#ifndef RENDERER
#define RENDERER


#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>

#include "program_pipeline.h"
#include "texture.h"
#include "camera.h"
#include "osm_element.h"


class Renderer{
public:
    Renderer();

    bool fancyDisplayMode;
    bool drawBuildingSides;
    bool drawGround;

    // window params
    unsigned int windowWidth;
    unsigned int windowHeight;
    std::string windowTitle;
    GLFWwindow* pWindow;


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

    // need to keep those for swapping
    ProgramPipeline::ShaderProgram* pGeometryShader1;
    ProgramPipeline::ShaderProgram* pGeometryShader2;
    ProgramPipeline::ShaderProgram* pGeometryShader3;
    ProgramPipeline::ShaderProgram* pFragmentShader1;
    ProgramPipeline::ShaderProgram* pFragmentShader2;
    ProgramPipeline::ShaderProgram* pFragmentShader3;


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


    // subfunctions
    void init_pipelines();
    void init_pipelines_buildingSides();
    void init_pipelines_filledBuildings();
    void init_pipelines_generalLines();
    void init_pipelines_groundTriangles();
    void init_pipelines_roads();
    void init_pipelines_screenQuad();
    void init_pipelines_envmap();
    void init_pipelines_text();
    void init_pipelines_nodes();

    void render(Camera *camera);
//    void RenderText(Text t);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, bool projected, Camera *camera);
    //text renderign stuff
    struct Character {
        GLuint     TextureID;  // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        GLuint     Advance;    // Offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;



    template<typename T> void createGLBuffer(GLuint &bufferName, std::vector<T> bufferData) {
#ifdef __APPLE__
        glGenBuffers(1, &bufferName);
        glBindBuffer(GL_ARRAY_BUFFER, bufferName);
        glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(T), 0, GL_DYNAMIC_DRAW /*GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                             GL_MAP_WRITE_BIT*/);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferData.size() * sizeof(T), bufferData.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

#else
        glCreateBuffers(1, &bufferName);
        glNamedBufferStorage(bufferName, bufferData.size() * sizeof(T), // nbWaysNodes * vec2 * float
                             NULL,
                             GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                             GL_MAP_WRITE_BIT);
        glNamedBufferSubData(bufferName, 0, bufferData.size() * sizeof(T), bufferData.data());
#endif
    }
};

#endif // RENDERER
