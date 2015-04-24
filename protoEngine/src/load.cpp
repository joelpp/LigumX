
#include "game.h"
#include <sstream>


using namespace std;
using namespace glm;

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


