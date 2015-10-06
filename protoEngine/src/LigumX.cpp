

#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "LigumX.h"
#include "SectorData.h"
#include "Settings.h"


using namespace glm;
using namespace std;


LigumX* Game;

int main(int argc, char *argv[])
{
    srand(987654321);

    Game = new LigumX();
    while(Game->running) {
        glfwPollEvents();

        Game->mainLoop();

        glfwSwapBuffers(Game->renderer.pWindow);
    }

    glfwTerminate();
    return 0;
}

World* LigumX::getWorld(){
    return world;
}

string LigumX::labelFromType(OSMElement::ElementType type){
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

    }
}

void LigumX::initCamera(){
    camera = new Camera();
    camera->setViewSize(m_settings.f("viewSize"));
    renderer.camera = camera;
    draggingCamera = false;

    camera->translateTo(m_settings.f3("cameraPosition"));
    camera->lookAtTargetPos = m_settings.f3("cameraLookAt");

    PRINTVEC3(camera->position)
    PRINTVEC3(camera->lookAtTargetPos);
}

void LigumX::loadSettings(){
    m_settings.load();

    buildingHeight = m_settings.i("buildingheight");
    buildingSideScaleFactor = 1;
    renderer.drawGround = false;
    renderer.showText = false;
    showTweakBar = false;

}

void LigumX::populateTypeColorArray(){

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

//    /*


    // for (auto it = typeColorMap.begin; it != typeColorMap.end; it++){
        // displayElementType.emplace(it->first, true);
    // }
//*/
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


