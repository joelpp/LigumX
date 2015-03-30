#ifndef RELATION
#define RELATION
#include <vector>
#include "member.h"
class Relation: public Member{
private:
    std::vector<std::string> members;

public:
    Relation();
    Relation(std::string id);

    void addMember(std::string);
};

#endif // RELATION

