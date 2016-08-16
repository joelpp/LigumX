#include "stdafx.h"

#include "node.h"

Node::Node(){
    this->elevation = 0;

}

Node::Node(glm::vec2 latLong){
    this->elevation = 0;

}

Node::Node(std::string _id, float _longitude, float _latitude){
    this->id = _id;
    this->latitude = _latitude;
    this->longitude = _longitude;
    this->elevation = 0;
//    this->tags = unordered_map<std::string, std::string>();
}


std::string Node::toString(){
    char str[200];
    sprintf(str, "Node ID=%s lat=%f lon=%f", this->id.c_str(), this->latitude, this->longitude);
    std::string toReturn = std::string(str);
    for ( auto it = this->tags.begin(); it != this->tags.end(); ++it ){
        char tagstr[100];
        
        sprintf(tagstr, " | k: %s v: %s |", it->first.c_str(), it->second.c_str());
        toReturn.append(std::string(tagstr));
    }
    return toReturn;
}


glm::vec2 Node::getLatLong(){
    return glm::vec2(longitude, latitude);
}

glm::vec3 Node::getLatLongEle(){
    return glm::vec3(longitude, latitude, elevation);
}
