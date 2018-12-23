#pragma once

#pragma region  FORWARD_DECLARATIONS OSMDataProcessorSettings
#include "LXSystem.h"

class OSMDataProcessorSettings;


#pragma endregion  FORWARD_DECLARATIONS OSMDataProcessorSettings

#pragma region  HEADER OSMDataProcessorSettings
class OSMDataProcessorSettings : public LXObject
{
public:
static const int ClassID = 1877541511;
static const LXType Type = LXType_OSMDataProcessorSettings;
static constexpr const char* ClassName = "OSMDataProcessorSettings";

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
bool GetProcessAddressInterpolation() { return m_ProcessAddressInterpolation; }; 
void SetProcessAddressInterpolation(bool value) { m_ProcessAddressInterpolation = value; }; 
bool GetCheckPointInRoad() { return m_CheckPointInRoad; }; 
void SetCheckPointInRoad(bool value) { m_CheckPointInRoad = value; }; 
bool GetCheckPointInBuilding() { return m_CheckPointInBuilding; }; 
void SetCheckPointInBuilding(bool value) { m_CheckPointInBuilding = value; }; 
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
bool m_ProcessAddressInterpolation = true;
bool m_CheckPointInRoad = false;
bool m_CheckPointInBuilding = false;
public:
static const int g_PropertyCount = 13;
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
PIDX_ProcessAddressInterpolation,
PIDX_CheckPointInRoad,
PIDX_CheckPointInBuilding,
};
bool Serialize(bool writing);
void ShowPropertyGrid();

#pragma endregion  HEADER OSMDataProcessorSettings

};
