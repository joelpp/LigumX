#ifndef GAME_H
#define GAME_H

// #define GLFW_INCLUDE_GLCOREARB

#include <sstream>
#include <cstdarg>
#include <unordered_map>
#include <chrono>
#include <typeinfo>
#include <stdlib.h>
#include <map>

#include "renderer.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "AntTweakBar.h"
#include "Logging.h"
#include "entity.h"
#include "settings.h"


// class Renderer;
class OSMElement;
class Relation;
class Node;
class Way;
class LineSegment;
class Triangle;
class World;
class RenderDataManager;
// class Settings;
// 

#define TIME(x)    {auto begin = std::chrono::high_resolution_clock::now();\
                    x;\
                    auto end = std::chrono::high_resolution_clock::now();\
                    std::cout << "Time to run \"" << #x << "\" : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;}


#define string_pair std::pair<std::string,std::string>

class LigumX {

public:

    // general state and functions
    void init();
    bool running;
    void mainLoop();

    World* getWorld();

    std::pair<glm::vec2, glm::vec2> windowBoundingBox();
    void updateRenderData();

    // TODO: move this to data
    void populateTypeColorArray();
    std::string labelFromType(OSMElement::ElementType type);

    // TODO: move this to input controller
    // GLFW callbacks. Implement if needed.
    void SetCallbacks();
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



    //TODO: maybe some kind of viewport struct or class could hold a camera, renderer (or renderer holds viewport), mouse positions, etc.
    // camera
    Camera* camera;
    bool draggingCamera;
    glm::vec2 oldMousePosition;
    glm::vec2 windowPosToWorldPos(glm::vec2 ij);


    std::unordered_map<OSMElement::ElementType, std::vector<glm::vec3> > waysNodesPositionsMap;
    std::unordered_map<std::string, int> tagConversionTable;
    World* world;
    RenderDataManager* renderData;

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

    void init_tweakBar();
    void init_freetype();
    void initCamera();
    void loadSettings();
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


//    void RenderText(Text t);
//    
//    
static LigumX& GetInstance()
{
    static LigumX instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
    return instance;
}
private:
    REGISTERCLASS(LigumX);

    LigumX() {};                   // Constructor? (the {} brackets) are needed here.

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
    LigumX(LigumX const&)               = delete;
    void operator=(LigumX const&)  = delete;
};

#endif // GAME_H
