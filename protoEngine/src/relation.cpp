#include "relation.h"

Relation::Relation(){

}

Relation::Relation(std::string id){
    this->id = id;

}

void Relation::addMember(OSMElement* element){
    members.push_back(element);
}

std::string Relation::toString(){
    return "Unimplemented method :(";
}
