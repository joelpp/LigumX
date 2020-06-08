#include "GFXUniformDescription.h"

#pragma region  CLASS_SOURCE GFXUniformDescription

#include "GFXUniformDescription.h"
#include "serializer.h"
const ClassPropertyData GFXUniformDescription::g_Properties[] = 
{
{ "Type", PIDX_Type, offsetof(GFXUniformDescription, m_Type), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "UniformName", PIDX_UniformName, offsetof(GFXUniformDescription, m_UniformName), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Location", PIDX_Location, offsetof(GFXUniformDescription, m_Location), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
};
void GFXUniformDescription::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_Type], m_Type);
	serializer.SerializeString(g_Properties[PIDX_UniformName], m_UniformName);
	serializer.SerializeInt(g_Properties[PIDX_Location], m_Location);
}
bool GFXUniformDescription::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool GFXUniformDescription::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Type], &m_Type , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_UniformName], m_UniformName  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Location], &m_Location , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	return true;
}
void GFXUniformDescription::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	GFXUniformDescription* other = (GFXUniformDescription*) otherObj;
	other->SetType(m_Type);
	other->SetUniformName(m_UniformName);
	other->SetLocation(m_Location);
}
const char* GFXUniformDescription::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE GFXUniformDescription
