#ifndef GAME_H
#define GAME_H

// #define GLFW_INCLUDE_GLCOREARB


#include <stdlib.h>
#include <iostream>
#include <cstdarg>
#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include <chrono>
#include "glm/glm.hpp"
#include "AntTweakBar.h"


#include "renderer.h"
#include "world.h"
#include "program_pipeline.h"
#include "camera.h"
#include "osm_element.h"
#include "node.h"
#include "way.h"
#include "relation.h"
#include "texture.h"
#include "filter.h"
#include "linesegment.h"
#include "heightfield.h"
#include "triangle.h"
#include "entity.h"
#include "chunk.h"


#define TIME(x)    {auto begin = std::chrono::high_resolution_clock::now();\
                    x;\
                    auto end = std::chrono::high_resolution_clock::now();\
                    std::cout << "Time to run \"" << #x << "\" : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;}
#define PRINTVEC2(v) std::cout << #v << ": x=" << v.x << " y=" << v.y << "\n";
#define PRINTVEC2VECTOR(v) for (int _index_ = 0; _index_ < v.size(); _index_++) std::cout << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y << "\n";
#define PRINTVEC3(v) std::cout << #v << ": x=" << v.x << " y=" << v.y << " z=" << v.z << "\n";
#define PRINTVEC3VECTOR(v) for (int _index_ = 0; _index_ < v.size(); _index_++) std::cout << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y  << v[_index_].z << "\n";
#define PRINT(f) std::cout << #f << ": " << f << "\n";
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
using namespace SpatialIndex;

class Game {
public:
    // general state and functions
    Game() {init();}
    void init();
    bool running;
    void mainLoop();

    void insertDebugMessage(std::string message, GLenum severity, GLuint id = 0);

    void fillBuffers(std::vector<glm::vec3> *nodesPositions,
                     std::vector<glm::vec3> *waysNodesPositions,
                     std::vector<glm::vec3> *waysNodesColors,
                     std::vector<glm::vec3> *roadsPositions,
                     std::vector<glm::vec3> *buildingTrianglePositions,
                     std::vector<glm::vec3> *buildingLines,
                     std::vector<float> *buildingLinesTexCoords,
                     std::vector<glm::vec3> *groundTrianglesPositions,
                     std::vector<glm::vec2> *groundTrianglesUV,
                     std::vector<float> *groundTriangleTextureIDs);
    // void triangulateLoops();

    Node* findClosestNode(glm::vec2 xy);
    std::vector<Way*> findNClosestWays(int n, glm::vec2 xy, int filter, std::vector<double> &distances, std::vector<glm::vec2> &directions, std::vector<std::pair<Node*, Node*> > &_nodePairs);
    double pointLineSegmentDistance(glm::vec2 p, glm::vec2 p1, glm::vec2 p2, glm::vec2 &direction);
    void updateSelectedWay(Way* way);
    void populateTypeColorArray();

    double contourLineInterpolate(glm::vec2 xy);
    int getLerpedContourLines(glm::vec2 xy, std::vector<Way*> ways, std::vector<glm::vec2> directions, std::vector<std::pair<Node*, Node*>> nodePairs);
    std::pair<int, int> findCommonWay(std::vector<Way*> firstNodeWays, std::vector<Way*> secondNodeWays);
    std::string labelFromType(OSMElement::ElementType type);

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

    // camera
    Camera* camera;
    bool draggingCamera;
    glm::vec2 oldMousePosition;
    glm::vec2 windowPosToWorldPos(glm::vec2 ij);

    // viewing
    Renderer renderer;
    float viewRectLeft, viewRectRight, viewRectBottom, viewRectTop; // geo coordinates of the viewing region
    glm::vec2 viewRectBottomLeft;
    glm::vec2 viewRectTopRight;
    glm::vec2 viewRectVecDiago; // vector from bottomLeft to topRight;


    // data
    std::vector<glm::vec3> groundTrianglesPositions; // positions of nodes forming ways, possibly contains duplicates.
    std::vector<glm::vec2> groundTrianglesUV;
    std::unordered_map<OSMElement::ElementType, std::vector<glm::vec3> > waysNodesPositionsMap;
    std::unordered_map<std::string, int> tagConversionTable;
    World* world;

    // debug stuff

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
    float sunOrientation;
    float sunTime;
    float sunSpeed;
    bool sunMoveAuto;
    bool saveScreenshot;
    bool drawGround;
    bool renderText;
    

    void init_tweakBar();
    void init_freetype();

    // Entity stuff
    EntityManager entityManager;
    Camera savedCam;
    bool inEntityLand;
    void TW_CALL toggleEntityLand();



    struct characterQuadVertices{
        GLfloat vertices[6][3];
        characterQuadVertices( GLfloat qwe[6][3])
        {
            for(int i=0; i<6; ++i) {
                for(int j=0; j<3; ++j) {
                    vertices[i][j] = qwe[i][j];
                }
            }
        }
    };

    struct Text {
        std::string text;
        glm::vec3 position;
        bool projected;
        float scale;
        std::vector<characterQuadVertices> quads;
    };

    std::vector<Text> texts;
//    void RenderText(Text t);
};

extern Game* game;

#endif // GAME_H
