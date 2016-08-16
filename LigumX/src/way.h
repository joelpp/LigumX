#ifndef WAY_H
#define WAY_H

#include "osm_element.h"
#include <vector>
#include "glm/glm.hpp"
#include <string>
#include <iostream>

class LineSegment;
class OSMElement;
class Node;
typedef std::pair<int, int> WayPair;

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
    bool hasPointInside(glm::vec2 xy);
    bool loops();

    static WayPair findCommon(const std::vector<Way*> firstNodeWays, const std::vector<Way*> secondNodeWays);
};

#endif // WAY

