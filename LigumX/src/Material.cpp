#include "stdafx.h"
#include "Material.h"
#include "fstream"
#include "property.h" // todo : make a mandatory includes code region

#pragma region  CLASS_SOURCE Material

#include "Material.h"
#include "serializer.h"
#include "Texture.h"
const ClassPropertyData Material::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(Material, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "AmbientColor", PIDX_AmbientColor, offsetof(Material, m_AmbientColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, 0.f, 1.f, 0,}, 
{ "DiffuseColor", PIDX_DiffuseColor, offsetof(Material, m_DiffuseColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, 0.f, 1.f, 0,}, 
{ "SpecularColor", PIDX_SpecularColor, offsetof(Material, m_SpecularColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, 0.f, 1.f, 0,}, 
{ "IsPBR", PIDX_IsPBR, offsetof(Material, m_IsPBR), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Shininess", PIDX_Shininess, offsetof(Material, m_Shininess), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Metallic", PIDX_Metallic, offsetof(Material, m_Metallic), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0.f, 1.f, 0,}, 
{ "Roughness", PIDX_Roughness, offsetof(Material, m_Roughness), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0.f, 1.f, 0,}, 
{ "AO", PIDX_AO, offsetof(Material, m_AO), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "EmissiveFactor", PIDX_EmissiveFactor, offsetof(Material, m_EmissiveFactor), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "DiffuseTextureEnabled", PIDX_DiffuseTextureEnabled, offsetof(Material, m_DiffuseTextureEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SpecularTextureEnabled", PIDX_SpecularTextureEnabled, offsetof(Material, m_SpecularTextureEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Unlit", PIDX_Unlit, offsetof(Material, m_Unlit), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "RefractionIndex", PIDX_RefractionIndex, offsetof(Material, m_RefractionIndex), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0.f, 1.f, 0,}, 
{ "IsGlass", PIDX_IsGlass, offsetof(Material, m_IsGlass), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ReflectEnvironment", PIDX_ReflectEnvironment, offsetof(Material, m_ReflectEnvironment), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DiffuseTexture", PIDX_DiffuseTexture, offsetof(Material, m_DiffuseTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Material, DiffuseTexture, Texture*),}, 
{ "SpecularTexture", PIDX_SpecularTexture, offsetof(Material, m_SpecularTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Material, SpecularTexture, Texture*),}, 
{ "HeightfieldTexture", PIDX_HeightfieldTexture, offsetof(Material, m_HeightfieldTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShaderFamily", PIDX_ShaderFamily, offsetof(Material, m_ShaderFamily), 0, LXType_Object, sizeof(ShaderFamily), LXType_ShaderFamily, false, LXType_None, false, PropertyFlags_Enum, 0, 0, 0,}, 
};
void Material::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Enabled], m_Enabled);
	serializer.SerializeVec3(g_Properties[PIDX_AmbientColor], m_AmbientColor);
	serializer.SerializeVec3(g_Properties[PIDX_DiffuseColor], m_DiffuseColor);
	serializer.SerializeVec3(g_Properties[PIDX_SpecularColor], m_SpecularColor);
	serializer.SerializeBool(g_Properties[PIDX_IsPBR], m_IsPBR);
	serializer.SerializeFloat(g_Properties[PIDX_Shininess], m_Shininess);
	serializer.SerializeFloat(g_Properties[PIDX_Metallic], m_Metallic);
	serializer.SerializeFloat(g_Properties[PIDX_Roughness], m_Roughness);
	serializer.SerializeFloat(g_Properties[PIDX_AO], m_AO);
	serializer.SerializeFloat(g_Properties[PIDX_EmissiveFactor], m_EmissiveFactor);
	serializer.SerializeBool(g_Properties[PIDX_DiffuseTextureEnabled], m_DiffuseTextureEnabled);
	serializer.SerializeBool(g_Properties[PIDX_SpecularTextureEnabled], m_SpecularTextureEnabled);
	serializer.SerializeBool(g_Properties[PIDX_Unlit], m_Unlit);
	serializer.SerializeFloat(g_Properties[PIDX_RefractionIndex], m_RefractionIndex);
	serializer.SerializeBool(g_Properties[PIDX_IsGlass], m_IsGlass);
	serializer.SerializeBool(g_Properties[PIDX_ReflectEnvironment], m_ReflectEnvironment);
	serializer.SerializeObjectPtr(g_Properties[PIDX_DiffuseTexture], m_DiffuseTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_SpecularTexture], m_SpecularTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_HeightfieldTexture], m_HeightfieldTexture);
}
bool Material::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Material::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("Enabled", m_Enabled  );
	ImguiHelpers::ShowVec3("AmbientColor", m_AmbientColor , 0.f, 1.f );
	ImguiHelpers::ShowVec3("DiffuseColor", m_DiffuseColor , 0.f, 1.f );
	ImguiHelpers::ShowVec3("SpecularColor", m_SpecularColor , 0.f, 1.f );
	ImguiHelpers::ShowBool("IsPBR", m_IsPBR  );
	ImguiHelpers::ShowFloat("Shininess", m_Shininess , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("Metallic", m_Metallic , 0.f, 1.f );
	ImguiHelpers::ShowFloat("Roughness", m_Roughness , 0.f, 1.f );
	ImguiHelpers::ShowFloat("AO", m_AO , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("EmissiveFactor", m_EmissiveFactor , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowBool("DiffuseTextureEnabled", m_DiffuseTextureEnabled  );
	ImguiHelpers::ShowBool("SpecularTextureEnabled", m_SpecularTextureEnabled  );
	ImguiHelpers::ShowBool("Unlit", m_Unlit  );
	ImguiHelpers::ShowFloat("RefractionIndex", m_RefractionIndex , 0.f, 1.f );
	ImguiHelpers::ShowBool("IsGlass", m_IsGlass  );
	ImguiHelpers::ShowBool("ReflectEnvironment", m_ReflectEnvironment  );
	ImguiHelpers::ShowObjectPtr("DiffuseTexture", m_DiffuseTexture  );
	ImguiHelpers::ShowObjectPtr("SpecularTexture", m_SpecularTexture  );
	ImguiHelpers::ShowObjectPtr("HeightfieldTexture", m_HeightfieldTexture  );
	return true;
}
const char* Material::GetTypeName()
{
	return ClassName;
}
const std::string EnumValues_ShaderFamily[] = 
{
"Basic",
"SolidColor",
"Envmap",
"Terrain",
"Roads",
};

const ShaderFamily Indirection_ShaderFamily[] =
{
	ShaderFamily_Basic,
	ShaderFamily_SolidColor,
	ShaderFamily_Envmap,
	ShaderFamily_Terrain,
	ShaderFamily_Roads,
};

#pragma endregion  CLASS_SOURCE Material

MaterialMap Material::MaterialList;


Material* Material::Get(std::string materialName)
{
	return MaterialList.at(materialName);
}

void Material::InitializeToDefaults()
{
	// todo : handle this once we have default constructors from gen files
	m_Enabled = true;

	m_EmissiveFactor = 0.0f;

	SetObjectID(g_ObjectManager->GetNewObjectID());

	SetDiffuseColor(glm::vec3(1, 0, 0));
}

Material::Material()
{
	InitializeToDefaults();
}

Material::Material(ProgramPipeline* programPipeline)
{
	InitializeToDefaults();

	m_ProgramPipeline = programPipeline;
}

Material::Material(ProgramPipeline* programPipeline, glm::vec3 albedo)
{
	InitializeToDefaults();

	m_ProgramPipeline = programPipeline;
	SetDiffuseColor(albedo);
}

Material::Material(ShaderFamily family)
{
	InitializeToDefaults();

	m_ShaderFamily = family;
}


void Material::SetDiffuseTexture_Callback(Texture* value)
{
	m_DiffuseTexture = value;

	m_DiffuseTextureEnabled = (value != nullptr);
}


void Material::SetSpecularTexture_Callback(Texture* value)
{
	m_SpecularTexture = value;

	m_SpecularTextureEnabled = (value != nullptr);
}

