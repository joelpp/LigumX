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
#include "Logging.h"
#include "entity.h"
#include "DefaultObjects.h"
#include "settings.h"


// class Renderer;
class OSMElement;
class Relation;
class Node;
class Way;
class LineSegment;
class World;
class RenderDataManager;
// class Settings;
// 

#define TIME(x)    {auto begin = std::chrono::high_resolution_clock::now();\
                    x;\
                    auto end = std::chrono::high_resolution_clock::now();\
                    std::cout << "Time to run \"" << #x << "\" : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;}


#define string_pair std::pair<std::string,std::string>

enum EApplicationState
{
	EApplicationState_Uninitialized,
	EApplicationState_Running,
	EApplicationState_WantsToQuit
};

class LigumX 
{

public:
	static Renderer* GetRenderer() { return m_Renderer; };
	static Renderer* m_Renderer;
    // general state and functions
    void Initialize();
	void InitializeGlobalObjects();

	void BeginFrame();
	void DoFrame();
	void EndFrame();

    void MainLoop();

	void Shutdown();

	void ResetWorld();

    World* GetWorld();

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
    static void HandleKeyboardInput(GLFWwindow* /*pGlfwWindow*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/);
    static void glfwCharCallback(GLFWwindow* /*pGlfwWindow*/, unsigned int /*codepoint*/) {}
    static void glfwWindowPositionCallback(GLFWwindow* /*pGlfwWindow*/, int /*xpos*/, int /*ypos*/) {}
    static void glfwWindowSizeCallback(GLFWwindow* /*pGlfwWindow*/, int /*width*/, int /*height*/) {}
    static void glfwWindowClosedCallback(GLFWwindow* /*pGlfwWindow*/);
    static void glfwWindowRefreshCallback(GLFWwindow* /*pGlfwWindow*/) {}
    static void glfwWindowFocusCallback(GLFWwindow* /*pGlfwWindow*/, int /*focused*/) {}
    static void glfwWindowIconifyCallback(GLFWwindow* /*pGlfwWindow*/, int /*iconified*/) {}
    static void glfwWindowFramebufferSizeCallback(GLFWwindow* /*pGlfwWindow*/, int /*width*/, int /*height*/) {}

    World* world;

    RenderDataManager* m_RenderDataManager;

    // debug stuff
    void loadSettings();

	void SetApplicationState(EApplicationState appState) { m_ApplicationState = appState; }
	EApplicationState GetApplicationState() { return m_ApplicationState; }

static LigumX& GetInstance()
{
    static LigumX instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
    return instance;
}

private:

	EApplicationState m_ApplicationState = EApplicationState_Uninitialized;

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
