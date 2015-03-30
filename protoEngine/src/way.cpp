#include "way.h"

Way::Way(){

}

Way::Way(std::string id){
    this->id = id;
}


void Way::addRef(std::string ref){
    nodes.push_back(ref);
}
