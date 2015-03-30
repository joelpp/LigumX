#ifndef WAY_H
#define WAY_H

#include "node.h"
#include "member.h"
#include <vector>
#include <string>

class Way : public Member{
private:
    std::string id;
    std::vector<std::string> nodes;

public:
    Way();
    Way(std::string id);
    void addRef(std::string ref);

};

#endif // WAY

