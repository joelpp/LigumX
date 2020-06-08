#include "GFXUniformGroup.h"

#pragma region  CLASS_SOURCE GFXUniformGroup

#include "GFXUniformGroup.h"
#include "serializer.h"
const ClassPropertyData GFXUniformGroup::g_Properties[] = 
{
{ "GroupName", PIDX_GroupName, offsetof(GFXUniformGroup, m_GroupName), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void GFXUniformGroup::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeString(g_Properties[PIDX_GroupName], m_GroupName);
}
bool GFXUniformGroup::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool GFXUniformGroup::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_GroupName], m_GroupName  );
	return true;
}
void GFXUniformGroup::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	GFXUniformGroup* other = (GFXUniformGroup*) otherObj;
	other->SetGroupName(m_GroupName);
}
const char* GFXUniformGroup::GetTypeName()
{
	return ClassName;
}
const std::string EnumValues_UniformGroupType[] = 
{
"ShadowMap",
"LightingOptions",
"DisplayOptions",
};

const UniformGroupType Indirection_UniformGroupType[] =
{
	UniformGroupType_ShadowMap,
	UniformGroupType_LightingOptions,
	UniformGroupType_DisplayOptions,
};

#pragma endregion  CLASS_SOURCE GFXUniformGroup

GFXUniformGroup::GFXUniformGroup()
{
	m_Uniforms.resize(GFXShaderStage_Count);
}

void GFXUniformGroup::AddUniform(GFXShaderStage stage, GFXUniformDescription& desc)
{
	m_Uniforms[stage].emplace(desc.GetUniformName().c_str(), desc);
}

GFXUniformDescription& GFXUniformGroup::GetUniformDescription(GFXShaderStage stage, const char* name)
{
	return m_Uniforms[stage][name];
}

int GFXUniformGroup::GetUniformLocation(GFXShaderStage stage, const char* name)
{
	return m_Uniforms[stage][name].GetLocation();
}
