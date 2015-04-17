#include "way.h"

Way::Way(){

}

Way::Way(std::string id){
    this->id = id;
}

Way::Way(std::string _id, LineSegment ls, int type){
    this->id = _id;

    addRef(new Node("n0", ls.p0.x, ls.p0.y));
    addRef(new Node("n1", ls.p1.x, ls.p1.y));
    eType = (OSMElement::ElementType) type;
}

void Way::addRef(Node* ref){
    nodes.push_back(ref);
}
std::string Way::toString(){
    char str[200];
    sprintf_s(str, "Way ID=%s, %d nodes", this->id.c_str(), nodes.size());
    std::string toReturn = std::string(str);
    for ( auto it = this->tags.begin(); it != this->tags.end(); ++it ){
        char tagstr[200];
        sprintf_s(tagstr, " | k: %s v: %s |", it->first.c_str(), it->second.c_str());
        toReturn.append(std::string(tagstr));
    }
    return toReturn;
}
