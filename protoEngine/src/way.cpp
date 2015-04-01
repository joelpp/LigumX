#include "way.h"

Way::Way(){

}

Way::Way(std::string id){
    this->id = id;
}


void Way::addRef(Node* ref){
    nodes.push_back(ref);
}
std::string Way::toString(){
    char str[200];
    sprintf_s(str, "Way ID=%s ", this->id.c_str());
    std::string toReturn = std::string(str);
    for ( auto it = this->tags.begin(); it != this->tags.end(); ++it ){
        char tagstr[200];
        sprintf_s(tagstr, " | k: %s v: %s |", it->first.c_str(), it->second.c_str());
        toReturn.append(std::string(tagstr));
    }
    return toReturn;
}
