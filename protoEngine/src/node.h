#ifndef NODE_H
#define NODE_H
#include <string>
#include <unordered_map>
#include "OSMElement.h"

class Node: public OSMElement{
private:

public:
    float latitude;
    float longitude;

    Node();
    Node(std::string id, float latitude, float longitude);
    std::string toString();
    bool operator == (Node& n) const {
       return latitude==n.latitude && longitude==n.longitude;
    }
};

#endif // NODE

