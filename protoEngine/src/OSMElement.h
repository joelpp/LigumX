#ifndef OSMELEMENT
#define OSMELEMENT
#include <string>
#include <unordered_map>
class OSMElement{
protected:

public:
    std::unordered_map<std::string, std::string> tags;

    std::string id;

    OSMElement(){};
    void addTag(std::string key, std::string value){ this->tags.emplace(key, value); }
    bool hasTag(std::string tag);
    bool hasTagAndValue(std::string key, std::string value);
    virtual std::string toString() = 0;
};


#endif // OSMELEMENT

