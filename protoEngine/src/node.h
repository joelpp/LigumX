#ifndef NODE_H
#define NODE_H
#include <string>
#include <unordered_map>
#include "osm_element.h"

class Node: public OSMElement{
private:

public:
    float latitude;
    float longitude;
    double elevation;
    Node();
    Node(std::string id, float longitude, float latitude);
    std::string toString();
    bool operator == (Node& n) const {
       return latitude==n.latitude && longitude==n.longitude;
    }
};

#endif // NODE

