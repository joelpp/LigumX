#ifndef NODE_H
#define NODE_H
#include <string>
#include <unordered_map>

class Node{
private:
    std::string id;
    double latitude;
    double longitude;
    std::unordered_map<std::string, std::string> tags;
    std::vector<Way*> ways; //needed?

public:

    Node();
};

#endif // NODE

