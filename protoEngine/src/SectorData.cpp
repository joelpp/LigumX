#include "SectorData.h"
// #include "game.cpp"
using namespace std;

OSMElement::ElementType typeFromStrings(string key, string value);

SectorData::SectorData(){

}

SectorData::SectorData(glm::vec2 pos){
    tinyxml2::XMLDocument doc;

    std::string path = BuildXMLPath(pos);

    doc.LoadFile(path.c_str());

    tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();

    cout << docRoot->Value() << "\n";
    int i = 0;
    for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        if (string(child->Value()).compare("bound") == 0){
            // string box = child->ToElement()->FindAttribute("box")->Value();
            // std::istringstream ss(box);
            // std::string token;

            // std::getline(ss, token, ','); viewRectBottom = atof(token.c_str()) * coordinateInflationFactor - 45;
            // std::getline(ss, token, ','); viewRectLeft = atof(token.c_str()) * coordinateInflationFactor   + 73;
            // std::getline(ss, token, ','); viewRectTop = atof(token.c_str()) * coordinateInflationFactor    - 45;
            // std::getline(ss, token, ','); viewRectRight = atof(token.c_str()) * coordinateInflationFactor  + 73;

            // PRINT(viewRectBottom);
            // PRINT(viewRectLeft);
            // PRINT(viewRectTop);
            // PRINT(viewRectRight);


            // sector = world->getOrCreateSector(viewRectBottomLeft);
        }
        if (string(child->Value()).compare("node") == 0){
//            cout << "Looking at a node \n";
//            cout << child->ToElement()->FindAttribute("id")->Value() << "\n";

            string id = child->ToElement()->FindAttribute("id")->Value();
            float latitude = atof(child->ToElement()->FindAttribute("lat")->Value()) - 45;
            float longitude = atof(child->ToElement()->FindAttribute("lon")->Value()) + 73;
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


std::string SectorData::BuildXMLPath(glm::vec2 pos){
    std::stringstream ss;
#ifdef __APPLE__
    ss << "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/";
#else
    ss << "../data/Data_";
#endif
    // ss<<pos.x * 1000<<"x"<<pos.y*1000<<".xml";
    ss<< 0<<".xml";
    // std::cout << ss.str() << "\n";

    return ss.str();
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


