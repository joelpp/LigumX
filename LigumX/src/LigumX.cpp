#include "stdafx.h"


#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "LigumX.h"
#include "ObjectIDManager.h"
#include "SectorData.h"
#include "Settings.h"
#include "RenderDataManager.h"
#include "World.h"
#include "Sector.h"
#include "Heightfield.h"
#include "Mesh.h"
#include "Serializer.h"


#include "imgui_impl_glfw_gl3.h"
#include "imgui.h"
using namespace glm;
using namespace std;

std::string ProgramPipeline::ShadersPath;
const double static_dt = 1.0 / 100.0;

//Renderer* LigumX::m_Renderer;

int main(int argc, char *argv[])
{
    std::string _ClassName = "Main";
    srand(987654321);

    LigumX& game = LigumX::GetInstance();
	
    game.init();

	game.running = true;
    while(game.running) 
	{
        glfwPollEvents();

        game.mainLoop();
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

    if (Settings::GetInstance().i("loadNewSectors"))
    {
        updateWorld(10);
    }

	m_Renderer->render(world);
}

void LigumX::init()
{
	//=============================================================================
	// Load Default data.
	//=============================================================================


	g_Serializer = new Serializer();
	g_ObjectIDManager = new ObjectIDManager();

	m_Renderer = new Renderer();
	m_Renderer->SetObjectID(28763);
	m_Renderer->Serialize(false);

	g_DefaultMeshes = new DefaultMeshes();

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
	//glm::vec2 cameraPos = glm::vec2(renderer.GetDebugCamera()->GetPosition());

 //   std::vector<Sector*>* newSectors = world->updateSectorsAroundPoint(cameraPos, loadingRingSize);

 //   for(int i = 0; i < newSectors->size(); ++i)
 //   {
 //   
 //       Sector* sector = newSectors->at(i);
 //       if (sector->m_initializationLevel < Sector::FullyInitialized)
 //       {
 //           if (sector->m_initializationLevel == Sector::ContourLoaded)
 //           {
 //               renderData->initializeSector(sector);
 //           }

 //           if (sector->m_initializationLevel == Sector::DataLoaded)
 //           {
 //               renderData->fillBuffers(sector);
 //           }

 //           if (sector->m_initializationLevel == Sector::HeightfieldGenerated)
 //           {
 //               //PRINT("heightfield geenerated, adding to terrain buffer")
 //                renderData->addToTerrainBuffer(sector);
 //           }
 //       }


 //   }

 //   delete(newSectors);

	world->Update();
}


void LigumX::loadSettings(){
    Settings& s = Settings::GetInstance();
    s.load();

    glm::vec2 coordinateShift = trunc(Settings::GetInstance().f2("cameraPosition"));
    s.add("coordinateShifting", -coordinateShift);
    buildingHeight = s.i("buildingHeight");
    buildingSideScaleFactor = 1;

	Renderer& renderer = Renderer::GetInstance();
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

void LigumX::Shutdown()
{
	m_Renderer->Shutdown();

	running = false;
}