#pragma once

#pragma region  FORWARD_DECLARATIONS OSMDataProcessorSettings
#include "property.h"

class OSMDataProcessorSettings;


#pragma endregion  FORWARD_DECLARATIONS OSMDataProcessorSettings

class OSMDataProcessorSettings
{
#pragma region  HEADER OSMDataProcessorSettings
public:
static const int ClassID = 1877541511;
static const LXType Type = LXType_OSMDataProcessorSettings;
static constexpr const char* ClassName = "OSMDataProcessorSettings";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const float& GetMinFacadeLength() { return m_MinFacadeLength; }; 
void SetMinFacadeLength(float value) { m_MinFacadeLength = value; }; 
const float& GetMaxFacadeLength() { return m_MaxFacadeLength; }; 
void SetMaxFacadeLength(float value) { m_MaxFacadeLength = value; }; 
const float& GetMinPlotLengthRatio() { return m_MinPlotLengthRatio; }; 
void SetMinPlotLengthRatio(float value) { m_MinPlotLengthRatio = value; }; 
const float& GetMaxPlotLengthRatio() { return m_MaxPlotLengthRatio; }; 
void SetMaxPlotLengthRatio(float value) { m_MaxPlotLengthRatio = value; }; 
const float& GetMinBuildingDepth() { return m_MinBuildingDepth; }; 
void SetMinBuildingDepth(float value) { m_MinBuildingDepth = value; }; 
const float& GetMaxBuildingDepth() { return m_MaxBuildingDepth; }; 
void SetMaxBuildingDepth(float value) { m_MaxBuildingDepth = value; }; 
const float& GetMinHeight() { return m_MinHeight; }; 
void SetMinHeight(float value) { m_MinHeight = value; }; 
const float& GetMaxHeight() { return m_MaxHeight; }; 
void SetMaxHeight(float value) { m_MaxHeight = value; }; 
const float& GetMinNeighborDistance() { return m_MinNeighborDistance; }; 
void SetMinNeighborDistance(float value) { m_MinNeighborDistance = value; }; 
const float& GetMaxNeighborDistance() { return m_MaxNeighborDistance; }; 
void SetMaxNeighborDistance(float value) { m_MaxNeighborDistance = value; }; 
const bool& GetProcessAddressInterpolation() { return m_ProcessAddressInterpolation; }; 
void SetProcessAddressInterpolation(bool value) { m_ProcessAddressInterpolation = value; }; 
private:
int m_ObjectID;
std::string m_Name;
float m_MinFacadeLength = 25.f;
float m_MaxFacadeLength = 50.f;
float m_MinPlotLengthRatio = 1.1f;
float m_MaxPlotLengthRatio = 1.3f;
float m_MinBuildingDepth = 25.f;
float m_MaxBuildingDepth = 50.f;
float m_MinHeight = 20.f;
float m_MaxHeight = 50.f;
float m_MinNeighborDistance = 40.f;
float m_MaxNeighborDistance = 70.f;
bool m_ProcessAddressInterpolation = true;
public:
static const int g_PropertyCount = 13;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMDataProcessorSettingsPIDX
{
PIDX_ObjectID,
PIDX_Name,
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
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMDataProcessorSettings

};
