#ifndef WAY_H
#define WAY_H

#include "node.h"
#include "osm_element.h"
#include <vector>
#include <string>

class Way : public OSMElement{
private:

public:
    Way();
    Way(std::string id);
    void addRef(Node* ref);



    std::vector<Node*> nodes;
    std::string toString();

    bool selectable;


};

#endif // WAY

