

#include "OSMElement.h"

#pragma region  CLASS_SOURCE OSMElement
const std::string EnumValues_OSMElementType[] = 
{
"AmenityLibrary",
"AmenityParking",
"AmenityTheatre",
"AmenityPlaceOfWorship",
"AmenityPolice",
"AmenityTownhall",
"AmenityUniversity",
"Boundary",
"HighwayTrunk",
"HighwayPrimary",
"HighwaySecondary",
"HighwayTertiary",
"HighwayResidential",
"HighwayService",
"HighwayUnclassified",
"Sidewalk",
"Contour",
"Building_Commercial",
"Building_Dormitory",
"Building_Unmarked",
"Building_Hospital",
"Building_School",
"Building_ShopSupermarket",
"Building_Addressinterpolation",
"LanduseIndustrial",
"LanduseRetail",
"LeisurePark",
"LeisurePlayground",
"LeisureSportsCentre",
"LeisureSwimmingPool",
"LeisureBaseball",
"NaturalBareRock",
"NaturalBeach",
"NaturalScrub",
"NaturalWood",
"NaturalWater",
"RailwaySubway",
"AddressInterpolation",
"NotImplemented",
"Unknown",
"Count",
};

const OSMElementType Indirection_OSMElementType[] =
{
	OSMElementType_AmenityLibrary,
	OSMElementType_AmenityParking,
	OSMElementType_AmenityTheatre,
	OSMElementType_AmenityPlaceOfWorship,
	OSMElementType_AmenityPolice,
	OSMElementType_AmenityTownhall,
	OSMElementType_AmenityUniversity,
	OSMElementType_Boundary,
	OSMElementType_HighwayTrunk,
	OSMElementType_HighwayPrimary,
	OSMElementType_HighwaySecondary,
	OSMElementType_HighwayTertiary,
	OSMElementType_HighwayResidential,
	OSMElementType_HighwayService,
	OSMElementType_HighwayUnclassified,
	OSMElementType_Sidewalk,
	OSMElementType_Contour,
	OSMElementType_Building_Commercial,
	OSMElementType_Building_Dormitory,
	OSMElementType_Building_Unmarked,
	OSMElementType_Building_Hospital,
	OSMElementType_Building_School,
	OSMElementType_Building_ShopSupermarket,
	OSMElementType_Building_Addressinterpolation,
	OSMElementType_LanduseIndustrial,
	OSMElementType_LanduseRetail,
	OSMElementType_LeisurePark,
	OSMElementType_LeisurePlayground,
	OSMElementType_LeisureSportsCentre,
	OSMElementType_LeisureSwimmingPool,
	OSMElementType_LeisureBaseball,
	OSMElementType_NaturalBareRock,
	OSMElementType_NaturalBeach,
	OSMElementType_NaturalScrub,
	OSMElementType_NaturalWood,
	OSMElementType_NaturalWater,
	OSMElementType_RailwaySubway,
	OSMElementType_AddressInterpolation,
	OSMElementType_NotImplemented,
	OSMElementType_Unknown,
	OSMElementType_Count,
};
#pragma endregion  CLASS_SOURCE OSMElement
































































































































































































































using namespace std;

std::string OSMElement::getValue(std::string key){
    return tags[key];
}

bool OSMElement::hasTag(std::string tag){

        std::unordered_map<std::string,std::string>::const_iterator got = tags.find(tag);

     if ( got == tags.end() )
        return false;
     else
        return true;


}

bool OSMElement::hasITag(int tag){
    try{
        std::unordered_map<int,int>::const_iterator got = itags.find(tag);

         if ( got == itags.end() )
            return false;
         else
            return true;
    }
    catch(...){
        PRINT("exception!");
        return false;
    }
}

bool OSMElement::hasTagAndValue(std::string key, std::string value){

    if (hasTag(key))
        return ((tags[key].compare(value) == 0) || tags[key].compare("") == 0);
    else
        return false;
}

bool OSMElement::hasITagAndValue(int key, int value){
    try
	{
        if (hasITag(key))
            return (itags[key] == value);
        else
            return false;
    }
    catch(...)
	{
        return false;
    }
}

#define BEGIN_KEY(keyName) \
if (key.compare(#keyName) == 0) \
{ 

#define END_KEY(keyName) \
else return OSMElement::NOT_IMPLEMENTED; \
}

#define ADD_VALUE(valueName, osmElementType) if	(value.compare(#valueName) == 0) return OSMElement::osmElementType;
#define ELSE_ADD_VALUE(valueName, osmElementType) else if (value.compare(#valueName) == 0) return OSMElement::osmElementType;

#define ADD_SINGLE_KEY(keyName, osmElementType) \
else if (key.compare(#keyName) == 0) \
{ \
	return OSMElement::osmElementType; \
} \

#define ELSE_BEGIN_KEY(keyName) \
else if (key.compare(#keyName) == 0) \
{ 


OSMElement::ElementType OSMElement::GetTypeFromStrings(const std::string& key, const std::string& value)
{
	BEGIN_KEY(highway)
		ADD_VALUE(trunk,				HIGHWAY_TRUNK)
		ELSE_ADD_VALUE(primary,			HIGHWAY_PRIMARY)
		ELSE_ADD_VALUE(secondary,		HIGHWAY_SECONDARY)
		ELSE_ADD_VALUE(tertiary,		HIGHWAY_TERTIARY)
		ELSE_ADD_VALUE(residential,		HIGHWAY_RESIDENTIAL)
		ELSE_ADD_VALUE(service,			HIGHWAY_SERVICE)
		ELSE_ADD_VALUE(unclassified,	HIGHWAY_UNCLASSIFIED)
	END_KEY(highway)

	ELSE_BEGIN_KEY(natural)
		ADD_VALUE(wood,					NATURAL_WOOD)
		ELSE_ADD_VALUE(water,			NATURAL_WATER)
	END_KEY(natural)

	ELSE_BEGIN_KEY(building)
		ADD_VALUE(yes,			BUILDING_UNMARKED)
		ELSE_ADD_VALUE(school,	BUILDING_SCHOOL)
	END_KEY(building)

	ELSE_BEGIN_KEY(leisure)
		ADD_VALUE(park, LEISURE_PARK)
	END_KEY(leisure)

	ADD_SINGLE_KEY(addr:interpolation,	ADDR_INTERPOLATION)
	ADD_SINGLE_KEY(landuse,				LANDUSE)
	ADD_SINGLE_KEY(boundary,			BOUNDARY)
	ADD_SINGLE_KEY(contour,	CONTOUR)

    else return OSMElement::NOT_IMPLEMENTED;
}

#undef BEGIN_KEY
#undef END_KEY
#undef ADD_VALUE
#undef ELSE_ADD_VALUE
#undef ADD_SINGLE_KEY
#undef ELSE_BEGIN_KEY

#define BEGIN_KEY(keyName) \
if (key.compare(#keyName) == 0) \
{ 

#define END_KEY(keyName) \
else return OSMElementType_Unknown; \
}

#define ADD_VALUE(valueName, osmElementType) if	(value.compare(#valueName) == 0) return osmElementType;
#define ELSE_ADD_VALUE(valueName, osmElementType) else if (value.compare(#valueName) == 0) return osmElementType;

#define ADD_SINGLE_KEY(keyName, osmElementType) \
else if (key.compare(#keyName) == 0) \
{ \
	return osmElementType; \
} \

#define ELSE_BEGIN_KEY(keyName) \
else if (key.compare(#keyName) == 0) \
{ 


OSMElementType OSMElement::GetOSMTypeFromStrings(const std::string& key, const std::string& value)
{
	BEGIN_KEY(highway)
		ADD_VALUE(trunk,				OSMElementType_HighwayTrunk)
		ELSE_ADD_VALUE(primary,			OSMElementType_HighwayPrimary)
		ELSE_ADD_VALUE(secondary,		OSMElementType_HighwaySecondary)
		ELSE_ADD_VALUE(tertiary,		OSMElementType_HighwayTertiary)
		ELSE_ADD_VALUE(residential,		OSMElementType_HighwayResidential)
		ELSE_ADD_VALUE(service,			OSMElementType_HighwayService)
		ELSE_ADD_VALUE(unclassified,	OSMElementType_HighwayUnclassified)
	END_KEY(highway)

	ELSE_BEGIN_KEY(natural)
		ADD_VALUE(wood,			OSMElementType_NaturalWood)
		ELSE_ADD_VALUE(water, OSMElementType_NaturalWater)
		ELSE_ADD_VALUE(bare_rock, OSMElementType_NaturalBareRock)
	END_KEY(natural)

	ELSE_BEGIN_KEY(building)
		ADD_VALUE(yes,			OSMElementType_Building_Unmarked)
		ELSE_ADD_VALUE(school,	OSMElementType_Building_School)
	END_KEY(building)

	ELSE_BEGIN_KEY(leisure)
		ADD_VALUE(park, OSMElementType_LeisurePark)
	END_KEY(leisure)

	ELSE_BEGIN_KEY(landuse)
		ADD_VALUE(retail, OSMElementType_LanduseRetail)
		ELSE_ADD_VALUE(industrial, OSMElementType_LanduseIndustrial)
	END_KEY(landuse)

	ADD_SINGLE_KEY(addr:interpolation,	OSMElementType_AddressInterpolation)
	ADD_SINGLE_KEY(boundary,			OSMElementType_Boundary)
	ADD_SINGLE_KEY(contour,				OSMElementType_Contour)

	else return OSMElementType_Unknown;
}
