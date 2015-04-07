#ifndef OSMELEMENT
#define OSMELEMENT
#include <string>
#include <unordered_map>
#include "filter.h"
class OSMElement{
protected:

public:
    std::unordered_map<std::string, std::string> tags;
    std::unordered_map<int, int> itags;

    std::string id;

    OSMElement(){};
    void addTag(std::string key, std::string value){ this->tags.emplace(key, value); }
    void addITag(int key, int value){ this->itags.emplace(key, value); }

    bool hasTag(std::string tag);
    bool hasITag(int tag);

    bool hasTagAndValue(std::string key, std::string value);
    bool hasITagAndValue(int key, int value);
    bool passesFilter(Filter f);

    virtual std::string toString() = 0;

    std::vector<OSMElement*> parents;
};


#endif // OSMELEMENT

