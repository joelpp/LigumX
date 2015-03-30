#ifndef NODE_H
#define NODE_H
#include <string>
#include <unordered_map>
#include "member.h"

class Node: public Member{
private:
    float latitude;
    float longitude;

public:

    Node();
    Node(std::string id, float latitude, float longitude);
    std::string toString();
};

#endif // NODE

