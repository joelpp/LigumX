#include "stdafx.h"
#include "node.h"

#include "CurlRequest.h"
#include "Heightfield.h"
#include "LigumX.h"
#include "osm_element.h"
#include "Sector.h"
#include "Settings.h"
#include "SectorData.h"
#include "EngineSettings.h"
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

SectorData::SectorData(CurlRequest* request)
{
	m_CurlRequest = request;
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
    tinyxml2::XMLDocument doc;

	const std::string& path = m_CurlRequest->GetFilename();

    doc.LoadFile(path.c_str());

    tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();

    for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
		std::string childValue = std::string(child->Value());
        if (childValue == "bound")
		{

        }

        else if (childValue == "node")
		{
            string id = child->ToElement()->FindAttribute("id")->Value();
            float longitude = atof(child->ToElement()->FindAttribute("lon")->Value());
            float latitude = atof(child->ToElement()->FindAttribute("lat")->Value());

            Node* node = new Node(id, longitude, latitude);
            node->elevation = 0;

            for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement())
			{
                string key = tag->ToElement()->FindAttribute("k")->Value();
                string value = tag->ToElement()->FindAttribute("v")->Value();
                node -> addTag(key, value);
            }

			glm::vec2 lonLat = glm::vec2(longitude, latitude);

			lonLat -= g_EngineSettings->GetStartLonLat();
			lonLat /= g_EngineSettings->GetExtent();


			glm::vec2 sectorIndex = glm::floor(lonLat);
			glm::vec2 startPos = g_EngineSettings->GetStartLonLat() + sectorIndex * g_EngineSettings->GetExtent();

			if (sectorIndex == glm::vec2(0, 0))
			{
			}
			else
			{
				Sector* sector = world->GetSector(startPos);

				if (sector)
				{

				}
				else
				{
					sector = new Sector(startPos);
					PRINTVEC2(sectorIndex);
					sector->SetOffsetIndex(sectorIndex);

					//RenderDataManager::InitializeSector(sector);

					world->sectors.push_back(sector);
				}

				//sector->nodes.emplace(id, node);
			}

			nodes.emplace(id, node);
		}

        else if (childValue == "way")
		{
            string id = child->ToElement()->FindAttribute("id")->Value();
            Way* way = new Way(id);
            way->eType = OSMElement::NOT_IMPLEMENTED;
            for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement())
			{
                if (string(way_child->Value()).compare("nd") == 0)
				{
                    string ref = way_child->ToElement()->FindAttribute("ref")->Value();
                    way->AddNode(nodes[ref]);
                }
                else if (string(way_child->Value()).compare("tag") == 0)
				{
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
                for (auto it = way->GetNodes().begin(); it != way->GetNodes().end(); ++it)
                {
                    Node* node = *it;
                    node->elevation = elevation;
                }
            }

            ways.emplace(id, way);
        }
    }
}

void SectorData::elevateNodes(Heightfield* heightfield)
{
    for (auto it = ways.begin(); it != ways.end(); ++it)
    {
        Way* way = it->second;

        for (auto nodeIt = way->GetNodes().begin(); nodeIt != way->GetNodes().end(); ++nodeIt)
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
        for (auto nodeIt = way->GetNodes().begin() ; nodeIt != way->GetNodes().end(); ++nodeIt){

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
