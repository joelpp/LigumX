#ifndef WAY_H
#define WAY_H

#include "node.h"
#include "osm_element.h"
#include "linesegment.h"
#include <vector>
#include <string>

class LineSegment;

class Way : public OSMElement{
private:

public:
    Way();
    Way(std::string id);
    Way(std::string id, LineSegment ls, int type);
    void addRef(Node* ref);



    std::vector<Node*> nodes;
    std::string toString();

    bool selectable;


};

#endif // WAY

