#ifndef OSMELEMENT
#define OSMELEMENT
#include <string>
#include <unordered_map>
#include "filter.h"
class OSMElement{
protected:

public:
    enum tag_value { HIGHWAY_TRUNK = 0 << 1,
                     HIGHWAY_PRIMARY = 1 << 1,
                     HIGHWAY_SECONDARY = 2 << 1,
                     HIGHWAY_TERTIARY = 3 << 1,
                     HIGHWAY_RESIDENTIAL = 4 << 1,
                     HIGHWAY_SERVICE = 5 << 1,
                     CONTOUR = 6 << 1,
                     BUILDING = 7 << 1,
                     LEISURE_PARK = 8 << 1,
                     NATURAL_WOOD = 9 << 1,
                     RAILWAY_SUBWAY = 10 << 1 };

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

