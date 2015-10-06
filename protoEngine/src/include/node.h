#ifndef NODE_H
#define NODE_H

#include <unordered_map>

#include "glm/glm.hpp"

#include "Logging.h"
#include "osm_element.h"

class Node: public OSMElement{
private:

public:
    float latitude;
    float longitude;
    double elevation;
    Node();
    Node(glm::vec2 latLong);
    Node(std::string id, float longitude, float latitude);
    std::string toString();
    glm::vec2 getLatLong();
    glm::vec3 getLatLongEle();
    bool operator == (Node& n) const {
       return latitude==n.latitude && longitude==n.longitude;
    }
};

#endif // NODE

