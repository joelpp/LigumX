#ifndef OSMELEMENT
#define OSMELEMENT
#include "Logging.h"

#include <unordered_map>
#include <vector>
#include "common.h"

#pragma region  FORWARD_DECLARATIONS OSMElement
#include "property.h"

class OSMElement;

enum OSMElementType
{
OSMElementType_HighwayTrunk,
OSMElementType_HighwayPrimary,
OSMElementType_HighwaySecondary,
OSMElementType_HighwayTertiary,
OSMElementType_HighwayResidential,
OSMElementType_HighwayService,
OSMElementType_HighwayUnclassified,
OSMElementType_Sidewalk,
OSMElementType_Contour,
OSMElementType_Building_Unmarked,
OSMElementType_Building_School,
OSMElementType_Building_Addressinterpolation,
OSMElementType_Boundary,
OSMElementType_LeisurePark,
OSMElementType_NaturalWood,
OSMElementType_NaturalWater,
OSMElementType_NaturalBareRock,
OSMElementType_LanduseRetail,
OSMElementType_LanduseIndustrial,
OSMElementType_RailwaySubway,
OSMElementType_AddressInterpolation,
OSMElementType_NotImplemented,
OSMElementType_Unknown,
OSMElementType_Count,
};

extern const std::string EnumValues_OSMElementType[23];
extern const OSMElementType Indirection_OSMElementType[23];
const int EnumLength_OSMElementType = 23;


#pragma endregion  FORWARD_DECLARATIONS OSMElement
class Way;
class Node;


class OSMElement
{

private:
  REGISTERCLASS(OSMElement);

public:

    enum ElementType 
	{ 
		HIGHWAY_TRUNK			= 1 << 0,
        HIGHWAY_PRIMARY			= 1 << 1,
        HIGHWAY_SECONDARY		= 1 << 2,
        HIGHWAY_TERTIARY		= 1 << 3,
        HIGHWAY_RESIDENTIAL		= 1 << 4,
        HIGHWAY_SERVICE			= 1 << 5,
        HIGHWAY_UNCLASSIFIED	= 1 << 6,
        SIDEWALK				= 1 << 7, 
        CONTOUR					= 1 << 10,
        BUILDING_UNMARKED		= 1 << 15,
        BUILDING_SCHOOL			= 1 << 16,
        BUILDING_ADDRINTERP		= 1 << 17,
        BOUNDARY				= 1 << 21,
        LEISURE_PARK			= 1 << 22,
        NATURAL_WOOD			= 1 << 23,
        NATURAL_WATER			= 1 << 24,
        LANDUSE					= 1 << 25,
        RAILWAY_SUBWAY			= 1 << 26,
        aDEBUG					= 1 << 27,
        ADDR_INTERPOLATION		= 1 << 28,
        NOT_IMPLEMENTED			= 1 << 29,
        ANY_TYPE				= 1 << 30,
        GRID_LINE				= 1 << 31
	} ;



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

    std::string getValue(std::string key);

    virtual std::string toString() = 0;

    std::vector<OSMElement*> parents;

	static ElementType GetTypeFromStrings(const std::string& key, const std::string& value);
	static OSMElementType GetOSMTypeFromStrings(const std::string& key, const std::string& value);

};
namespace std
{
template<>
struct hash<OSMElement::ElementType> 
{
    size_t operator()(const OSMElement::ElementType &et) const 
	{
        return std::hash<int>()(et);
    }
};
}

#endif // OSMELEMENT

