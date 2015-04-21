#include <sstream>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <typeinfo>
#include <chrono>

#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "game.h"
#include "texture.h"



using namespace glm;
using namespace std;



Game* game;

int main()
{

    game = new Game();
    while(game->running) {
        glfwPollEvents();

        game->mainLoop();

        glfwSwapBuffers(game->pWindow);
    }

    glfwTerminate();
    return 0;
}









void Game::loadXML(string path){
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());
    tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();
    cout << docRoot->Value() << "\n";
    for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        if (string(child->Value()).compare("bound") == 0){
            string box = child->ToElement()->FindAttribute("box")->Value();
            std::istringstream ss(box);
            std::string token;

            std::getline(ss, token, ','); viewRectBottom = atof(token.c_str()) * coordinateInflationFactor - 45;
            std::getline(ss, token, ','); viewRectLeft = atof(token.c_str()) * coordinateInflationFactor   + 73;
            std::getline(ss, token, ','); viewRectTop = atof(token.c_str()) * coordinateInflationFactor    - 45;
            std::getline(ss, token, ','); viewRectRight = atof(token.c_str()) * coordinateInflationFactor  + 73;

            PRINT(viewRectBottom);
            PRINT(viewRectLeft);
            PRINT(viewRectTop);
            PRINT(viewRectRight);
            viewRectBottomLeft = vec2(viewRectLeft,viewRectBottom);
            viewRectTopRight = vec2(viewRectRight,viewRectTop);
            viewRectVecDiago = viewRectBottomLeft - viewRectTopRight;
        }
        if (string(child->Value()).compare("node") == 0){
//            cout << "Looking at a node \n";
//            cout << child->ToElement()->FindAttribute("id")->Value() << "\n";

            string id = child->ToElement()->FindAttribute("id")->Value();
            float latitude = atof(child->ToElement()->FindAttribute("lat")->Value()) * coordinateInflationFactor  - 45;
            float longitude = atof(child->ToElement()->FindAttribute("lon")->Value()) * coordinateInflationFactor + 73;
            //double latitude = strtod(child->ToElement()->FindAttribute("lat")->Value(), NULL);
            //double longitude = strtod(child->ToElement()->FindAttribute("lon")->Value(), NULL);

            Node* node = new Node(id, longitude, latitude);
            node->elevation = 0;
//            if (path.compare("../data/result.xml") == 0) node->elevation = contourLineInterpolate(vec2(node->longitude, node->latitude)) * 1.0001;
            for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement()){
                string key = tag->ToElement()->FindAttribute("k")->Value();
                string value = tag->ToElement()->FindAttribute("v")->Value();
                node -> addTag(key, value);
            }
            theNodes.emplace(id, node);
        }

        else if (string(child->Value()).compare("way") == 0){
//            cout << "Looking at a way \n";
            string id = child->ToElement()->FindAttribute("id")->Value();
            Way* way = new Way(id);
            way->eType = OSMElement::NOT_IMPLEMENTED;
            for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement()){
                if (string(way_child->Value()).compare("nd") == 0){
                    string ref = way_child->ToElement()->FindAttribute("ref")->Value();
                    way -> addRef(theNodes[ref]);
                }
                else if (string(way_child->Value()).compare("tag") == 0){
                    string key = way_child->ToElement()->FindAttribute("k")->Value();
                    string value = way_child->ToElement()->FindAttribute("v")->Value();
                    way -> addTag(key, value);

                    OSMElement::ElementType _eType = typeFromStrings(key, value);
                    if (_eType == OSMElement::NOT_IMPLEMENTED) continue;
                    else way->eType = _eType;
                }
            }


            theWays.emplace(id, way);
        }
        else if (string(child->Value()).compare("relation") == 0){
            string id = child->ToElement()->FindAttribute("id")->Value();
            Relation *relation = new Relation(id);

            for (tinyxml2::XMLNode* relation_child = child->FirstChildElement(); relation_child != NULL; relation_child = relation_child->NextSiblingElement()){
                if (string(relation_child->Value()).compare("member") == 0){
                    string type = relation_child->ToElement()->FindAttribute("type")->Value();
                    string elementId = relation_child->ToElement()->FindAttribute("ref")->Value();
//                    PRINT(type);
//                    PRINT(elementId);
                    //Odd. Ucommenting this crashes the program when trying to fill the buffers.
                    try{
                        if (type.compare("node") == 0) relation->addMember(theNodes.at(elementId));
                        else if (type.compare("way") == 0) relation->addMember(theWays.at(elementId));
                        else if (type.compare("relation") == 0) relation->addMember(theRelations.at(elementId));
                    }
                    catch(...){

                    }
                }
                else if (string(relation_child->Value()).compare("tag") == 0){
                    string key = relation_child->ToElement()->FindAttribute("k")->Value();
                    string value = relation_child->ToElement()->FindAttribute("v")->Value();
                    relation->addTag(key, value);
                }
            }
            theRelations.emplace(id, relation);
        }
    }
}



Node* Game::findClosestNode(vec2 xy){
    Node* closest;
    double bestDist = 99999;
    for ( auto it = theNodes.begin(); it != theNodes.end(); ++it ){
        Node* n = it->second;

        vec2 point = vec2(n->longitude, n->latitude);

        double dist = glm::distance(point, xy);

        if (dist < bestDist){
            closest = n;
            bestDist = dist;
        }
    }
    return closest;
}


std::vector<Way*> Game::findNClosestWays(int n, vec2 xy, int filter, vector<double> &distances, vector<vec2> &_directions, vector<std::pair<Node*, Node*>> &_nodePairs){
    Way* closest = NULL;
    std::vector<Way*> closests;
    std::vector<double> bestDists;
    std::vector<vec2> directions;
    std::pair<Node*, Node*> a;
    vector<std::pair<Node*, Node*>> nodePairs;
    for (int i = 0; i < n; i++){
        bestDists.push_back(99999);
        closests.push_back(NULL);
        directions.push_back(vec2(0,0));
        nodePairs.push_back(a);
    }

    //Look at all the ways
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;
//        if (!way->selectable) continue;

        // This way's type not yet implemented :( get to it!
        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

        // A specific filter has been chosen. Check if this way passes it!
        if (filter != OSMElement::ANY_TYPE)
            if ((way->eType & filter) == 0) continue;

        Node* firstNode;
        Node* secondNode;
        int counter = 1;
        bool first = true;
        vec2 p1, p2;
        double bestDist = 99999;
        vec2 direction;
        std::pair<Node*, Node*> aPair;

        //Look at all this way's nodes
        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt){

            // Keep first node and continue
            if (first){
                firstNode = *nodeIt;
                first = false;
                continue;
            }

            // Prepare next pair by putting the old second in the new first
            if (counter == 0) firstNode = secondNode;
            // Look at next pair by looking at new second node
            else{
                secondNode = *nodeIt;
                p1 = vec2(firstNode->longitude, firstNode->latitude);
                p2 = vec2(secondNode->longitude, secondNode->latitude);
                vec2 dir;
                double dist = pointLineSegmentDistance(xy, p2, p1, dir);

                if (dist < bestDist){
                    closest = way;
                    bestDist = dist;
                    direction = dir;
                    aPair = std::pair<Node*, Node*>(firstNode, secondNode);
                }
            }

            //flip-flop
            counter = (counter + 1) % 2;
        }
        //Now you're done looking at all the nodes.
        //Iterate over all current closest list, from closest to farthest
        for (int i = 0; i < n; i++){

            // Otherwise, we found a smaller distance (the way may have been seen or not)
            if (bestDist <= bestDists[i]){


                // Move everything behind this element farther by 1
                for (int j = n-1; j > i; j--){
                    bestDists[j] = bestDists[j-1];
                    closests[j] = closests[j-1];
                    directions[j] = directions[j-1];
                    nodePairs[j] = nodePairs[j-1];
                }

                // Keep the way and the distance.
                bestDists[i] = bestDist;
                closests[i] = way;
                directions[i] = direction;
                nodePairs[i] = aPair;
                break; //get out! you don't want to keep going in the array.

            }

        }

    }
    _directions = directions;
    distances = bestDists;
    _nodePairs = nodePairs;
    return closests;
}

vec2 Game::windowPosToWorldPos(vec2 ij){
    //if viewsize == x, you see between -x and x
    //Find position according to the window, between -1 and 1
    vec2 relativeToWindow;
    relativeToWindow.x = 2*(ij.x / windowWidth)-1;
    relativeToWindow.y = 2*(ij.y / windowHeight)-1;

    //Required because maps have y going up while the screen has y going down
    relativeToWindow.y *= -1;

    //World position is then the camera position + the relative
    //widnow position, scaled by the viewsize.
    vec2 worldPos;
    worldPos = vec2(camera->position.x,camera->position.y) + relativeToWindow * camera->viewSize;

    return worldPos;

}

double Game::pointLineSegmentDistance(vec2 p, vec2 v, vec2 w, vec2 &direction) {
  // Return minimum distance between line segment vw and point p
  const double l2 = glm::length(w-v) * glm::length(w-v);  // i.e. |w-v|^2 -  avoid a sqrt
  if (l2 == 0.0){
      direction = v - p;
      return glm::distance(p, v);   // v == w case
  }
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line.
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  const double t = glm::dot(p - v, w - v) / l2;
  if (t < 0.0){
      direction = v - p;
      return glm::distance(p, v); // Beyond the 'v' end of the segment
  }
  else if (t > 1.0){
      direction = w - p;
      return glm::distance(p, w);  // Beyond the 'w' end of the segment
  }
  const vec2 projection = v + (vec2(t,t) * (w - v));  // Projection falls on the segment
  direction = projection - p;
  return glm::distance(p, projection);
}


OSMElement::ElementType Game::typeFromStrings(string key, string value){

    if (key.compare("highway") == 0){
        if (value.compare("trunk") == 0) return OSMElement::HIGHWAY_TRUNK;
        else if (value.compare("primary") == 0) return OSMElement::HIGHWAY_PRIMARY;
        else if (value.compare("secondary") == 0) return OSMElement::HIGHWAY_SECONDARY;
        else if (value.compare("tertiary") == 0) return OSMElement::HIGHWAY_TERTIARY;
        else if (value.compare("residential") == 0) return OSMElement::HIGHWAY_RESIDENTIAL;
        else if (value.compare("service") == 0) return OSMElement::HIGHWAY_SERVICE;
        else if (value.compare("unclassified") == 0) return OSMElement::HIGHWAY_UNCLASSIFIED;
    }
    else if (key.compare("natural") == 0){
        if (value.compare("wood") == 0) return OSMElement::NATURAL_WOOD;
        if (value.compare("water") == 0) return OSMElement::NATURAL_WATER;
    }
    else if (key.compare("building") == 0){
        if (value.compare("yes") == 0) return OSMElement::BUILDING_UNMARKED;
        else if (value.compare("school") == 0) return OSMElement::BUILDING_SCHOOL;
    }
    else if (key.compare("contour") == 0) return OSMElement::CONTOUR;
    else if (key.compare("leisure") == 0){
        if (value.compare("park") == 0) return OSMElement::LEISURE_PARK;
    }
    else if (key.compare("addr:interpolation") == 0){ return OSMElement::ADDR_INTERPOLATION; }
    else return OSMElement::NOT_IMPLEMENTED;

}

string Game::labelFromType(OSMElement::ElementType type){
    switch (type){
        case(OSMElement::HIGHWAY_TRUNK): return "Highway (trunk)"; break;
        case(OSMElement::HIGHWAY_PRIMARY): return "Highway (primary)";break;
        case(OSMElement::HIGHWAY_SECONDARY): return "Highway (secondary)";break;
        case(OSMElement::HIGHWAY_TERTIARY): return "Highway (tertiary)";break;
        case(OSMElement::HIGHWAY_RESIDENTIAL): return "Highway (residential)";break;
        case(OSMElement::HIGHWAY_UNCLASSIFIED): return "Highway (unclassified)";break;
        case(OSMElement::HIGHWAY_SERVICE): return "Highway (service)";break;
        case(OSMElement::NATURAL_WOOD): return "Natural (wood)";break;
        case(OSMElement::NATURAL_WATER): return "Natural (water)";break;
        case(OSMElement::BUILDING_UNMARKED): return "Building (unmarked)";break;
        case(OSMElement::BUILDING_SCHOOL): return "Building (school)";break;
        case(OSMElement::LEISURE_PARK): return "Leisure (park)";break;
        case(OSMElement::ADDR_INTERPOLATION): return "Address interpolation";break;
        case(OSMElement::CONTOUR): return "Contour line";break;
        case(OSMElement::GRID_LINE): return "Grid line";break;
        case(OSMElement::aDEBUG): return "debug";break;
        case(OSMElement::RAILWAY_SUBWAY): return "Railway (subway)";break;

    }
}

void Game::updateSelectedWay(Way* myWay){ //or the highway

    if (selectedWay.way != NULL){
        vector<vec3> toWrite;

//        for (int i = 0; i < (selectedWay.way->nodes.size() * 2) - 2; i++) toWrite.push_back(colorFromTags(selectedWay.way));

        // push it to the buffer
        glNamedBufferSubData(glidWaysColors, selectedWay.numberOfBytesBefore, selectedWay.numberOfBytesToWrite, toWrite.data());

    }

    // Figure out how many bytes into the buffer we have to start writing
    int numberOfBytesBefore = 0;
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;

//        if (!way->selectable) continue;
        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

        // If we havent yet reached the way we're looking for, add the current way's number of nodes to the offset
        // Each color was pushed twice per node, except for the first and last which accounted for 1 push (so * 2 - 2)
        if (way != myWay) numberOfBytesBefore += way->nodes.size() * 2 - 2;
        // we found our way! get out.
        else break;
    }
    //Offset is thus numberOfNodesBefore * sizeof(vec3) * sizeof(float)
    numberOfBytesBefore *= 3 * 4;

    // We want to write as many vec3's as there are nodes (-1) in the way
    int numberOfBytesToWrite = (myWay->nodes.size()*2 - 2 ) * 3 * 4;

    //Create the data to write
    vector<vec3> toWrite;
    for (int i = 0; i < (myWay->nodes.size() * 2) - 2; i++) toWrite.push_back(vec3(1.f,1.f,1.f));

    // push it to the buffer
    glNamedBufferSubData(glidWaysColors, numberOfBytesBefore, numberOfBytesToWrite, toWrite.data());

    selectedWay.way = myWay;
    selectedWay.numberOfBytesBefore = numberOfBytesBefore;
    selectedWay.numberOfBytesToWrite = numberOfBytesToWrite;
}

void Game::populateTypeColorArray(){

    typeColorMap.emplace(OSMElement::HIGHWAY_TRUNK, vec3(1,1,1));
    typeColorMap.emplace(OSMElement::HIGHWAY_PRIMARY, vec3(0.9,0.9,0.9));
    typeColorMap.emplace(OSMElement::HIGHWAY_SECONDARY, vec3(0.8,0.8,0.8));
    typeColorMap.emplace(OSMElement::HIGHWAY_TERTIARY, vec3(0.7,0.7,0.7));
    typeColorMap.emplace(OSMElement::HIGHWAY_RESIDENTIAL, vec3(0.6,0.6,0.6));
    typeColorMap.emplace(OSMElement::HIGHWAY_UNCLASSIFIED, vec3(0.5,0.5,0.5));
    typeColorMap.emplace(OSMElement::HIGHWAY_SERVICE, vec3(0.4,0.4,0.4));
    typeColorMap.emplace(OSMElement::BUILDING_UNMARKED, vec3(0,0,1));
    typeColorMap.emplace(OSMElement::BUILDING_SCHOOL, vec3(0,0,1));
    typeColorMap.emplace(OSMElement::BUILDING_ADDRINTERP, vec3(0.4,1,1));
    typeColorMap.emplace(OSMElement::RAILWAY_SUBWAY, vec3(1,0,1));
    typeColorMap.emplace(OSMElement::NATURAL_WOOD, vec3(0,0.5,0));
    typeColorMap.emplace(OSMElement::NATURAL_WATER, vec3(0,0,0.5));
    typeColorMap.emplace(OSMElement::LEISURE_PARK, vec3(0,1,0));
    typeColorMap.emplace(OSMElement::ADDR_INTERPOLATION, vec3(1,0,0));
//    typeColorMap.emplace(OSMElement::CONTOUR, vec3(0));
    typeColorMap.emplace(OSMElement::GRID_LINE, vec3(0.4,0.4,0.4));
    typeColorMap.emplace(OSMElement::aDEBUG, vec3(1.0,0,1.0));

//    /*


    // for (auto it = typeColorMap.begin; it != typeColorMap.end; it++){
        // displayElementType.emplace(it->first, true);
    // }
//*/
    displayElementType.emplace(OSMElement::HIGHWAY_TRUNK, true);
    displayElementType.emplace(OSMElement::HIGHWAY_PRIMARY, true);
    displayElementType.emplace(OSMElement::HIGHWAY_SECONDARY, true);
    displayElementType.emplace(OSMElement::HIGHWAY_TERTIARY, true);
    displayElementType.emplace(OSMElement::HIGHWAY_RESIDENTIAL, true);
    displayElementType.emplace(OSMElement::HIGHWAY_UNCLASSIFIED, true);
    displayElementType.emplace(OSMElement::HIGHWAY_SERVICE, true);
    displayElementType.emplace(OSMElement::BUILDING_UNMARKED, true);
    displayElementType.emplace(OSMElement::BUILDING_SCHOOL, true);
    displayElementType.emplace(OSMElement::BUILDING_ADDRINTERP, true);
    displayElementType.emplace(OSMElement::RAILWAY_SUBWAY, true);
    displayElementType.emplace(OSMElement::NATURAL_WOOD, true);
    displayElementType.emplace(OSMElement::NATURAL_WATER, true);
    displayElementType.emplace(OSMElement::LEISURE_PARK, true);
    displayElementType.emplace(OSMElement::ADDR_INTERPOLATION, true);
//    typeColorMap.emplace(OSMElement::CONTOUR, vec3(0));
    displayElementType.emplace(OSMElement::GRID_LINE, true);
    displayElementType.emplace(OSMElement::aDEBUG, true);
}

static bool deleteAll( OSMElement * theElement ) { delete theElement; return true; }


void Game::generateGridLines( vector<vec3> *groundTrianglesPositions, vector<vec2> *groundTrianglesUV){

    double step = 0.001;

    int lonCounter = 0;
    int latCounter = 0;
    std::vector<Way*> latitudeLines;
    std::vector<Way*> longitudeLines;

    // Start by making horizontal lines
    for( double lon = viewRectLeft; lon <= viewRectRight; lon += step){
        Way* way = new Way();

        way->eType = OSMElement::GRID_LINE;
        latCounter = 0;

//        For each line generate a bunch of nodes
        for (double lat = viewRectBottom; lat <= viewRectTop; lat += step){

            Node* n = new Node();
            n->id = to_string(static_cast<long double>(lon)).append(",").append(to_string(static_cast<long double>(lat)));
            n->longitude = lon;
            n->latitude = lat;
            n->elevation = (contourLineInterpolate(vec2(n->longitude, n->latitude)));
//            n->elevation = 0;

            way->addRef(n);
            latCounter++;
        }
        way->id = string("LON").append(to_string(static_cast<long double>((way->nodes[0]->longitude))));
        longitudeLines.push_back(way);
        lonCounter++;
    }
    PRINTINT(unsuccessfulInterpolations);
    // Then, create as many vertical lines as there are nodes in the previous horizontal lines
    for (int i = 0; i < latCounter; i++){
        Way* way = new Way();
        way->eType = OSMElement::GRID_LINE;

        // For ith vertical line grab the ith nodes in each horizontal line previously created
        for (int j = 0; j < lonCounter; j++) way->addRef(longitudeLines[j]->nodes[i]);

        way->id = string("LAT").append(to_string(static_cast<long double>((way->nodes[0]->latitude))));
        latitudeLines.push_back(way);

    }

//     Add it all to the way database
//    for (int i = 0; i < latitudeLines.size(); i++){
//        theWays.emplace(latitudeLines[i]->id, latitudeLines[i]);
//    }
//    for (int i = 0; i < longitudeLines.size(); i++){
//        theWays.emplace(longitudeLines[i]->id, longitudeLines[i]);
//    }

    for (int i = 0; i < latCounter-1; i++){
        for (int j = 0; j < lonCounter-1; j++){
            Way* way = new Way();

            way->addRef(latitudeLines[i]->nodes[j]);
            way->addRef(latitudeLines[i]->nodes[j+1]);
            way->addRef(latitudeLines[i+1]->nodes[j]);
            way->addRef(latitudeLines[i]->nodes[j]);

            way->eType = OSMElement::GRID_LINE;

            way->id = string("TRI1_").append(std::to_string(i)).append(std::to_string(j));
            theWays.emplace(way->id, way);

            groundTrianglesPositions->push_back(way->nodes[0]->getLatLongEle());
            groundTrianglesPositions->push_back(way->nodes[1]->getLatLongEle());
            groundTrianglesPositions->push_back(way->nodes[2]->getLatLongEle());
            groundTrianglesUV->push_back(vec2(0,0));
            groundTrianglesUV->push_back(vec2(1,0));
            groundTrianglesUV->push_back(vec2(0,1));
            way = new Way();

            way->addRef(latitudeLines[i]->nodes[j+1]);
            way->addRef(latitudeLines[i+1]->nodes[j+1]);
            way->addRef(latitudeLines[i+1]->nodes[j]);
            way->addRef(latitudeLines[i]->nodes[j+1]);

            way->eType = OSMElement::GRID_LINE;

            way->id = string("TRI2_").append(std::to_string(i)).append(std::to_string(j));
            theWays.emplace(way->id, way);

            groundTrianglesPositions->push_back(way->nodes[0]->getLatLongEle());
            groundTrianglesPositions->push_back(way->nodes[1]->getLatLongEle());
            groundTrianglesPositions->push_back(way->nodes[2]->getLatLongEle());
            groundTrianglesUV->push_back(vec2(1,0));
            groundTrianglesUV->push_back(vec2(1,1));
            groundTrianglesUV->push_back(vec2(0,1));
//            if ( j == 1) break;
        }
//        if ( i == 1) break;
    }

//    latitudeLines.remove_if(deleteAll);

}

inline double lerp(double a, double b, double t){ return a * t + b * (1 - t); }

double Game::contourLineInterpolate(vec2 xy){
    // Get closest contour lines to xy
    vector<double> distances;
    vector<vec2> directions;
    vector<std::pair<Node*, Node*>> nodePairs;
    vector<Way*> closests = findNClosestWays(5, xy, OSMElement::CONTOUR, distances, directions,nodePairs);

//    int lerpedWay = 1;
    int lerpedWay = getLerpedContourLines(xy, closests, directions, nodePairs);
    if (lerpedWay != -1) closests[1] = closests[lerpedWay];
    else{
        lerpedWay = 1;
        return stod(closests[0]->getValue("ele")) / 15000;
        unsuccessfulInterpolations++;
//        return 0;
    }

    // prepare for lerping
    double sum = distances[0] + distances[lerpedWay];
    double t = 1 - distances[0] / sum;

    // get found contour lines elevations
    std::vector<double> heights;
    heights.push_back(stod(closests[0]->getValue("ele")));
    heights.push_back(stod(closests[lerpedWay]->getValue("ele")));

    double result = lerp(heights[0], heights[1], t) / 15000;
    return result;
}

/**
 * @brief Game::getLerpedContourLines
 * @param xy
 * @param ways
 * @param directions
 * @param nodePairs
 * @return
 */
int Game::getLerpedContourLines(vec2 xy, vector<Way*> ways, vector<vec2> directions, vector<std::pair<Node*, Node*>> nodePairs){


    //Linesegment between our search point and the closest point on the candidate contour line.
    LineSegment L0;
    L0.p0 = xy;

//    PRINTVEC2VECTOR(directions);
    //Iterate on all candidate contours
    for (int i = 0; i < ways.size(); i++){

        L0.p1 = xy + directions[i];

        if (i == 0){
//            Way* way = new Way(string("DEBUG_").append(glm::to_string(xy)).append(std::to_string(i)), L0, OSMElement::aDEBUG);
//            theWays.emplace(way->id, way);
            continue;
        }

        //running linesegment on the absolute known nearest contour
        LineSegment L1;

        // The running nodes forming segments on the known nearest contour
        Node* n0;
        Node* n1;

        // needed to start actually counting at 1
        int counter = 1;

        int numOfIntersections = 0;
        bool intersects = false;
        bool first = true;
        // Loop on all nodes on the best contour
        for (auto nodeIt = ways[0]->nodes.begin() ; nodeIt != ways[0]->nodes.end(); ++nodeIt){
            if (first){
                n0 = *nodeIt;
                first = false;
                continue;
            }

            if ((counter % 2) == 0) n0 = n1; //start a new linesegment
            else{
                n1 = *nodeIt;

                L1.p0 = vec2(n0->longitude, n0->latitude);
                L1.p1 = vec2(n1->longitude, n1->latitude);
//                PRINTELEMENT(L0);
//                PRINTELEMENT(L1);
                if (L0.intersects(L1)){
//                    PRINTSTRING("INTERSECTS!");
                    intersects = true;
                    numOfIntersections++;
//                    break; // we now know this way isnt viable as it intersected the nearest. we can stop looping around
                }
            }

            counter = (counter + 1) % 2;
        }
//        if (!intersects) return i;
        // an even number of intersections means we were inside the polygon. maybe?
//        if ((numOfIntersections % 2) == 0){
         if (!intersects){
//            Way* way = new Way(string("DEBUG_").append(glm::to_string(xy)).append(std::to_string(i)), L0, OSMElement::aDEBUG);
//            theWays.emplace(way->id, way);
            return i;
        }
    }
    L0.p1 = xy + directions[1];
//    Way* way = new Way(string("DEBUG_").append(glm::to_string(xy)).append(std::to_string(1)), L0, OSMElement::aDEBUG);
//    theWays.emplace(way->id, way);
    return -1; // method didnt really work .revert to default.
}

// Convert the address interpolation ways into building polygons
void Game::extrudeAddrInterps(){
    int counter = 0;

    // Iterate on all ways
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;

        // Keep only ADDR_INTERPOLATIONs
        if (way->eType != OSMElement::ADDR_INTERPOLATION) continue;

        vector<Node*> nodes;
        // Store this way's 2 nodes
        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt) nodes.push_back(*nodeIt);

        // We only want to look at roads
        int filter = OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_PRIMARY | OSMElement::HIGHWAY_TRUNK | OSMElement::HIGHWAY_SERVICE | OSMElement::HIGHWAY_UNCLASSIFIED;

        // Preparing road query...
        vector<vector<vec2> > directions;
        directions.push_back(vector<vec2>());
        directions.push_back(vector<vec2>());

        vector<std::pair<Node*, Node*>> nodePairs;
        vector<vector<double> > distances;
        distances.push_back(vector<double>());
        distances.push_back(vector<double>());

        // Find the closest roads to this ADDR_INTERP
        vector<Way*> firstNodeWays = findNClosestWays(5, vec2(nodes[0]->longitude, nodes[0]->latitude), filter, distances[0], directions[0], nodePairs);
        vector<Way*> secondNodeWays = findNClosestWays(5, vec2(nodes[1]->longitude, nodes[1]->latitude), filter, distances[1], directions[1], nodePairs);

        LineSegment L0(nodes[0], nodes[1]);

        bool intersectOtherRoad = false;
        //Check if the adress way intersects any of these roads
        for (int i = 0 ; i < firstNodeWays.size(); i++){
            LineSegment L1(firstNodeWays[i]);

            if (!intersectOtherRoad && L0.intersects(L1)) intersectOtherRoad = true;
        }

        for (int i = 0 ; i < secondNodeWays.size(); i++){
            LineSegment L1(secondNodeWays[i]);

            if (!intersectOtherRoad && L0.intersects(L1)) intersectOtherRoad = true;
        }

//        if (intersectOtherRoad) continue;

        // Find the closest way that both nodes share (should generally be the one parallel to the addr_interp)
        // Maybe better to look at dot products? This works ok for now
        pair<int, int> commonIndices = findCommonWay(firstNodeWays, secondNodeWays);

        int i = commonIndices.first;
        int j = commonIndices.second;

        // Multiplicative factor related to the polygon size
        double factor = 1;

        //If finding the common way didnt work just move on
        if (i == -1) continue;

        // Move on to the next if the common road is too far from one of the nodes
        if (distances[0][i] > 0.0002 * factor) continue;
        if (distances[1][j] > 0.0002 * factor) continue;

        // Move on to next if the nodes are on different sides of the common road
        if (dot(directions[0][i], directions[1][j]) < 0) continue;


        // Create the new nodes by moving from each edge of the ADDR_INTERP, perpendicularly to the road
        Node* node3 = new Node();
        node3->id = string("ADDR_INTERP_NODE3_").append(std::to_string(counter));
        node3->longitude = nodes[0]->longitude - directions[0][i].x * factor;
        node3->latitude = nodes[0] -> latitude - directions[0][i].y * factor;

        Node* node4 = new Node();
        node4->id = string("ADDR_INTERP_NODE4_").append(std::to_string(counter));
        node4->longitude = nodes[1]->longitude - directions[0][i].x * factor;
        node4->latitude = nodes[1]->latitude - directions[0][i].y * factor;

        // Create the new building
        Way* buildingWay  = new Way();
        buildingWay->addRef(nodes[1]);
        buildingWay->addRef(node4);
        buildingWay->addRef(node3);
        buildingWay->addRef(nodes[0]);
        buildingWay->addRef(nodes[1]);
        buildingWay->eType = OSMElement::BUILDING_ADDRINTERP;
        buildingWay->id = string("ADDR_INTERP_BUILDING").append(std::to_string(counter));

        // Store the new building
        theWays.emplace(buildingWay->id, buildingWay);
        counter++;
    }

    PRINTSTRING("Extruding address interpolation into polygons");
    PRINTINT(counter);
}

pair<int, int> Game::findCommonWay(vector<Way*> firstNodeWays, vector<Way*> secondNodeWays){
    for (int i = 0; i < firstNodeWays.size(); i++){
        for (int j = 0; j < secondNodeWays.size(); j++){
            if (firstNodeWays[i]->id.compare(secondNodeWays[j]->id) == 0) return pair<int, int>(i,j);
        }
    }
    PRINTSTRING("Couldnt find suitable similar ways!");
    return pair<int, int>(-1,-1);
}


