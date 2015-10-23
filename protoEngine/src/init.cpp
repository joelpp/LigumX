#include "LigumX.h"
#include <vector>
#include <chrono>

#include "world.h"
#include "Settings.h"
#include "Sector.h"
#include "SectorData.h"
#include "heightfield.h"
#include "triangle.h"
#include "node.h"
#include "way.h"
#include "relation.h"
#include "RenderDataManager.h"

using namespace std;
using namespace glm;

static void test_error_cb (int error, const char *description)
    {
        fprintf(stderr, "%d: %s\n", error, description);
    }

void LigumX::SetCallbacks(){
    Renderer& renderer = Renderer::GetInstance();
    glfwSetMouseButtonCallback( renderer.pWindow, glfwMouseButtonCallback );
    glfwSetKeyCallback( renderer.pWindow, glfwKeyCallback );
    glfwSetCharCallback( renderer.pWindow, glfwCharCallback );
    glfwSetCursorPosCallback( renderer.pWindow, glfwMousePositionCallback );
    glfwSetCursorEnterCallback( renderer.pWindow, glfwMouseEntersCallback );
    glfwSetScrollCallback( renderer.pWindow, glfwMouseScrollCallback );
    glfwSetWindowPosCallback( renderer.pWindow, glfwWindowPositionCallback );
    glfwSetWindowSizeCallback( renderer.pWindow, glfwWindowSizeCallback );
    glfwSetWindowCloseCallback( renderer.pWindow, glfwWindowClosedCallback );
    glfwSetWindowRefreshCallback( renderer.pWindow, glfwWindowRefreshCallback );
    glfwSetWindowFocusCallback( renderer.pWindow, glfwWindowFocusCallback );
    glfwSetWindowIconifyCallback( renderer.pWindow, glfwWindowIconifyCallback );
    glfwSetFramebufferSizeCallback( renderer.pWindow, glfwWindowFramebufferSizeCallback );
}

void LigumX::init()
{
    //=============================================================================
    // Parameters, camera setup.
    //=============================================================================
    
    Renderer::GetInstance().Initialize();
    running = true;
    loadSettings();

    initCamera();
    init_tweakBar();

    populateTypeColorArray();


    // register GLFW and GLdebug callbacks
    SetCallbacks();

    //=============================================================================
    // Load world data.
    //=============================================================================

    Settings& settings = Settings::GetInstance();
    world = new World(settings.f("sectorSize"));
    // world->createSector(vec2(-73.650, 45.500));
    vec2 tp;
    if (settings.i("useCameraPositionAsTestPoint")) tp = glm::vec2(camera->position);
    else tp = settings.f2("testPoint");
    

    //=============================================================================
    // create and fill VBOs.
    //============================================================================
    Renderer &renderer = Renderer::GetInstance();
    renderData = new RenderDataManager();
    updateRenderData();

    //=============================================================================
    // Textures, framebuffer, renderbuffer
    //=============================================================================
    renderer.init_pipelines();
    
}

void LigumX::init_freetype(){

}





