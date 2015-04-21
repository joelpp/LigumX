#ifndef GAME_H
#define GAME_H

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <cstdarg>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "tinyxml2.h"
#include "AntTweakBar.h"

#include "program_pipeline.h"
#include "camera.h"
#include "osm_element.h"
#include "node.h"
#include "way.h"
#include "relation.h"
#include "texture.h"
#include "filter.h"
#include "linesegment.h"
#include <chrono>


#define TIME(x)    {auto begin = std::chrono::high_resolution_clock::now();\
                    x;\
                    auto end = std::chrono::high_resolution_clock::now();\
                    std::cout << "Time to run \"" << #x << "\" : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;}
#define PRINTVEC2(v) std::cout << #v << ": x=" << v.x << " y=" << v.y << "\n";
#define PRINTVEC2VECTOR(v) for (int _index_ = 0; _index_ < v.size(); _index_++) std::cout << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y << "\n";
#define PRINTVEC3(v) std::cout << #v << ": x=" << v.x << " y=" << v.y << " z=" << v.z << "\n";
#define PRINTVEC3VECTOR(v) for (int _index_ = 0; _index_ < v.size(); _index_++) std::cout << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y  << v[_index_].z << "\n";
#define PRINT(f) std::cout << #f << ": = " << f << "\n";
#define PRINTSTRING(f) std::cout << f << "\n";
#define PRINTBOOL(B) string b = B==1?"TRUE":"FALSE"; std::cout << #B << " : " << b << "\n";
#define PRINTINT(i) std::cout << #i << ": x=" << i << "\n";
#define PRINTVECTOR(e) std::cout << "Element Vector: " << #e << "\n"; \
                       for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_] << "\n";
#define PRINTELEMENT(e) std::cout << #e << " " << e.toString() << "\n";
#define PRINTELEMENTPTR(e) std::cout << #e << " " << e->toString() << "\n";
#define PRINTELEMENTVECTOR(e) std::cout << "Element Vector: " << #e << "\n"; \
                              for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_]->toString() << "\n";

#define string_pair std::pair<std::string,std::string>


class Game {
public:
    // general state and functions
    Game() {init();}
    void init();
    bool running;
    void mainLoop();
    void insertDebugMessage(std::string message, GLenum severity, GLuint id = 0);
    void loadXML(std::string path);

    void fillBuffers(std::vector<glm::vec3> *waysNodesPositions,
                     std::vector<glm::vec3> *waysNodesColors,
                     std::vector<vec3> *roadsPositions,
                     std::vector<vec3> *buildingTrianglePositions,
                     std::vector<vec3> *buildingLines,
                     std::vector<float> *buildingLinesTexCoords,
                     std::vector<vec3> *groundTrianglesPositions,
                     std::vector<vec2> *groundTrianglesUV);



    GLFWwindow* pWindow;
    Node* findClosestNode(glm::vec2 xy);
    std::vector<Way*> findNClosestWays(int n, glm::vec2 xy, int filter, std::vector<double> &distances, std::vector<glm::vec2> &directions, std::vector<std::pair<Node*, Node*> > &_nodePairs);
    double pointLineSegmentDistance(glm::vec2 p, glm::vec2 p1, glm::vec2 p2, glm::vec2 &direction);
    void updateSelectedWay(Way* way);
    OSMElement::ElementType typeFromStrings(std::string key, std::string value);
    void populateTypeColorArray();

    void generateGridLines(std::vector<vec3> *groundTrianglesPositions, std::vector<vec2> *groundTrianglesUV);

    double contourLineInterpolate(glm::vec2 xy);
    int getLerpedContourLines(glm::vec2 xy, std::vector<Way*> ways, std::vector<glm::vec2> directions, std::vector<std::pair<Node*, Node*>> nodePairs);
    void extrudeAddrInterps();
    std::pair<int, int> findCommonWay(std::vector<Way*> firstNodeWays, std::vector<Way*> secondNodeWays);
    std::string labelFromType(OSMElement::ElementType type);
    template<typename T> void createGLBuffer(GLuint &bufferName, std::vector<T> bufferData) {
        glCreateBuffers(1, &bufferName);
        glNamedBufferStorage(bufferName, bufferData.size() * sizeof(T), // nbWaysNodes * vec2 * float
                             NULL,
                             GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                             GL_MAP_WRITE_BIT);
        glNamedBufferSubData(bufferName, 0, bufferData.size() * sizeof(T), bufferData.data());
    }

    // viewing
    float viewRectLeft, viewRectRight, viewRectBottom, viewRectTop; // geo coordinates of the viewing region
    glm::vec2 viewRectBottomLeft;
    glm::vec2 viewRectTopRight;
    glm::vec2 viewRectVecDiago; // vector from bottomLeft to topRight;

    // window params
    unsigned int windowWidth;
    unsigned int windowHeight;
    std::string windowTitle;

    // GLFW callbacks. Implement if needed.
    static void glfwMouseButtonCallback(GLFWwindow* /*pGlfwWindow*/, int /*button*/, int /*action*/, int /*mods*/);
    static void glfwMousePositionCallback(GLFWwindow* /*pGlfwWindow*/, double /*x*/, double /*y*/);
    static void glfwMouseEntersCallback(GLFWwindow* /*pGlfwWindow*/, int /*entered*/) {}
    static void glfwMouseScrollCallback(GLFWwindow* /*pGlfwWindow*/, double /*x*/, double /*y*/);
    static void glfwKeyCallback(GLFWwindow* /*pGlfwWindow*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/);
    static void glfwCharCallback(GLFWwindow* /*pGlfwWindow*/, unsigned int /*codepoint*/) {}
    static void glfwWindowPositionCallback(GLFWwindow* /*pGlfwWindow*/, int /*xpos*/, int /*ypos*/) {}
    static void glfwWindowSizeCallback(GLFWwindow* /*pGlfwWindow*/, int /*width*/, int /*height*/) {}
    static void glfwWindowClosedCallback(GLFWwindow* /*pGlfwWindow*/);
    static void glfwWindowRefreshCallback(GLFWwindow* /*pGlfwWindow*/) {}
    static void glfwWindowFocusCallback(GLFWwindow* /*pGlfwWindow*/, int /*focused*/) {}
    static void glfwWindowIconifyCallback(GLFWwindow* /*pGlfwWindow*/, int /*iconified*/) {}
    static void glfwWindowFramebufferSizeCallback(GLFWwindow* /*pGlfwWindow*/, int /*width*/, int /*height*/) {}
//    static void debugCallback(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum /*severity*/,
//                       GLsizei /*length*/, const GLchar* /*message*/, const void* /*pVoidWindow*/);

    // shaders
    ProgramPipeline* pPipelineLines;
    ProgramPipeline* pPipelineRoads;
    ProgramPipeline* pPipelineScreenQuad;
    ProgramPipeline* pPipelineBuildings;
    ProgramPipeline* pPipelineBuildingSides;
    ProgramPipeline* pPipelineGround;
    ProgramPipeline* pPipelineEnvmap;
    // need to keep those for swapping
    ProgramPipeline::ShaderProgram* pGeometryShader1;
    ProgramPipeline::ShaderProgram* pGeometryShader2;
    ProgramPipeline::ShaderProgram* pGeometryShader3;
    ProgramPipeline::ShaderProgram* pFragmentShader1;
    ProgramPipeline::ShaderProgram* pFragmentShader2;
    ProgramPipeline::ShaderProgram* pFragmentShader3;

    // VBOs
    GLuint glidWaysPositions;
    GLuint glidWaysColors;
    unsigned int nbWaysVertices;
    unsigned int nbGroundVertices;
    GLuint glidBufferRoadsPositions;
    unsigned int nbRoads;
    std::vector<GLint> firstVertexForEachRoad;
    std::vector<GLsizei> nbVerticesForEachRoad;
    GLuint glidScreenQuadPositions;
    GLuint glidScreenQuadTexCoords;
    GLuint glidBufferBuildingTriangleVertices;
    unsigned int nbBuildingTriangles;
    GLuint glidBufferBuildingLines;
    unsigned int nbBuildingLines;
    GLuint glidBufferBuildingLoopLengths;
    GLuint glidGroundTrianglePositions;
    GLuint glidGroundTriangleUVs;
    std::unordered_map<OSMElement::ElementType, std::vector<vec3> > waysNodesPositionsMap;
    std::unordered_map<OSMElement::ElementType, GLuint > glidWaysNodesPositions;
    std::unordered_map<OSMElement::ElementType, vec3> typeColorMap;
    std::unordered_map<OSMElement::ElementType, bool> displayElementType;
    // camera
    Camera* camera;
    bool draggingCamera;
    glm::vec2 oldMousePosition;
    glm::vec2 windowPosToWorldPos(glm::vec2 ij);
    // data
    std::unordered_map<std::string, Node*> theNodes;
    std::unordered_map<std::string, Way*> theWays;
    std::unordered_map<std::string, Relation*> theRelations;

    std::unordered_map<std::string, int> tagConversionTable;

    // textures
    GLuint glidTextureScreenRoads; // for implicit definition of the roads.
    GLuint glidFramebuffer;
    Texture* pBuildingTex;
    std::unordered_map<std::string, Texture*> textureMap;
    // debug stuff

    bool fancyDisplayMode;
    bool showTweakBar;
    bool drawBuildingSides;
    TwBar *myBar;
    struct WaySelection{
        Way* way;
        int numberOfBytesBefore;
        int numberOfBytesToWrite;
    };
    int unsuccessfulInterpolations;
    bool interpolateContours;
    WaySelection selectedWay;
    float buildingHeight;
    float buildingSideScaleFactor;
    double coordinateInflationFactor;
    float sunOrientation;
    float sunTime;
    float sunSpeed;
    bool sunMoveAuto;
    bool saveScreenshot;


    // subfunctions
    void init_pipelines();
    void init_pipelines_buildingSides();
    void init_pipelines_filledBuildings();
    void init_pipelines_generalLines();
    void init_pipelines_groundTriangles();
    void init_pipelines_roads();
    void init_pipelines_screenQuad();
    void init_pipelines_envmap();
    void init_tweakBar();



};

extern Game* game;








//class PipeNodes;

//class LigumX
//{
//public:
//    goGLu::Window* window;
//    DataBuffer1D<glm::vec2>* nodes;
//    PipeNodes* pipeNodes;
//    goGLu::ProgramPipeline::ShaderProgram* vertexShaderNodes;
//    goGLu::ProgramPipeline::ShaderProgram* fragmentShaderNodes;
//    DataBuffer1D<glm::vec2>* ways;
//    DataBuffer1D<glm::vec3>* waysColors;
//    goGLu::Camera* camera;
//    goGLu::Keyboard* keyboard;

//public:
//    LigumX();
//    void mainLoop();
//};

#endif // GAME_H
