#include "stdafx.h"


#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "LigumX.h"
#include "SectorData.h"
#include "Settings.h"
#include "RenderDataManager.h"
#include "World.h"
#include "Sector.h"
#include "Heightfield.h"
#include "Mesh.h"


#include "imgui_impl_glfw_gl3.h"
#include "imgui.h"
using namespace glm;
using namespace std;

std::string ProgramPipeline::ShadersPath;
const double static_dt = 1.0 / 100.0;


int main(int argc, char *argv[])
{
    std::string _ClassName = "Main";
    srand(987654321);

    LigumX& game = LigumX::GetInstance();



	// Restore state
	//glBindTexture(GL_TEXTURE_2D, last_texture);
	
	
	game.running = true;
    game.init();

	//io.IniFilename = "imgui.ini";
	//io.RenderDrawListsFn = my_render_function;  // Setup a render function, or set to NULL and call GetDrawData() after Render() to access the render data.

	// TODO: Fill others settings of the io structure

	// Load texture atlas
	// There is a default font so you don't need to care about choosing a font yet
	//unsigned char* pixels;
	//int width, height;
	//io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	//// Upload texture to graphics system
	//GLint last_texture;
	//glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

    while(game.running) 
	{
        glfwPollEvents();

		//ImGuiIO& io = ImGui::GetIO();
		//io.DeltaTime = 1.0f / 60.0f;
		//io.MousePos = mouse_pos;
		//io.MouseDown[0] = mouse_button_0;
		//io.MouseDown[1] = mouse_button_1;
		//io.KeysDown[i] = ...

        game.mainLoop();

        glfwSwapBuffers(Renderer::GetInstance().pWindow);
    }

    glfwTerminate();
    return 0;
}

void LigumX::mainLoop()
{
    static double dt = 0.0;
//    static double curr_time = glfwGetTime();
    static double physic_accumulator = 0.0;

    physic_accumulator += dt;

    camera->handlePresetNewFrame(Renderer::GetInstance().pWindow);



    
    if (Settings::GetInstance().i("loadNewSectors"))
    {
        updateWorld(10);
    }

	Renderer::GetInstance().render(world);

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

    populateTypeColorArray();

    // register GLFW and GLdebug callbacks
    SetCallbacks();

	//=============================================================================
	// Load Default data.
	//=============================================================================
	g_DefaultMeshes = new DefaultMeshes();


    //=============================================================================
    // Load world data.
    //=============================================================================

    Settings& settings = Settings::GetInstance();
    world = new World(settings.f("sectorSize"));
    // world->createSector(vec2(-73.650, 45.500));
    vec2 tp;
    if (settings.i("useCameraPositionAsTestPoint")) tp = glm::vec2(camera->GetPosition());
    else tp = settings.f2("testPoint");
    

    //=============================================================================
    // create and fill VBOs.
    //============================================================================
    Renderer &renderer = Renderer::GetInstance();

    renderer.init_pipelines();

    renderData = new RenderDataManager();
    renderer.setDataSource(renderData);

    //=============================================================================
    // Textures, framebuffer, renderbuffer
    //=============================================================================
    //init_tweakBar();

    // updateWorld(3);

}

void LigumX::updateWorld(int loadingRingSize)
{
    Renderer& renderer = Renderer::GetInstance();

    std::vector<Sector*>* newSectors = world->updateSectorsAroundPoint(glm::vec2(camera->GetPosition()), loadingRingSize);

    for(int i = 0; i < newSectors->size(); ++i)
    {
    
        Sector* sector = newSectors->at(i);
        if (sector->m_initializationLevel < Sector::FullyInitialized)
        {
            if (sector->m_initializationLevel == Sector::ContourLoaded)
            {
                renderData->initializeSector(sector);
            }

            if (sector->m_initializationLevel == Sector::DataLoaded)
            {
                renderData->fillBuffers(sector);
            }

            if (sector->m_initializationLevel == Sector::HeightfieldGenerated)
            {
                //PRINT("heightfield geenerated, adding to terrain buffer")
                 renderData->addToTerrainBuffer(sector);
            }
        }


    }

    delete(newSectors);

    // newSectors = world->sectorsAroundPoint(glm::vec2(camera->position), 0);

    // Sector* sector = newSectors->at(0);
    // if (sector->m_heightfield)
    // {
    //     double height = sector->m_heightfield->getHeight(glm::vec2(camera->position));
    //     PRINT(height * 15000);
    // }
}


void LigumX::initCamera()
{
    camera = new Camera();
    camera->setViewSize(Settings::GetInstance().f("viewSize"));
    Renderer::GetInstance().camera = camera;
    draggingCamera = false;

    camera->translateTo(Settings::GetInstance().f3("cameraPosition"));
    camera->lookAtTargetPos = Settings::GetInstance().f3("cameraLookAt");
}

void LigumX::loadSettings(){
    Settings& s = Settings::GetInstance();
    s.load();

    glm::vec2 coordinateShift = trunc(Settings::GetInstance().f2("cameraPosition"));
    s.add("coordinateShifting", -coordinateShift);
    buildingHeight = s.i("buildingHeight");
    buildingSideScaleFactor = 1;

	Renderer& renderer = Renderer::GetInstance();
	renderer.drawGround = false;
	renderer.showText = true;
	renderer.m_ShowGUI = true;

    ProgramPipeline::ShadersPath = s.s("ShadersPath");

}


World* LigumX::getWorld()
{
    return world;
}

string LigumX::labelFromType(OSMElement::ElementType type)
{
    switch (type){
        case(OSMElement::HIGHWAY_TRUNK):        return "Highway (trunk)"; break;
        case(OSMElement::HIGHWAY_PRIMARY):      return "Highway (primary)";break;
        case(OSMElement::HIGHWAY_SECONDARY):    return "Highway (secondary)";break;
        case(OSMElement::HIGHWAY_TERTIARY):     return "Highway (tertiary)";break;
        case(OSMElement::HIGHWAY_RESIDENTIAL):  return "Highway (residential)";break;
        case(OSMElement::HIGHWAY_UNCLASSIFIED): return "Highway (unclassified)";break;
        case(OSMElement::HIGHWAY_SERVICE):      return "Highway (service)";break;
        case(OSMElement::NATURAL_WOOD):         return "Natural (wood)";break;
        case(OSMElement::NATURAL_WATER):        return "Natural (water)";break;
        case(OSMElement::BUILDING_UNMARKED):    return "Building (unmarked)";break;
        case(OSMElement::BUILDING_SCHOOL):      return "Building (school)";break;
        case(OSMElement::BUILDING_ADDRINTERP):  return "Building (address interpolation)";break;
        case(OSMElement::LEISURE_PARK):         return "Leisure (park)";break;
        case(OSMElement::ADDR_INTERPOLATION):   return "Address interpolation";break;
        case(OSMElement::CONTOUR):              return "Contour line";break;
        case(OSMElement::GRID_LINE):            return "Grid line";break;
        case(OSMElement::aDEBUG):               return "debug";break;
        case(OSMElement::RAILWAY_SUBWAY):       return "Railway (subway)";break;
        case(OSMElement::LANDUSE):              return "Land use";break;
        case(OSMElement::BOUNDARY):             return "Boundary";break;
        case(OSMElement::SIDEWALK):             return "Sidewalk";break;
        case(OSMElement::NOT_IMPLEMENTED):      return "NOT_IMPLEMENTED";break;
        case(OSMElement::ANY_TYPE):             return "ANY_TYPE";break;

    }
    return "!!! WRONG TYPE !!!";
}


void LigumX::populateTypeColorArray(){
    Renderer& renderer = Renderer::GetInstance();
    renderer.typeColorMap.emplace(OSMElement::HIGHWAY_TRUNK, vec3(1,1,1));
    renderer.typeColorMap.emplace(OSMElement::HIGHWAY_PRIMARY, vec3(0.9,0.9,0.9));
    renderer.typeColorMap.emplace(OSMElement::HIGHWAY_SECONDARY, vec3(0.8,0.8,0.8));
    renderer.typeColorMap.emplace(OSMElement::HIGHWAY_TERTIARY, vec3(0.7,0.7,0.7));
    renderer.typeColorMap.emplace(OSMElement::HIGHWAY_RESIDENTIAL, vec3(0.6,0.6,0.6));
    renderer.typeColorMap.emplace(OSMElement::HIGHWAY_UNCLASSIFIED, vec3(0.5,0.5,0.5));
    renderer.typeColorMap.emplace(OSMElement::HIGHWAY_SERVICE, vec3(0.4,0.4,0.4));
    renderer.typeColorMap.emplace(OSMElement::BUILDING_UNMARKED, vec3(0,0,1));
    renderer.typeColorMap.emplace(OSMElement::BUILDING_SCHOOL, vec3(0,0,1));
    renderer.typeColorMap.emplace(OSMElement::BUILDING_ADDRINTERP, vec3(0.4,1,1));
    renderer.typeColorMap.emplace(OSMElement::RAILWAY_SUBWAY, vec3(1,0,1));
    renderer.typeColorMap.emplace(OSMElement::NATURAL_WOOD, vec3(0,0.5,0));
    renderer.typeColorMap.emplace(OSMElement::NATURAL_WATER, vec3(0,0,0.5));
    renderer.typeColorMap.emplace(OSMElement::LEISURE_PARK, vec3(0,1,0));
    renderer.typeColorMap.emplace(OSMElement::ADDR_INTERPOLATION, vec3(1,0,0));
    renderer.typeColorMap.emplace(OSMElement::GRID_LINE, vec3(0.4,0.4,0.4));
    renderer.typeColorMap.emplace(OSMElement::aDEBUG, vec3(1.0,0,1.0));
    renderer.typeColorMap.emplace(OSMElement::LANDUSE, vec3(1.0,1.0,1.0));
    renderer.typeColorMap.emplace(OSMElement::BOUNDARY, vec3(1.0,1.0,1.0));
    renderer.typeColorMap.emplace(OSMElement::CONTOUR, vec3(1.0,0.1,0.1));

    renderer.displayElementType.emplace(OSMElement::HIGHWAY_TRUNK, true);
    renderer.displayElementType.emplace(OSMElement::HIGHWAY_PRIMARY, true);
    renderer.displayElementType.emplace(OSMElement::HIGHWAY_SECONDARY, true);
    renderer.displayElementType.emplace(OSMElement::HIGHWAY_TERTIARY, true);
    renderer.displayElementType.emplace(OSMElement::HIGHWAY_RESIDENTIAL, true);
    renderer.displayElementType.emplace(OSMElement::HIGHWAY_UNCLASSIFIED, true);
    renderer.displayElementType.emplace(OSMElement::HIGHWAY_SERVICE, true);
    renderer.displayElementType.emplace(OSMElement::BUILDING_UNMARKED, true);
    renderer.displayElementType.emplace(OSMElement::BUILDING_SCHOOL, true);
    renderer.displayElementType.emplace(OSMElement::BUILDING_ADDRINTERP, true);
    renderer.displayElementType.emplace(OSMElement::RAILWAY_SUBWAY, true);
    renderer.displayElementType.emplace(OSMElement::NATURAL_WOOD, true);
    renderer.displayElementType.emplace(OSMElement::NATURAL_WATER, true);
    renderer.displayElementType.emplace(OSMElement::LEISURE_PARK, true);
    renderer.displayElementType.emplace(OSMElement::ADDR_INTERPOLATION, true);
    renderer.displayElementType.emplace(OSMElement::GRID_LINE, true);
    renderer.displayElementType.emplace(OSMElement::aDEBUG, true);
    renderer.displayElementType.emplace(OSMElement::LANDUSE, true);
    renderer.displayElementType.emplace(OSMElement::BOUNDARY, true);
    renderer.displayElementType.emplace(OSMElement::CONTOUR, true);

}

static bool deleteAll( OSMElement * theElement ) { delete theElement; return true; }


void LigumX::toggleEntityLand() {
    if(!inEntityLand) {
        savedCam = *camera; // save current camera setup

        camera->translateTo(vec3(0,0,0.01));
        camera->lookAtTargetPos = vec3(0,0,0);
        camera->cameraType = Camera::CameraType::TOP_3D;
        camera->controlType = Camera::ControlType::QWEASDZXC_ARROWS;
        inEntityLand = true;
    } else {
        // go back to regular LigumX space
        inEntityLand = false;
        *camera = savedCam; // reinstall saved camera setup
    }
}


static void test_error_cb (int error, const char *description)
{
    fprintf(stderr, "%d: %s\n", error, description);
}

void LigumX::SetCallbacks(){
    Renderer& renderer = Renderer::GetInstance();
    glfwSetMouseButtonCallback( renderer.pWindow, glfwMouseButtonCallback );
    glfwSetKeyCallback( renderer.pWindow, HandleKeyboardInput);
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

