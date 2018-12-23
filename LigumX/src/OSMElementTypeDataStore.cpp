#include "OSMElementTypeDataStore.h"

#include "Way.h"


#pragma region  CLASS_SOURCE OSMElementTypeDataStore
OSMElementTypeDataStore* g_OSMElementTypeDataStore;

#include "OSMElementTypeDataStore.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMElementTypeDataStore::g_Properties[] = 
{
{ "Data", PIDX_Data, offsetof(OSMElementTypeDataStore, m_Data), 0, LXType_stdvector, sizeof(std::vector<OSMElementTypeData>), LXType_stdvector, false, LXType_OSMElementTypeData, false, 0, 0, 0, 0,}, 
};
bool OSMElementTypeDataStore::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMElementTypeDataStore::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	return true;
}

#pragma endregion  CLASS_SOURCE OSMElementTypeDataStore

#define AddAttribute(key, value) data->GetAttributes().push_back(OSMAttribute(#key, #value));
#define AddSingleAttribute(key) data->GetAttributes().push_back(OSMAttribute(#key, ""));

OSMElementTypeDataStore::OSMElementTypeDataStore()
{
	m_Data.resize(OSMElementType_Count);

	OSMElementTypeData* data = &m_Data[OSMElementType_AmenityLibrary];
	data->SetType(OSMElementType_AmenityLibrary);
	AddAttribute(building, yes);
	AddAttribute(amenity, library);

	data = &m_Data[OSMElementType_AmenityParking];
	data->SetType(OSMElementType_AmenityParking);
	data->SetFillIn(true);
	AddAttribute(amenity, parking);

	data = &m_Data[OSMElementType_AmenityTheatre];
	data->SetType(OSMElementType_AmenityTheatre);
	data->SetFillIn(true);
	AddAttribute(building, yes);
	AddAttribute(amenity, theatre);

	data = &m_Data[OSMElementType_AmenityPlaceOfWorship];
	data->SetType(OSMElementType_AmenityPlaceOfWorship);
	AddAttribute(building, yes);
	AddAttribute(amenity, place_of_worship);

	data = &m_Data[OSMElementType_AmenityPolice];
	data->SetType(OSMElementType_AmenityPolice);
	AddAttribute(building, yes);
	AddAttribute(amenity, police);

	data = &m_Data[OSMElementType_AmenityTownhall];
	data->SetType(OSMElementType_AmenityTownhall);
	AddAttribute(building, yes);
	AddAttribute(amenity, townhall);

	data = &m_Data[OSMElementType_AmenityUniversity];
	data->SetType(OSMElementType_AmenityUniversity);
	AddAttribute(building, yes);
	AddAttribute(amenity, university);

	data = &m_Data[OSMElementType_Boundary];
	data->SetType(OSMElementType_Boundary);
	data->SetDebugColor(glm::vec3(0.0, 0.0, 0.5));
	AddSingleAttribute(boundary);

	data = &m_Data[OSMElementType_Building_Commercial];
	data->SetType(OSMElementType_Building_Commercial);
	data->SetDebugColor(glm::vec3(1.0, 1.0, 1.0));
	AddAttribute(building, commercial);

	data = &m_Data[OSMElementType_Building_Dormitory];
	data->SetType(OSMElementType_Building_Dormitory);
	data->SetDebugColor(glm::vec3(1.0, 1.0, 1.0));
	AddAttribute(building, dormitory);

	data = &m_Data[OSMElementType_Building_Hospital];
	data->SetType(OSMElementType_Building_Hospital);
	data->SetDebugColor(glm::vec3(1.0, 1.0, 1.0));
	AddAttribute(building, hospital);

	data = &m_Data[OSMElementType_Building_Unmarked]; 
	data->SetType(OSMElementType_Building_Unmarked);
	data->SetDebugColor(glm::vec3(0.0, 0.0, 1.0));
	AddAttribute(building, yes);

	data = &m_Data[OSMElementType_Building_School]; 
	data->SetType(OSMElementType_Building_School);
	data->SetDebugColor(glm::vec3(0.0, 0.0, 1.0));
	AddAttribute(building, school);

	data = &m_Data[OSMElementType_Building_ShopSupermarket];
	data->SetType(OSMElementType_Building_ShopSupermarket);
	data->SetDebugColor(glm::vec3(0.0, 0.0, 1.0));
	AddAttribute(building, yes);
	AddAttribute(shop, supermarket);

	data = &m_Data[OSMElementType_Contour];
	data->SetType(OSMElementType_Contour);
	data->SetDebugColor(glm::vec3(1.0, 1.0, 1.0));
	AddSingleAttribute(contour);

	data = &m_Data[OSMElementType_HighwayTrunk];
	data->SetType(OSMElementType_HighwayTrunk);
	data->SetDebugColor(glm::vec3(1.0, 1.0, 1.0));
	AddAttribute(highway, trunk);

	data = &m_Data[OSMElementType_HighwayPrimary];
	data->SetType(OSMElementType_HighwayPrimary);
	data->SetDebugColor(glm::vec3(0.9, 0.9, 0.9));
	AddAttribute(highway, primary);

	data = &m_Data[OSMElementType_HighwaySecondary];
	data->SetType(OSMElementType_HighwaySecondary);
	data->SetDebugColor(glm::vec3(0.8, 0.8, 0.8));
	AddAttribute(highway, secondary);

	data = &m_Data[OSMElementType_HighwayTertiary];
	data->SetType(OSMElementType_HighwayTertiary);
	data->SetDebugColor(glm::vec3(0.7, 0.7, 0.7));
	AddAttribute(highway, tertiary);

	data = &m_Data[OSMElementType_HighwayResidential];
	data->SetType(OSMElementType_HighwayResidential);
	data->SetDebugColor(glm::vec3(0.6, 0.6, 0.6));
	AddAttribute(highway, residential);

	data = &m_Data[OSMElementType_HighwayService];
	data->SetType(OSMElementType_HighwayService);
	data->SetDebugColor(glm::vec3(0.5, 0.5, 0.5));
	AddAttribute(highway, service);

	data = &m_Data[OSMElementType_HighwayUnclassified];
	data->SetType(OSMElementType_HighwayUnclassified);
	data->SetDebugColor(glm::vec3(0.4, 0.4, 0.4));
	AddAttribute(highway, unclassified);

	data = &m_Data[OSMElementType_LanduseRetail];
	data->SetType(OSMElementType_LanduseRetail);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0.86, 0.86, 0.86));
	AddAttribute(landuse, retail);

	data = &m_Data[OSMElementType_LanduseIndustrial];
	data->SetType(OSMElementType_LanduseIndustrial);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(91, 47, 82) / 255.f);
	AddAttribute(landuse, industrial);

	data = &m_Data[OSMElementType_LeisureSportsCentre];
	data->SetType(OSMElementType_LeisureSportsCentre);
	data->SetDebugColor(glm::vec3(0, 45, 114) / 255.f);
	AddAttribute(leisure, sports_centre);
	AddAttribute(building, yes);

	data = &m_Data[OSMElementType_LeisurePark];
	data->SetType(OSMElementType_LeisurePark);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0.1, 1.0, 0.1));
	AddAttribute(leisure, park);

	data = &m_Data[OSMElementType_LeisurePlayground];
	data->SetType(OSMElementType_LeisurePlayground);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0.3, 1.0, 0.5));
	AddAttribute(leisure, playground);

	data = &m_Data[OSMElementType_LeisureSwimmingPool];
	data->SetType(OSMElementType_LeisureSwimmingPool);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0, 45, 114) / 255.f);
	AddAttribute(leisure, swimming_pool);

	data = &m_Data[OSMElementType_LeisureBaseball];
	data->SetType(OSMElementType_LeisureBaseball);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0, 45, 114) / 255.f);
	AddAttribute(leisure, pitch);
	AddAttribute(sport, baseball);

	data = &m_Data[OSMElementType_NaturalBareRock];
	data->SetType(OSMElementType_NaturalBareRock);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0.0, 1.0, 0.0));
	AddAttribute(natural, bare_rock);

	data = &m_Data[OSMElementType_NaturalBeach];
	data->SetType(OSMElementType_NaturalBeach);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(238, 214, 175) / 255.f);
	AddAttribute(natural, beach);

	data = &m_Data[OSMElementType_NaturalScrub];
	data->SetType(OSMElementType_NaturalScrub);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(61, 64, 49) / 255.f);
	AddAttribute(natural, wood);

	data = &m_Data[OSMElementType_NaturalWood];
	data->SetType(OSMElementType_NaturalWood);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0.0, 1.0, 0.0));
	AddAttribute(natural, wood);

	data = &m_Data[OSMElementType_NaturalWater]; 
	data->SetType(OSMElementType_NaturalWater);
	data->SetFillIn(true);
	data->SetDebugColor(glm::vec3(0.0, 0.2, 0.8));
	AddAttribute(natural, water);

	data = &m_Data[OSMElementType_RailwaySubway]; 
	data->SetType(OSMElementType_RailwaySubway);
	data->SetDebugColor(glm::vec3(0.0, 0.8, 1.0));
	AddAttribute(railway, subway);

	data = &m_Data[OSMElementType_AddressInterpolation]; 
	data->SetType(OSMElementType_AddressInterpolation);
	data->SetDebugColor(glm::vec3(1.0, 1.0, 1.0));
	AddSingleAttribute(addr:interpolation);

	data = &m_Data[OSMElementType_Sidewalk];
	data->SetType(OSMElementType_Sidewalk);
	data->SetDebugColor(glm::vec3(1.0, 0.0, 1.0));
	AddAttribute(highway, footway);

	data = &m_Data[OSMElementType_NotImplemented]; 
	data->SetType(OSMElementType_NotImplemented);
	data->SetDebugColor(glm::vec3(1.0, 0.1, 0.1));

	data = &m_Data[OSMElementType_Unknown]; 
	data->SetType(OSMElementType_Unknown);
	data->SetDebugColor(glm::vec3(1.0, 0.0, 0.0));
}


OSMElementType OSMElementTypeDataStore::GetWayType(Way* way)
{
	for (OSMElementTypeData& data : m_Data)
	{
		bool allAttributesFound = true;

		if (data.GetAttributes().empty())
		{
			continue;
		}

		for (OSMAttribute& attribute : data.GetAttributes())
		{
			bool found = false;
			for (const std::pair<std::string, std::string>& tags : way->tags)
			{
				if (tags.first == attribute.GetKey())
				{
					if (attribute.GetValue().empty() || tags.second == attribute.GetValue())
					{
						found = true;
						break;
					}
				}
				else
				{
					continue;
				}
			}

			if (!found)
			{
				allAttributesFound = false;
				break;
			}
		}

		if (allAttributesFound)
		{
			return data.GetType();
		}

	}

	return OSMElementType_Unknown;
}
