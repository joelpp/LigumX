#include "OSMDataProcessorSettings.h"


#pragma region  CLASS_SOURCE OSMDataProcessorSettings

#include "OSMDataProcessorSettings.h"
#include "serializer.h"
#include "Material.h"
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
{ "AlleyMaterial", PIDX_AlleyMaterial, offsetof(OSMDataProcessorSettings, m_AlleyMaterial), 0, LXType_ObjectPtr, sizeof(Material*), LXType_Material, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "RoadMaterial", PIDX_RoadMaterial, offsetof(OSMDataProcessorSettings, m_RoadMaterial), 0, LXType_ObjectPtr, sizeof(Material*), LXType_Material, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "GenericBuildingMaterial", PIDX_GenericBuildingMaterial, offsetof(OSMDataProcessorSettings, m_GenericBuildingMaterial), 0, LXType_ObjectPtr, sizeof(Material*), LXType_Material, true, LXType_None, false, 0, 0, 0, 0,}, 
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
	serializer.SerializeObjectPtr(g_Properties[PIDX_AlleyMaterial], m_AlleyMaterial);
	serializer.SerializeObjectPtr(g_Properties[PIDX_RoadMaterial], m_RoadMaterial);
	serializer.SerializeObjectPtr(g_Properties[PIDX_GenericBuildingMaterial], m_GenericBuildingMaterial);
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
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MinFacadeLength], &m_MinFacadeLength , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MaxFacadeLength], &m_MaxFacadeLength , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MinPlotLengthRatio], &m_MinPlotLengthRatio , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MaxPlotLengthRatio], &m_MaxPlotLengthRatio , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MinBuildingDepth], &m_MinBuildingDepth , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MaxBuildingDepth], &m_MaxBuildingDepth , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MinHeight], &m_MinHeight , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MaxHeight], &m_MaxHeight , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MinNeighborDistance], &m_MinNeighborDistance , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MaxNeighborDistance], &m_MaxNeighborDistance , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ProcessAddressInterpolation], &m_ProcessAddressInterpolation  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_CheckPointInRoad], &m_CheckPointInRoad  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_CheckPointInBuilding], &m_CheckPointInBuilding  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_AlleyMaterial], &m_AlleyMaterial  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_RoadMaterial], &m_RoadMaterial  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_GenericBuildingMaterial], &m_GenericBuildingMaterial  );
	return true;
}
void OSMDataProcessorSettings::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	OSMDataProcessorSettings* other = (OSMDataProcessorSettings*) otherObj;
	other->SetMinFacadeLength(m_MinFacadeLength);
	other->SetMaxFacadeLength(m_MaxFacadeLength);
	other->SetMinPlotLengthRatio(m_MinPlotLengthRatio);
	other->SetMaxPlotLengthRatio(m_MaxPlotLengthRatio);
	other->SetMinBuildingDepth(m_MinBuildingDepth);
	other->SetMaxBuildingDepth(m_MaxBuildingDepth);
	other->SetMinHeight(m_MinHeight);
	other->SetMaxHeight(m_MaxHeight);
	other->SetMinNeighborDistance(m_MinNeighborDistance);
	other->SetMaxNeighborDistance(m_MaxNeighborDistance);
	other->SetProcessAddressInterpolation(m_ProcessAddressInterpolation);
	other->SetCheckPointInRoad(m_CheckPointInRoad);
	other->SetCheckPointInBuilding(m_CheckPointInBuilding);
	other->SetAlleyMaterial(m_AlleyMaterial);
	other->SetRoadMaterial(m_RoadMaterial);
	other->SetGenericBuildingMaterial(m_GenericBuildingMaterial);
}
const char* OSMDataProcessorSettings::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OSMDataProcessorSettings
