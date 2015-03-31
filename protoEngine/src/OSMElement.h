#ifndef OSMELEMENT
#define OSMELEMENT
#include <string>
#include <unordered_map>
class OSMElement{
protected:
    std::unordered_map<std::string, std::string> tags;

public:
    std::string id;

    OSMElement(){};
    void addTag(std::string key, std::string value){ this->tags.emplace(key, value); }

};

#endif // OSMELEMENT

