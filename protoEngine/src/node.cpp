#include "node.h"

Node::Node(){


}

Node::Node(std::string id, float latitude, float longitude){
    this->id = id;
    this->latitude = latitude;
    this->longitude = longitude;
//    this->tags = unordered_map<std::string, std::string>();
}


std::string toString(){
    return "lol";
}
