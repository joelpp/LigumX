

#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "LigumX.h"
#include "SectorData.h"
#include "Settings.h"
#include "RenderDataManager.h"
#include "World.h"
using namespace glm;
using namespace std;

std::string ProgramPipeline::ShadersPath;
const double static_dt = 1.0 / 100.0;


int main(int argc, char *argv[])
{
    srand(987654321);

    LigumX& game = LigumX::GetInstance();
    game.running = true;
    game.init();
    while(game.running) {
        glfwPollEvents();

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

    Renderer& renderer = Renderer::GetInstance();
    updateRenderData();
    renderer.render();

    if(showTweakBar) TwDraw();
}

void LigumX::updateRenderData()
{
    Renderer& renderer = Renderer::GetInstance();
    std::vector<Sector*>* newSectors = world->loadSectorsAroundPoint(glm::vec2(camera->position), Settings::GetInstance().i("loadingRingSize"));

    for(int i = 0; i < newSectors->size(); ++i)
    {

        renderData->addToTerrainBuffer(newSectors->at(i));
        renderData->fillBuffers(newSectors->at(i));
        renderer.init_pipelines();
    }

    delete(newSectors);
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

void LigumX::initCamera(){
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
    Renderer::GetInstance().drawGround = false;
    Renderer::GetInstance().showText = false;
    showTweakBar = false;
    ProgramPipeline::ShadersPath = s.s("ShadersPath");

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
    renderer.typeColorMap.emplace(OSMElement::CONTOUR, vec3(0.1,0.1,0.1));

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


void TW_CALL LigumX::toggleEntityLand() {
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


