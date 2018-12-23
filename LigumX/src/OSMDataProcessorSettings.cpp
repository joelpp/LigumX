#include "OSMDataProcessorSettings.h"


#pragma region  CLASS_SOURCE OSMDataProcessorSettings

#include "OSMDataProcessorSettings.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMDataProcessorSettings::g_Properties[] = 
{
{ "MinFacadeLength", PIDX_MinFacadeLength, offsetof(OSMDataProcessorSettings, m_MinFacadeLength), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MaxFacadeLength", PIDX_MaxFacadeLength, offsetof(OSMDataProcessorSettings, m_MaxFacadeLength), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MinPlotLengthRatio", PIDX_MinPlotLengthRatio, offsetof(OSMDataProcessorSettings, m_MinPlotLengthRatio), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MaxPlotLengthRatio", PIDX_MaxPlotLengthRatio, offsetof(OSMDataProcessorSettings, m_MaxPlotLengthRatio), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MinBuildingDepth", PIDX_MinBuildingDepth, offsetof(OSMDataProcessorSettings, m_MinBuildingDepth), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MaxBuildingDepth", PIDX_MaxBuildingDepth, offsetof(OSMDataProcessorSettings, m_MaxBuildingDepth), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MinHeight", PIDX_MinHeight, offsetof(OSMDataProcessorSettings, m_MinHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MaxHeight", PIDX_MaxHeight, offsetof(OSMDataProcessorSettings, m_MaxHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MinNeighborDistance", PIDX_MinNeighborDistance, offsetof(OSMDataProcessorSettings, m_MinNeighborDistance), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MaxNeighborDistance", PIDX_MaxNeighborDistance, offsetof(OSMDataProcessorSettings, m_MaxNeighborDistance), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ProcessAddressInterpolation", PIDX_ProcessAddressInterpolation, offsetof(OSMDataProcessorSettings, m_ProcessAddressInterpolation), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "CheckPointInRoad", PIDX_CheckPointInRoad, offsetof(OSMDataProcessorSettings, m_CheckPointInRoad), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "CheckPointInBuilding", PIDX_CheckPointInBuilding, offsetof(OSMDataProcessorSettings, m_CheckPointInBuilding), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool OSMDataProcessorSettings::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
void OSMDataProcessorSettings::ShowPropertyGrid()
{
	LXIMGUI_SHOW_FLOAT("MinFacadeLength", m_MinFacadeLength);
	LXIMGUI_SHOW_FLOAT("MaxFacadeLength", m_MaxFacadeLength);
	LXIMGUI_SHOW_FLOAT("MinPlotLengthRatio", m_MinPlotLengthRatio);
	LXIMGUI_SHOW_FLOAT("MaxPlotLengthRatio", m_MaxPlotLengthRatio);
	LXIMGUI_SHOW_FLOAT("MinBuildingDepth", m_MinBuildingDepth);
	LXIMGUI_SHOW_FLOAT("MaxBuildingDepth", m_MaxBuildingDepth);
	LXIMGUI_SHOW_FLOAT("MinHeight", m_MinHeight);
	LXIMGUI_SHOW_FLOAT("MaxHeight", m_MaxHeight);
	LXIMGUI_SHOW_FLOAT("MinNeighborDistance", m_MinNeighborDistance);
	LXIMGUI_SHOW_FLOAT("MaxNeighborDistance", m_MaxNeighborDistance);
	LXIMGUI_SHOW_BOOL("ProcessAddressInterpolation", m_ProcessAddressInterpolation);
	LXIMGUI_SHOW_BOOL("CheckPointInRoad", m_CheckPointInRoad);
	LXIMGUI_SHOW_BOOL("CheckPointInBuilding", m_CheckPointInBuilding);
}

#pragma endregion  CLASS_SOURCE OSMDataProcessorSettings
