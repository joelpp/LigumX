#ifndef OSMELEMENT
#define OSMELEMENT
#include "Logging.h"

#include <unordered_map>
#include <vector>
#include "common.h"

class Way;
class Node;


class OSMElement{
private:
  REGISTERCLASS(OSMElement);

public:

    enum ElementType { HIGHWAY_TRUNK = 1 << 0,
                       HIGHWAY_PRIMARY = 1 << 1,
                       HIGHWAY_SECONDARY = 1 << 2,
                       HIGHWAY_TERTIARY = 1 << 3,
                       HIGHWAY_RESIDENTIAL = 1 << 4,
                       HIGHWAY_SERVICE = 1 << 5,
                       HIGHWAY_UNCLASSIFIED = 1 << 6,
                       SIDEWALK = 1 << 7, 
                       CONTOUR = 1 << 10,

                       BUILDING_UNMARKED = 1 << 15,
                       BUILDING_SCHOOL = 1 << 16,
                       BUILDING_ADDRINTERP = 1 << 17,

                       BOUNDARY = 1 << 21,
                       LEISURE_PARK = 1 << 22,
                       NATURAL_WOOD = 1 << 23,
                       NATURAL_WATER = 1 << 24,

                       LANDUSE = 1 << 25,

                       RAILWAY_SUBWAY = 1 << 26,
                       aDEBUG = 1 << 27,
                       ADDR_INTERPOLATION = 1 << 28,
                       NOT_IMPLEMENTED = 1 << 29,
                       ANY_TYPE = 1 << 30,
                       GRID_LINE = 1 << 31} ;



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

    std::string getValue(std::string key);

    virtual std::string toString() = 0;

    std::vector<OSMElement*> parents;

    static ElementType typeFromStrings(std::string key, std::string value);    

};
namespace std
{
template<>
struct hash<OSMElement::ElementType> {
    size_t operator()(const OSMElement::ElementType &et) const {
        return std::hash<int>()(et);
    }
};
}

#endif // OSMELEMENT

