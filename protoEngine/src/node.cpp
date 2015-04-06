#include "node.h"
#include <iostream>
#include <stdio.h>
Node::Node(){


}

Node::Node(std::string _id, float _latitude, float _longitude){
    this->id = _id;
    this->latitude = _latitude;
    this->longitude = _longitude;
//    this->tags = unordered_map<std::string, std::string>();
}


std::string Node::toString(){
    char str[200];
    sprintf_s(str, "Node ID=%s lat=%f lon=%f", this->id.c_str(), this->latitude, this->longitude);
    std::string toReturn = std::string(str);
    for ( auto it = this->tags.begin(); it != this->tags.end(); ++it ){
        char tagstr[100];
        sprintf_s(tagstr, " | k: %s v: %s |", it->first.c_str(), it->second.c_str());
        toReturn.append(std::string(tagstr));
    }
    return toReturn;
}