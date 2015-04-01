#ifndef WAY_H
#define WAY_H

#include "node.h"
#include "OSMElement.h"
#include <vector>
#include <string>

class Way : public OSMElement{
private:
    std::string id;

public:
    Way();
    Way(std::string id);
    void addRef(Node* ref);
    std::string toString();
    std::vector<Node*> nodes;
};

#endif // WAY

