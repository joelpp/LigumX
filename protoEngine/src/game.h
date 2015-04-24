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
#include "entity.h"


class Game {
public:
    // general state and functions
    Game() {init();}
    void init();
    bool running;
    void mainLoop();
    void insertDebugMessage(std::string message, GLenum severity, GLuint id = 0);
    void loadXML(std::string path);
    void fillBuffers(std::vector<glm::vec2> *waysNodesPositions, std::vector<glm::vec3> *waysNodesColors, std::vector<glm::vec2> *roadsPositions, std::vector<glm::vec2>* buildingTrianglePositions);
    GLFWwindow* pWindow;
    Node* findClosestNode(glm::vec2 xy);
    std::vector<Way*> findNClosestWays(int n, glm::vec2 xy, int filter);
    double pointLineSegmentDistance(glm::vec2 p, glm::vec2 p1, glm::vec2 p2);
    bool isInterestingWay(Way* way);
    void updateSelectedWay(Way* way);
    OSMElement::ElementType typeFromStrings(std::string key, std::string value);
    glm::vec3 colorFromTags(Way* way);
    // viewing
    float viewRectLeft, viewRectRight, viewRectBottom, viewRectTop; // geo coordinates of the viewing region
    glm::vec2 viewRectBottomLeft;
    glm::vec2 viewRectTopRight;
    glm::vec2 viewRectVecDiago; // vector from bottomLeft to topRight;

    // window params
    unsigned int windowWidth;
    unsigned int windowHeight;
    std::string windowTitle;

    // GLFW callbacks
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
    GLuint glidBufferRoadsPositions;
    unsigned int nbRoads;
    std::vector<GLint> firstVertexForEachRoad;
    std::vector<GLsizei> nbVerticesForEachRoad;
    GLuint glidScreenQuadPositions;
    GLuint glidScreenQuadTexCoords;
    GLuint glidBufferBuildingTriangleVertices;
    unsigned int nbBuildingTriangles;


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

    // debug stuff

    bool showWhat;
    TwBar *myBar;
    struct WaySelection{
        Way* way;
        int numberOfBytesBefore;
        int numberOfBytesToWrite;
    };

    WaySelection selectedWay;

    EntityManager entityManager;
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
