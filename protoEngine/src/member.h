#ifndef MEMBER
#define MEMBER
#include <string>
#include <unordered_map>
class Member{
protected:
    std::unordered_map<std::string, std::string> tags;
    std::string id;

public:
    Member(){};
    void addTag(std::string key, std::string value){ this->tags.emplace(key, value); }

};

#endif // MEMBER

