#include "stdafx.h"
#include "node.h"

#include "CurlRequest.h"
#include "Heightfield.h"
#include "LigumX.h"
#include "osm_element.h"
#include "Sector.h"
#include "Settings.h"
#include "SectorData.h"
#include "SectorManager.h"
#include "RenderDataManager.h"
#include "way.h"
#include "World.h"

#include <sstream>
#include <fstream>

using namespace std;
using namespace glm;

OSMElement::ElementType typeFromStrings(string key, string value);



SectorData::SectorData()
{

}

SectorData::SectorData(glm::vec2 pos)
{
 
    m_pos = pos;
}

void SectorData::downloadData(std::string path)
{
    //string s = downloadSectorData(m_pos);

    //std::ofstream out(path);

    //out << s;
    //out.close();
}

void SectorData::loadData(EOSMDataType dataType)
{
    World *world = LigumX::GetInstance().world;
    glm::vec2 coordinateShifting = Settings::GetInstance().f2("coordinateShifting");
    tinyxml2::XMLDocument doc;

    std::string path = BuildXMLPath(dataType, m_pos);

    //if (!file_exists(path)) 
    //{
    //    if (dataType == MAP)
    //    {
    //        downloadData(path);
    //    }
    //    else if (dataType == CONTOUR)
    //    {
    //        downloadContourData(m_pos, path);
    //    }
    //}

    doc.LoadFile(path.c_str());

    tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();
    // std::unordered_map<std::string id, Way*>* wayVector = 0;
    // if (dataType == CONTOUR)
    // {
    //      wayVector = &contourWays;
    // }
    // else
    // {
    //      wayVector = &ways;
    // }

    // TODO: This could be part of a test?
    // cout << docRoot->Value() << "\n";
    
    int i = 0;
    for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        if (string(child->Value()).compare("bound") == 0){

        }
        if (string(child->Value()).compare("node") == 0){
//            cout << "Looking at a node \n";
//            cout << child->ToElement()->FindAttribute("id")->Value() << "\n";

            string id = child->ToElement()->FindAttribute("id")->Value();
            float longitude = atof(child->ToElement()->FindAttribute("lon")->Value()) + coordinateShifting.x;
            float latitude = atof(child->ToElement()->FindAttribute("lat")->Value()) + coordinateShifting.y;
            //double latitude = strtod(child->ToElement()->FindAttribute("lat")->Value(), NULL);
            //double longitude = strtod(child->ToElement()->FindAttribute("lon")->Value(), NULL);

            Node* node = new Node(id, longitude, latitude);
            node->elevation = 0;
           // if (path.compare("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/result.xml") == 0) node->elevation = contourLineInterpolate(vec2(node->longitude, node->latitude)) * 1.0001;
            for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement()){
                string key = tag->ToElement()->FindAttribute("k")->Value();
                string value = tag->ToElement()->FindAttribute("v")->Value();
                node -> addTag(key, value);
            }
            
            // /** UGLY 8*/
            // Sector* sl = world->sectorsAroundPoint((glm::vec2(longitude, latitude)), 0)->at(0);
            // PRINTINT(sl);
            // PRINTINT(sl->m_data);
            // sl->m_data->
            nodes.emplace(id, node);
            // theNodes.emplace(id, node);
            // addPoint(spatialIndex, longitude, latitude, atoi(id.c_str()));
            // i++;
        }

        else if (string(child->Value()).compare("way") == 0){
//            cout << "Looking at a way \n";
            string id = child->ToElement()->FindAttribute("id")->Value();
            Way* way = new Way(id);
            way->eType = OSMElement::NOT_IMPLEMENTED;
            for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement()){
                if (string(way_child->Value()).compare("nd") == 0){
                    string ref = way_child->ToElement()->FindAttribute("ref")->Value();
                    way -> addRef(nodes[ref]);
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

            if (dataType == CONTOUR)
            {
                float elevation = atof(way->tags["ele"].c_str()) / 15000 + 0.000001;
                for (auto it = way->nodes.begin(); it != way->nodes.end(); ++it)
                {
                    Node* node = *it;
                    node->elevation = elevation;
                }
            }
            ways.emplace(id, way);
            // theWays.emplace(id, way);
            // waysTypeMap[way->eType].emplace(id,way);
        }
//         else if (string(child->Value()).compare("relation") == 0){
//             string id = child->ToElement()->FindAttribute("id")->Value();
//             Relation *relation = new Relation(id);

//             for (tinyxml2::XMLNode* relation_child = child->FirstChildElement(); relation_child != NULL; relation_child = relation_child->NextSiblingElement()){
//                 if (string(relation_child->Value()).compare("member") == 0){
//                     string type = relation_child->ToElement()->FindAttribute("type")->Value();
//                     string elementId = relation_child->ToElement()->FindAttribute("ref")->Value();
// //                    PRINT(type);
// //                    PRINT(elementId);
//                     //Odd. Ucommenting this crashes the program when trying to fill the buffers.
//                     try{
//                         if (type.compare("node") == 0) relation->addMember(theNodes.at(elementId));
//                         else if (type.compare("way") == 0) relation->addMember(theWays.at(elementId));
//                         else if (type.compare("relation") == 0) relation->addMember(theRelations.at(elementId));
//                     }
//                     catch(...){

//                     }
//                 }
//                 else if (string(relation_child->Value()).compare("tag") == 0){
//                     string key = relation_child->ToElement()->FindAttribute("k")->Value();
//                     string value = relation_child->ToElement()->FindAttribute("v")->Value();
//                     relation->addTag(key, value);
//                 }
//             }
//             theRelations.emplace(id, relation);
//         }

    }


}

void SectorData::elevateNodes(Heightfield* heightfield)
{
    for (auto it = ways.begin(); it != ways.end(); ++it)
    {
        Way* way = it->second;

        for (auto nodeIt = way->nodes.begin(); nodeIt != way->nodes.end(); ++nodeIt)
        {
            Node* node = *nodeIt;
            double elevation = heightfield->getHeight(node->getLatLong());
//            elevation = max(0., min(elevation, 0.01));
            node->elevation = elevation + 0.0001;
        }
    }
}


std::string SectorData::BuildXMLPath(EOSMDataType dataType, glm::vec2 pos){
    std::stringstream savePath;
	//savePath << "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/";
	savePath << "C:/Users/Joel/Documents/LigumX/LigumX/data/";
    switch(dataType)
    {
        case CONTOUR:   savePath << "SRTMData/";
                        break;
        case MAP:       savePath << "OSMData/";
    }

	// todo: call instance of sector manager or something
    //int index = SectorManager::IDFromPos(pos);
	int index = -1;
    // savePath << m_pos.x * 1000 << "x" << m_pos.y * 1000;
    savePath << index;
    savePath << ".xml";

    return savePath.str();
}



OSMElement::ElementType typeFromStrings(string key, string value){
    if (key.compare("highway") == 0){
        if (value.compare("trunk") == 0) return OSMElement::HIGHWAY_TRUNK;
        else if (value.compare("primary") == 0) return OSMElement::HIGHWAY_PRIMARY;
        else if (value.compare("secondary") == 0) return OSMElement::HIGHWAY_SECONDARY;
        else if (value.compare("tertiary") == 0) return OSMElement::HIGHWAY_TERTIARY;
        else if (value.compare("residential") == 0) return OSMElement::HIGHWAY_RESIDENTIAL;
        else if (value.compare("service") == 0) return OSMElement::HIGHWAY_SERVICE;
        else if (value.compare("unclassified") == 0) return OSMElement::HIGHWAY_UNCLASSIFIED;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("natural") == 0){
        if (value.compare("wood") == 0) return OSMElement::NATURAL_WOOD;
        if (value.compare("water") == 0) return OSMElement::NATURAL_WATER;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("building") == 0){
        if (value.compare("yes") == 0) return OSMElement::BUILDING_UNMARKED;
        else if (value.compare("school") == 0) return OSMElement::BUILDING_SCHOOL;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("contour") == 0) return OSMElement::CONTOUR;
    else if (key.compare("leisure") == 0){
        if (value.compare("park") == 0) return OSMElement::LEISURE_PARK;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("addr:interpolation") == 0){ return OSMElement::ADDR_INTERPOLATION; }
    else if (key.compare("landuse") == 0){ return OSMElement::LANDUSE; }
    else if (key.compare("boundary") == 0){ return OSMElement::BOUNDARY; }

    else return OSMElement::NOT_IMPLEMENTED;
}

//TODO: I feel this should be more a static "find n closest ways in this list i carefully prepared" rather than per-sector Data_
std::vector<Way*> SectorData::findNClosestWays(int n, glm::vec2 xy, int filter, vector<double> &distances, vector<glm::vec2> &_directions, vector<std::pair<Node*, Node*>> &_nodePairs){
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
    for ( auto it = ways.begin(); it != ways.end(); ++it ){
        Way* way = it->second;
//        if (!way->selectable) continue;

        // This way's type not yet implemented :( get to it!
        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

        // A specific filter has been chosen. Check if this way passes it!
        if (filter != OSMElement::ANY_TYPE)
            if ((way->eType & filter) == 0) continue;

        Node* firstNode = NULL;
        Node* secondNode = NULL;
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
                double dist = Math::pointLineSegmentDistance(xy, p2, p1, dir);

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

Node* SectorData::findClosestNode(vec2 xy){
    Node* closest = NULL;
    double bestDist = 99999;
    for ( auto it = nodes.begin(); it != nodes.end(); ++it ){
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



//TODO: Again, this should be more of a "look into _loaded_ ways" rather than into a single sector.
// std::vector<Way*> SectorData::getNearestWays(OSMElement::ElementType type,double lon,double lat,double maxResults){
//     std::vector<Way*> toReturn;

//     std::vector<SpatialIndex::IData*>* data = getNearest(waysIndex[type], lon, lat, maxResults);

//     for (SpatialIndex::IData* &item : (*data))
//         {
//             // get the generic shape object which we can cast to other types
//             SpatialIndex::IShape* shape;
//             item->getShape(&shape);

//             // cast the shape to a Point
//             SpatialIndex::Point center;
//             shape->getCenter(center);

//             //get ID of shape
//             id_type id = item->getIdentifier();
//             std::stringstream ssid;
//             ssid << id;
//             Way* way = waysTypeMap[type][ssid.str()];

//             toReturn.push_back(way);
//         }
//         return toReturn;
// }
