#ifndef RELATION
#define RELATION
#include <vector>
#include "OSMElement.h"
class Relation: public OSMElement{
private:
    std::vector<std::string> members;

public:
    Relation();
    Relation(std::string id);

    void addMember(std::string);
};

#endif // RELATION

