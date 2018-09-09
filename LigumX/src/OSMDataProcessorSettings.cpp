#include "OSMDataProcessorSettings.h"


#pragma region  CLASS_SOURCE OSMDataProcessorSettings

#include "OSMDataProcessorSettings.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMDataProcessorSettings::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(OSMDataProcessorSettings, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0,}, 
{ "Name", PIDX_Name, offsetof(OSMDataProcessorSettings, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0,}, 
{ "MinFacadeLength", PIDX_MinFacadeLength, offsetof(OSMDataProcessorSettings, m_MinFacadeLength), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MaxFacadeLength", PIDX_MaxFacadeLength, offsetof(OSMDataProcessorSettings, m_MaxFacadeLength), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MinPlotLengthRatio", PIDX_MinPlotLengthRatio, offsetof(OSMDataProcessorSettings, m_MinPlotLengthRatio), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MaxPlotLengthRatio", PIDX_MaxPlotLengthRatio, offsetof(OSMDataProcessorSettings, m_MaxPlotLengthRatio), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MinBuildingDepth", PIDX_MinBuildingDepth, offsetof(OSMDataProcessorSettings, m_MinBuildingDepth), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MaxBuildingDepth", PIDX_MaxBuildingDepth, offsetof(OSMDataProcessorSettings, m_MaxBuildingDepth), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MinHeight", PIDX_MinHeight, offsetof(OSMDataProcessorSettings, m_MinHeight), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MaxHeight", PIDX_MaxHeight, offsetof(OSMDataProcessorSettings, m_MaxHeight), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MinNeighborDistance", PIDX_MinNeighborDistance, offsetof(OSMDataProcessorSettings, m_MinNeighborDistance), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "MaxNeighborDistance", PIDX_MaxNeighborDistance, offsetof(OSMDataProcessorSettings, m_MaxNeighborDistance), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "ProcessAddressInterpolation", PIDX_ProcessAddressInterpolation, offsetof(OSMDataProcessorSettings, m_ProcessAddressInterpolation), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
};
bool OSMDataProcessorSettings::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMDataProcessorSettings
