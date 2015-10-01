

#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "game.h"




using namespace glm;
using namespace std;
using namespace SpatialIndex;


Game* game;

int main(int argc, char *argv[])
{

    game = new Game();
    while(game->running) {
        glfwPollEvents();

        game->mainLoop();

        glfwSwapBuffers(game->renderer.pWindow);
    }

    glfwTerminate();
    return 0;
}



string Game::labelFromType(OSMElement::ElementType type){
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



void Game::populateTypeColorArray(){

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

// // Convert the address interpolation ways into building polygons
// void Game::extrudeAddrInterps(){
//     int counter = 0;

//     // Iterate on all ways
//     for ( auto it = world->theWays.begin(); it != world->theWays.end(); ++it ){
//         Way* way = it->second;

//         // Keep only ADDR_INTERPOLATIONs
//         if (way->eType != OSMElement::ADDR_INTERPOLATION) continue;

//         vector<Node*> nodes;
//         // Store this way's 2 nodes
//         for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt) nodes.push_back(*nodeIt);

//         // We only want to look at roads
//         int filter = OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_PRIMARY | OSMElement::HIGHWAY_TRUNK | OSMElement::HIGHWAY_SERVICE | OSMElement::HIGHWAY_UNCLASSIFIED;

//         // Preparing road query...
//         vector<vector<vec2> > directions;
//         directions.push_back(vector<vec2>());
//         directions.push_back(vector<vec2>());

//         vector<std::pair<Node*, Node*>> nodePairs;
//         vector<vector<double> > distances;
//         distances.push_back(vector<double>());
//         distances.push_back(vector<double>());

//         // Find the closest roads to this ADDR_INTERP
//         vector<Way*> firstNodeWays = findNClosestWays(5, vec2(nodes[0]->longitude, nodes[0]->latitude), filter, distances[0], directions[0], nodePairs);
//         vector<Way*> secondNodeWays = findNClosestWays(5, vec2(nodes[1]->longitude, nodes[1]->latitude), filter, distances[1], directions[1], nodePairs);

//         LineSegment L0(nodes[0], nodes[1]);

//         bool intersectOtherRoad = false;
//         //Check if the adress way intersects any of these roads
//         for (int i = 0 ; i < firstNodeWays.size(); i++){
//             LineSegment L1(firstNodeWays[i]);

//             if (!intersectOtherRoad && L0.intersects(L1)) intersectOtherRoad = true;
//         }

//         for (int i = 0 ; i < secondNodeWays.size(); i++){
//             LineSegment L1(secondNodeWays[i]);

//             if (!intersectOtherRoad && L0.intersects(L1)) intersectOtherRoad = true;
//         }

// //        if (intersectOtherRoad) continue;

//         // Find the closest way that both nodes share (should generally be the one parallel to the addr_interp)
//         // Maybe better to look at dot products? This works ok for now
//         pair<int, int> commonIndices = findCommonWay(firstNodeWays, secondNodeWays);

//         int i = commonIndices.first;
//         int j = commonIndices.second;

//         // Multiplicative factor related to the polygon size
//         double factor = 1;

//         //If finding the common way didnt work just move on
//         if (i == -1) continue;

//         // Move on to the next if the common road is too far from one of the nodes
//         if (distances[0][i] > 0.0002 * factor) continue;
//         if (distances[1][j] > 0.0002 * factor) continue;

//         // Move on to next if the nodes are on different sides of the common road
//         if (dot(directions[0][i], directions[1][j]) < 0) continue;


//         // Create the new nodes by moving from each edge of the ADDR_INTERP, perpendicularly to the road
//         Node* node3 = new Node();
//         node3->id = string("ADDR_INTERP_NODE3_").append(std::to_string(counter));
//         node3->longitude = nodes[0]->longitude - directions[0][i].x * factor;
//         node3->latitude = nodes[0] -> latitude - directions[0][i].y * factor;
//         node3->elevation = contourLineInterpolate(vec2(node3->longitude, node3->latitude)) * 1.0001;
//         Node* node4 = new Node();
//         node4->id = string("ADDR_INTERP_NODE4_").append(std::to_string(counter));
//         node4->longitude = nodes[1]->longitude - directions[0][i].x * factor;
//         node4->latitude = nodes[1]->latitude - directions[0][i].y * factor;
//         node4->elevation = contourLineInterpolate(vec2(node4->longitude, node4->latitude)) * 1.0001;
//         // Create the new building
//         Way* buildingWay  = new Way();
//         buildingWay->addRef(nodes[1]);
//         buildingWay->addRef(node4);
//         buildingWay->addRef(node3);
//         buildingWay->addRef(nodes[0]);
//         buildingWay->addRef(nodes[1]);
//         buildingWay->eType = OSMElement::BUILDING_ADDRINTERP;
//         buildingWay->id = string("ADDR_INTERP_BUILDING").append(std::to_string(counter));

//         // Store the new building
//         theWays.emplace(buildingWay->id, buildingWay);
//         counter++;
//     }

//     PRINTSTRING("Extruding address interpolation into polygons");
//     PRINTINT(counter);
// }

void TW_CALL Game::toggleEntityLand() {
    if(!inEntityLand) {
        savedCam = *camera; // save current camera setup

        camera->translateTo(vec3(0,0,0.01));
        camera->lookAtTargetPos = vec3(0,0,0);
        camera->cameraType = Camera::CameraType::TOP_3D;
        camera->controlType = Camera::ControlType::QWEASDZXC_ARROWS;
        inEntityLand = true;
    } else {
        // go back to regular game space
        inEntityLand = false;
        *camera = savedCam; // reinstall saved camera setup
    }
}


