#pragma once

#pragma region  FORWARD_DECLARATIONS OSMDataProcessorSettings
#include "LXSystem.h"
class Serializer2;

class OSMDataProcessorSettings;
class Material;


#pragma endregion  FORWARD_DECLARATIONS OSMDataProcessorSettings

#pragma region  HEADER OSMDataProcessorSettings
class OSMDataProcessorSettings : public LXObject
{
public:
static const int ClassID = -301590873;
static const LXType Type = LXType_OSMDataProcessorSettings;
static constexpr const char* ClassName = "OSMDataProcessorSettings";
virtual LXType GetLXType() { return LXType_OSMDataProcessorSettings; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

float GetMinFacadeLength() { return m_MinFacadeLength; }; 
void SetMinFacadeLength(float value) { m_MinFacadeLength = value; }; 
float GetMaxFacadeLength() { return m_MaxFacadeLength; }; 
void SetMaxFacadeLength(float value) { m_MaxFacadeLength = value; }; 
float GetMinPlotLengthRatio() { return m_MinPlotLengthRatio; }; 
void SetMinPlotLengthRatio(float value) { m_MinPlotLengthRatio = value; }; 
float GetMaxPlotLengthRatio() { return m_MaxPlotLengthRatio; }; 
void SetMaxPlotLengthRatio(float value) { m_MaxPlotLengthRatio = value; }; 
float GetMinBuildingDepth() { return m_MinBuildingDepth; }; 
void SetMinBuildingDepth(float value) { m_MinBuildingDepth = value; }; 
float GetMaxBuildingDepth() { return m_MaxBuildingDepth; }; 
void SetMaxBuildingDepth(float value) { m_MaxBuildingDepth = value; }; 
float GetMinHeight() { return m_MinHeight; }; 
void SetMinHeight(float value) { m_MinHeight = value; }; 
float GetMaxHeight() { return m_MaxHeight; }; 
void SetMaxHeight(float value) { m_MaxHeight = value; }; 
float GetMinNeighborDistance() { return m_MinNeighborDistance; }; 
void SetMinNeighborDistance(float value) { m_MinNeighborDistance = value; }; 
float GetMaxNeighborDistance() { return m_MaxNeighborDistance; }; 
void SetMaxNeighborDistance(float value) { m_MaxNeighborDistance = value; }; 
float GetRoadWidth() { return m_RoadWidth; }; 
void SetRoadWidth(float value) { m_RoadWidth = value; }; 
float GetRoadHeight() { return m_RoadHeight; }; 
void SetRoadHeight(float value) { m_RoadHeight = value; }; 
float GetFillInHeight() { return m_FillInHeight; }; 
void SetFillInHeight(float value) { m_FillInHeight = value; }; 
float GetAlleyWidth() { return m_AlleyWidth; }; 
void SetAlleyWidth(float value) { m_AlleyWidth = value; }; 
float GetBuildingHeightBase() { return m_BuildingHeightBase; }; 
void SetBuildingHeightBase(float value) { m_BuildingHeightBase = value; }; 
float GetBuildingHeightVariance() { return m_BuildingHeightVariance; }; 
void SetBuildingHeightVariance(float value) { m_BuildingHeightVariance = value; }; 
bool GetProcessAddressInterpolation() { return m_ProcessAddressInterpolation; }; 
void SetProcessAddressInterpolation(bool value) { m_ProcessAddressInterpolation = value; }; 
bool GetCheckPointInRoad() { return m_CheckPointInRoad; }; 
void SetCheckPointInRoad(bool value) { m_CheckPointInRoad = value; }; 
bool GetCheckPointInBuilding() { return m_CheckPointInBuilding; }; 
void SetCheckPointInBuilding(bool value) { m_CheckPointInBuilding = value; }; 
Material*& GetAlleyMaterial() { return m_AlleyMaterial; }; 
void SetAlleyMaterial(Material* value) { m_AlleyMaterial = value; }; 
Material*& GetParkingMaterial() { return m_ParkingMaterial; }; 
void SetParkingMaterial(Material* value) { m_ParkingMaterial = value; }; 
Material*& GetRoadMaterial() { return m_RoadMaterial; }; 
void SetRoadMaterial(Material* value) { m_RoadMaterial = value; }; 
Material*& GetGenericBuildingMaterial() { return m_GenericBuildingMaterial; }; 
void SetGenericBuildingMaterial(Material* value) { m_GenericBuildingMaterial = value; }; 
Material*& GetRockMaterial() { return m_RockMaterial; }; 
void SetRockMaterial(Material* value) { m_RockMaterial = value; }; 
Material*& GetGrassMaterial() { return m_GrassMaterial; }; 
void SetGrassMaterial(Material* value) { m_GrassMaterial = value; }; 
Material*& GetAdressInterpolationMaterial() { return m_AdressInterpolationMaterial; }; 
void SetAdressInterpolationMaterial(Material* value) { m_AdressInterpolationMaterial = value; }; 
private:
float m_MinFacadeLength = 25.f;
float m_MaxFacadeLength = 50.f;
float m_MinPlotLengthRatio = 1.7f;
float m_MaxPlotLengthRatio = 2.5f;
float m_MinBuildingDepth = 25.f;
float m_MaxBuildingDepth = 50.f;
float m_MinHeight = 20.f;
float m_MaxHeight = 50.f;
float m_MinNeighborDistance = 30.f;
float m_MaxNeighborDistance = 70.f;
float m_RoadWidth = 30.f;
float m_RoadHeight = 0.1f;
float m_FillInHeight = 0.05f;
float m_AlleyWidth = 20.f;
float m_BuildingHeightBase = 30.f;
float m_BuildingHeightVariance = 5.f;
bool m_ProcessAddressInterpolation = true;
bool m_CheckPointInRoad = false;
bool m_CheckPointInBuilding = false;
Material* m_AlleyMaterial = nullptr;
Material* m_ParkingMaterial = nullptr;
Material* m_RoadMaterial = nullptr;
Material* m_GenericBuildingMaterial = nullptr;
Material* m_RockMaterial = nullptr;
Material* m_GrassMaterial = nullptr;
Material* m_AdressInterpolationMaterial = nullptr;
public:
static const int g_PropertyCount = 26;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMDataProcessorSettingsPIDX
{
PIDX_MinFacadeLength,
PIDX_MaxFacadeLength,
PIDX_MinPlotLengthRatio,
PIDX_MaxPlotLengthRatio,
PIDX_MinBuildingDepth,
PIDX_MaxBuildingDepth,
PIDX_MinHeight,
PIDX_MaxHeight,
PIDX_MinNeighborDistance,
PIDX_MaxNeighborDistance,
PIDX_RoadWidth,
PIDX_RoadHeight,
PIDX_FillInHeight,
PIDX_AlleyWidth,
PIDX_BuildingHeightBase,
PIDX_BuildingHeightVariance,
PIDX_ProcessAddressInterpolation,
PIDX_CheckPointInRoad,
PIDX_CheckPointInBuilding,
PIDX_AlleyMaterial,
PIDX_ParkingMaterial,
PIDX_RoadMaterial,
PIDX_GenericBuildingMaterial,
PIDX_RockMaterial,
PIDX_GrassMaterial,
PIDX_AdressInterpolationMaterial,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMDataProcessorSettings

};
