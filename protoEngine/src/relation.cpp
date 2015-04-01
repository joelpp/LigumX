#include "relation.h"

Relation::Relation(){

}

Relation::Relation(std::string id){
    this->id = id;

}

void Relation::addMember(std::string id){
    members.push_back(id);
}

std::string Relation::toString(){
    return "Unimplemented method :(";
}
