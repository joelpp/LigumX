#ifndef OSMELEMENT
#define OSMELEMENT
#include <string>
#include <unordered_map>

#include "common.h"
#include "filter.h"


class OSMElement{
protected:

public:

    enum ElementType { HIGHWAY_TRUNK = 1 << 0,
                       HIGHWAY_PRIMARY = 1 << 1,
                       HIGHWAY_SECONDARY = 1 << 2,
                       HIGHWAY_TERTIARY = 1 << 3,
                       HIGHWAY_RESIDENTIAL = 1 << 4,
                       HIGHWAY_SERVICE = 1 << 5,

                       CONTOUR = 1 << 6,

                       BUILDING_UNMARKED = 1 << 7,
                       BUILDING_SCHOOL = 1 << 8,

                       LEISURE_PARK = 1 << 9,
                       NATURAL_WOOD = 1 << 10,

                       RAILWAY_SUBWAY = 1 << 11,

                       NOT_IMPLEMENTED = 1 << 12,
                       ANY_TYPE = 1 << 13} ;

    std::unordered_map<std::string, std::string> tags;
    std::unordered_map<int, int> itags;

    ElementType eType;
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

