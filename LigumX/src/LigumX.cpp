#include "stdafx.h"

#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "LigumX.h"
#include "Editor.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include "EngineStats.h"
#include "Serializer.h"
#include "ObjectManager.h"
#include "Settings.h"

#include "Mesh.h"
#include "DefaultTextureHolder.h"
#include "RenderDataManager.h"

#include "Heightfield.h"
#include "Sector.h"
#include "World.h"
#include "SectorData.h"
#include "SectorManager.h"

#include "OSMDataProcessor.h"
#include "OSMElementTypeDataStore.h"

#include "MainWindow.h"
#include "GUI.h"

#include "imgui_impl_glfw_gl3.h"
#include "imgui.h"

using namespace glm;
using namespace std;

const double static_dt = 1.0 / 100.0;

//Renderer* LigumX::m_Renderer;

int main(int argc, char *argv[])
{
    srand(987654321);

    LigumX& game = LigumX::GetInstance();
	
    game.Initialize();

    while(game.GetApplicationState() != EApplicationState_WantsToQuit) 
	{
        glfwPollEvents();

        game.MainLoop();
    }

    glfwTerminate();
    return 0;
}


void LigumX::BeginFrame()
{
	static double dt = 0.0;
	//    static double curr_time = glfwGetTime();
	static double physic_accumulator = 0.0;

	physic_accumulator += dt;

	g_InputHandler->FrameUpdate();

	m_SystemFrame++;
}

void LigumX::DoFrame()
{
	world->Update();

	g_RenderDataManager->GatherVisibleEntities(world, m_Renderer->GetDebugCamera());

	m_Renderer->render(world);
}

void LigumX::EndFrame()
{
	g_InputHandler->Reset();
}

void LigumX::MainLoop()
{
	BeginFrame();

	DoFrame();

	EndFrame();
}


void LigumX::Initialize()
{
	//=============================================================================
	// Load Default data.
	//=============================================================================
	InitializeGlobalObjects();

	SetCallbacks();

	loadSettings();

	populateTypeColorArray();

    //=============================================================================
    // Load world data.
    //=============================================================================

    Settings& settings = Settings::GetInstance();
    world = new World(settings.f("sectorSize"));
	world->SetObjectID(28716);
	world->Serialize(false);

    //=============================================================================
    // create and fill VBOs.
    //============================================================================
    Renderer &renderer = Renderer::GetInstance();

    m_RenderDataManager = new RenderDataManager();
    renderer.setDataSource(m_RenderDataManager);

	g_DefaultTextureHolder = new DefaultTextureHolder();


	SetApplicationState(EApplicationState_Running);
}


void LigumX::InitializeGlobalObjects()
{
	g_InputHandler = new InputHandler();
	g_Serializer = new Serializer();
	g_ObjectManager = new ObjectManager();

	g_Editor = new Editor(73524);

	g_EngineSettings = new EngineSettings(89373);

	m_Renderer = new Renderer();
	m_Renderer->SetObjectID(28763);
	m_Renderer->Serialize(false);

	g_GUI = new GUI();

	g_DefaultObjects = new DefaultObjects();
	g_DefaultObjects->Initialize();

	g_EngineStats = new EngineStats();
	
	g_SectorManager = new SectorManager();
	g_OSMDataProcessor = new OSMDataProcessor();
	g_OSMElementTypeDataStore = new OSMElementTypeDataStore();

}

void LigumX::loadSettings(){
    Settings& s = Settings::GetInstance();
    s.load();

    glm::vec2 coordinateShift = trunc(Settings::GetInstance().f2("cameraPosition"));
    s.add("coordinateShifting", -coordinateShift);

	Renderer& renderer = Renderer::GetInstance();
	renderer.m_ShowGUI = true;

}


World* LigumX::GetWorld()
{
    return world;
}

void LigumX::ResetWorld()
{
	world->ResetSectors();
}

string LigumX::labelFromType(OSMElement::ElementType type)
{
    switch (type)
	{
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
        case(OSMElement::aDEBUG):               return "debug";break;
        case(OSMElement::RAILWAY_SUBWAY):       return "Railway (subway)";break;
        case(OSMElement::LANDUSE):              return "Land use";break;
        case(OSMElement::BOUNDARY):             return "Boundary";break;
        case(OSMElement::SIDEWALK):             return "Sidewalk";break;
        case(OSMElement::NOT_IMPLEMENTED):      return "NOT_IMPLEMENTED";break;

    }
    return "!!! WRONG TYPE !!!";
}


void LigumX::populateTypeColorArray()
{

	// todo : this shouldnt really be in renderer
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



	renderer.typeColorMap.emplace(OSMElement::HIGHWAY_TRUNK, vec3(1, 1, 1));
	renderer.typeColorMap.emplace(OSMElement::HIGHWAY_PRIMARY, vec3(0.9, 0.9, 0.9));
	renderer.typeColorMap.emplace(OSMElement::HIGHWAY_SECONDARY, vec3(0.8, 0.8, 0.8));
	renderer.typeColorMap.emplace(OSMElement::HIGHWAY_TERTIARY, vec3(0.7, 0.7, 0.7));
	renderer.typeColorMap.emplace(OSMElement::HIGHWAY_RESIDENTIAL, vec3(0.6, 0.6, 0.6));
	renderer.typeColorMap.emplace(OSMElement::HIGHWAY_UNCLASSIFIED, vec3(0.5, 0.5, 0.5));
	renderer.typeColorMap.emplace(OSMElement::HIGHWAY_SERVICE, vec3(0.4, 0.4, 0.4));
	renderer.typeColorMap.emplace(OSMElement::BUILDING_UNMARKED, vec3(0, 0, 1));
	renderer.typeColorMap.emplace(OSMElement::BUILDING_SCHOOL, vec3(0, 0, 1));
	renderer.typeColorMap.emplace(OSMElement::BUILDING_ADDRINTERP, vec3(0.4, 1, 1));
	renderer.typeColorMap.emplace(OSMElement::RAILWAY_SUBWAY, vec3(1, 0, 1));
	renderer.typeColorMap.emplace(OSMElement::NATURAL_WOOD, vec3(0, 0.5, 0));
	renderer.typeColorMap.emplace(OSMElement::NATURAL_WATER, vec3(0, 0, 0.5));
	renderer.typeColorMap.emplace(OSMElement::LEISURE_PARK, vec3(0, 1, 0));
	renderer.typeColorMap.emplace(OSMElement::ADDR_INTERPOLATION, vec3(1, 0, 0));
	renderer.typeColorMap.emplace(OSMElement::GRID_LINE, vec3(0.4, 0.4, 0.4));
	renderer.typeColorMap.emplace(OSMElement::aDEBUG, vec3(1.0, 0, 1.0));
	renderer.typeColorMap.emplace(OSMElement::LANDUSE, vec3(1.0, 1.0, 1.0));
	renderer.typeColorMap.emplace(OSMElement::BOUNDARY, vec3(1.0, 1.0, 1.0));
	renderer.typeColorMap.emplace(OSMElement::CONTOUR, vec3(1.0, 0.1, 0.1));

}


void LigumX::SetCallbacks()
{
    Renderer& renderer = Renderer::GetInstance();

	GLFWwindow* window = renderer.m_Window->pWindow;
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback );
    glfwSetKeyCallback(window, HandleKeyboardInput);
    //glfwSetCharCallback( renderer.pWindow, glfwCharCallback );
    glfwSetCursorPosCallback(window, glfwMousePositionCallback );
    glfwSetCursorEnterCallback(window, glfwMouseEntersCallback );
    glfwSetScrollCallback(window, glfwMouseScrollCallback );
    glfwSetWindowPosCallback(window, glfwWindowPositionCallback );
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback );
    glfwSetWindowCloseCallback(window, glfwWindowClosedCallback );
    glfwSetWindowRefreshCallback(window, glfwWindowRefreshCallback );
    glfwSetWindowFocusCallback(window, glfwWindowFocusCallback );
    glfwSetWindowIconifyCallback(window, glfwWindowIconifyCallback );
    glfwSetFramebufferSizeCallback(window, glfwWindowFramebufferSizeCallback );
}

void LigumX::Shutdown()
{
	SetApplicationState(EApplicationState_WantsToQuit);
	m_Renderer->Shutdown();
}