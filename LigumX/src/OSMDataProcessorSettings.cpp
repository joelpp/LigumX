#include "OSMDataProcessorSettings.h"


#pragma region  CLASS_SOURCE OSMDataProcessorSettings

#include "OSMDataProcessorSettings.h"
#include "serializer.h"
const ClassPropertyData OSMDataProcessorSettings::g_Properties[] = 
{
{ "MinFacadeLength", PIDX_MinFacadeLength, offsetof(OSMDataProcessorSettings, m_MinFacadeLength), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MaxFacadeLength", PIDX_MaxFacadeLength, offsetof(OSMDataProcessorSettings, m_MaxFacadeLength), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MinPlotLengthRatio", PIDX_MinPlotLengthRatio, offsetof(OSMDataProcessorSettings, m_MinPlotLengthRatio), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MaxPlotLengthRatio", PIDX_MaxPlotLengthRatio, offsetof(OSMDataProcessorSettings, m_MaxPlotLengthRatio), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MinBuildingDepth", PIDX_MinBuildingDepth, offsetof(OSMDataProcessorSettings, m_MinBuildingDepth), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MaxBuildingDepth", PIDX_MaxBuildingDepth, offsetof(OSMDataProcessorSettings, m_MaxBuildingDepth), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MinHeight", PIDX_MinHeight, offsetof(OSMDataProcessorSettings, m_MinHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MaxHeight", PIDX_MaxHeight, offsetof(OSMDataProcessorSettings, m_MaxHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MinNeighborDistance", PIDX_MinNeighborDistance, offsetof(OSMDataProcessorSettings, m_MinNeighborDistance), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MaxNeighborDistance", PIDX_MaxNeighborDistance, offsetof(OSMDataProcessorSettings, m_MaxNeighborDistance), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "ProcessAddressInterpolation", PIDX_ProcessAddressInterpolation, offsetof(OSMDataProcessorSettings, m_ProcessAddressInterpolation), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "CheckPointInRoad", PIDX_CheckPointInRoad, offsetof(OSMDataProcessorSettings, m_CheckPointInRoad), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "CheckPointInBuilding", PIDX_CheckPointInBuilding, offsetof(OSMDataProcessorSettings, m_CheckPointInBuilding), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void OSMDataProcessorSettings::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeFloat(g_Properties[PIDX_MinFacadeLength], m_MinFacadeLength);
	serializer.SerializeFloat(g_Properties[PIDX_MaxFacadeLength], m_MaxFacadeLength);
	serializer.SerializeFloat(g_Properties[PIDX_MinPlotLengthRatio], m_MinPlotLengthRatio);
	serializer.SerializeFloat(g_Properties[PIDX_MaxPlotLengthRatio], m_MaxPlotLengthRatio);
	serializer.SerializeFloat(g_Properties[PIDX_MinBuildingDepth], m_MinBuildingDepth);
	serializer.SerializeFloat(g_Properties[PIDX_MaxBuildingDepth], m_MaxBuildingDepth);
	serializer.SerializeFloat(g_Properties[PIDX_MinHeight], m_MinHeight);
	serializer.SerializeFloat(g_Properties[PIDX_MaxHeight], m_MaxHeight);
	serializer.SerializeFloat(g_Properties[PIDX_MinNeighborDistance], m_MinNeighborDistance);
	serializer.SerializeFloat(g_Properties[PIDX_MaxNeighborDistance], m_MaxNeighborDistance);
	serializer.SerializeBool(g_Properties[PIDX_ProcessAddressInterpolation], m_ProcessAddressInterpolation);
	serializer.SerializeBool(g_Properties[PIDX_CheckPointInRoad], m_CheckPointInRoad);
	serializer.SerializeBool(g_Properties[PIDX_CheckPointInBuilding], m_CheckPointInBuilding);
}
bool OSMDataProcessorSettings::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMDataProcessorSettings::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowFloat("MinFacadeLength", m_MinFacadeLength , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MaxFacadeLength", m_MaxFacadeLength , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MinPlotLengthRatio", m_MinPlotLengthRatio , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MaxPlotLengthRatio", m_MaxPlotLengthRatio , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MinBuildingDepth", m_MinBuildingDepth , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MaxBuildingDepth", m_MaxBuildingDepth , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MinHeight", m_MinHeight , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MaxHeight", m_MaxHeight , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MinNeighborDistance", m_MinNeighborDistance , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MaxNeighborDistance", m_MaxNeighborDistance , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowBool("ProcessAddressInterpolation", m_ProcessAddressInterpolation  );
	ImguiHelpers::ShowBool("CheckPointInRoad", m_CheckPointInRoad  );
	ImguiHelpers::ShowBool("CheckPointInBuilding", m_CheckPointInBuilding  );
	return true;
}
const char* OSMDataProcessorSettings::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OSMDataProcessorSettings
