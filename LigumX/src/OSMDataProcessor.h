#pragma once

#include "glm/glm.hpp"
#include "OSMDataProcessorSettings.h"


float Random0To1();


template <typename T>
T GetRandomValue(T min, T max);

class Model;


struct AddrInterpBuildingInfo
{
	AddrInterpBuildingInfo(OSMDataProcessorSettings* settings)
	{
		m_BuildingDimensions.x = GetRandomValue(settings->GetMinFacadeLength(), settings->GetMaxFacadeLength());

		float minPlotLength = settings->GetMinPlotLengthRatio() * m_BuildingDimensions.x;
		float maxPlotLength = settings->GetMaxPlotLengthRatio() * m_BuildingDimensions.x;
		m_PlotDimensions.x = GetRandomValue(minPlotLength, maxPlotLength);

		m_PaddingBeforeFacade = (m_PlotDimensions.x - m_BuildingDimensions.x) / 2.f;
		m_PaddingAfterFacade = m_PaddingBeforeFacade;

		m_BuildingDimensions.y = GetRandomValue(settings->GetMinBuildingDepth(), settings->GetMaxBuildingDepth());

		m_PlotDimensions.y = m_BuildingDimensions.y * 1.5f;

		m_BuildingDimensions.z = GetRandomValue(settings->GetMinHeight(), settings->GetMaxHeight());

	}

	float GetPlotLength() { return m_PlotDimensions.x; }
	float GetPlotDepth() { return m_PlotDimensions.y; }
	float GetBuildingLength() { return m_BuildingDimensions.x; }
	float GetBuildingDepth() { return m_BuildingDimensions.y; }
	float GetBuildingHeight() { return m_BuildingDimensions.z; }

	glm::vec3 m_BuildingDimensions; // (facade, depth, height)
	glm::vec2 m_PlotDimensions; // facade, depth
	float m_PaddingAfterFacade;
	float m_PaddingBeforeFacade;

};
class Mesh;
class Way;
class Sector;
class Entity;
class OSMDataProcessorSettings;

#pragma region  FORWARD_DECLARATIONS OSMDataProcessor
#include "LXSystem.h"
class Serializer2;

class OSMDataProcessor;
class OSMDataProcessorSettings;


#pragma endregion  FORWARD_DECLARATIONS OSMDataProcessor

bool PointInRoad(Sector* sector, const glm::vec3& worldSpacePosition);

#pragma region  HEADER OSMDataProcessor
class OSMDataProcessor : public LXObject
{
public:
static const int ClassID = -277960222;
static const LXType Type = LXType_OSMDataProcessor;
static constexpr const char* ClassName = "OSMDataProcessor";
virtual LXType GetLXType() { return LXType_OSMDataProcessor; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

float GetRoadWidth() { return m_RoadWidth; }; 
void SetRoadWidth(float value) { m_RoadWidth = value; }; 
OSMDataProcessorSettings*& GetSettings() { return m_Settings; }; 
void SetSettings(OSMDataProcessorSettings* value) { m_Settings = value; }; 
private:
float m_RoadWidth = 30.f;
OSMDataProcessorSettings* m_Settings = nullptr;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMDataProcessorPIDX
{
PIDX_RoadWidth,
PIDX_Settings,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMDataProcessor

OSMDataProcessor();

void ProcessSector(Sector* sector);
Model* CreateModelForWay(Way* way, Entity* entity);

void CreateEntity(Way* way);
Model* CreateRoadModel(Way* way, Entity* entity);
Mesh* BuildRoadMesh(Way* way, Entity* entity);
Mesh* ProcessWayNodes(Sector* sector, Way* way);

void PrepareNextBuilding(AddrInterpBuildingInfo& buildingInfo, const glm::vec3& direction, float& spaceLeft, glm::vec3& plotStart);
Mesh* BuildAdressInterpolationBuilding(Sector* sector, Way* way, Mesh*& groundMesh);
void ProcessAddressInterpolation(Sector* sector, Way* way);
Mesh* BuildGenericBuilding(Sector* sector, Way* way);

void ProcessGenericBuilding(Sector* sector, Way* way);
void Reset();

static bool IsRoad(Way* way);

private:
	int m_GenericBuildingsProcessed = 0;
	int m_RoadsProcessed = 0;
	int m_AddressInterpolationsProcessed = 0;


};
