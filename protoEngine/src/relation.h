#ifndef RELATION
#define RELATION
#include <vector>
#include "OSMElement.h"
class Relation: public OSMElement{
private:

public:
    std::vector<OSMElement*> members;

    Relation();
    Relation(std::string id);

    void addMember(OSMElement* element);
    std::string toString();

};

#endif // RELATION

