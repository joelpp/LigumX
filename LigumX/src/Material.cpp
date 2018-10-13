#include "stdafx.h"
#include "Material.h"
#include "fstream"
#include "property.h" // todo : make a mandatory includes code region

#pragma region  CLASS_SOURCE Material

#include "Material.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Material::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(Material, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "AmbientColor", PIDX_AmbientColor, offsetof(Material, m_AmbientColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, 0, 0, 1, 0,}, 
{ "DiffuseColor", PIDX_DiffuseColor, offsetof(Material, m_DiffuseColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, 0, 0, 1, 0,}, 
{ "SpecularColor", PIDX_SpecularColor, offsetof(Material, m_SpecularColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, 0, 0, 1, 0,}, 
{ "IsPBR", PIDX_IsPBR, offsetof(Material, m_IsPBR), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "Shininess", PIDX_Shininess, offsetof(Material, m_Shininess), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "Metallic", PIDX_Metallic, offsetof(Material, m_Metallic), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, 0, 0, 1, 0,}, 
{ "Roughness", PIDX_Roughness, offsetof(Material, m_Roughness), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, 0, 0, 1, 0,}, 
{ "AO", PIDX_AO, offsetof(Material, m_AO), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "EmissiveFactor", PIDX_EmissiveFactor, offsetof(Material, m_EmissiveFactor), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "DiffuseTextureEnabled", PIDX_DiffuseTextureEnabled, offsetof(Material, m_DiffuseTextureEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "SpecularTextureEnabled", PIDX_SpecularTextureEnabled, offsetof(Material, m_SpecularTextureEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "Unlit", PIDX_Unlit, offsetof(Material, m_Unlit), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "RefractionIndex", PIDX_RefractionIndex, offsetof(Material, m_RefractionIndex), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, 0, 0, 1, 0,}, 
{ "IsGlass", PIDX_IsGlass, offsetof(Material, m_IsGlass), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "ReflectEnvironment", PIDX_ReflectEnvironment, offsetof(Material, m_ReflectEnvironment), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "DiffuseTexture", PIDX_DiffuseTexture, offsetof(Material, m_DiffuseTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Material, DiffuseTexture, Texture*),}, 
{ "SpecularTexture", PIDX_SpecularTexture, offsetof(Material, m_SpecularTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Material, SpecularTexture, Texture*),}, 
{ "HeightfieldTexture", PIDX_HeightfieldTexture, offsetof(Material, m_HeightfieldTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, 0, 0, 0, 0,}, 
{ "ProgramPipeline", PIDX_ProgramPipeline, offsetof(Material, m_ProgramPipeline), 0, LXType_ObjectPtr, sizeof(ProgramPipeline*), LXType_ProgramPipeline, true, LXType_None, PropertyFlags_Transient, 0, 0, 0,}, 
{ "ShaderFamily", PIDX_ShaderFamily, offsetof(Material, m_ShaderFamily), 0, LXType_Object, sizeof(ShaderFamily), LXType_ShaderFamily, false, LXType_None, PropertyFlags_Enum, 0, 0, 0,}, 
};
bool Material::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
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

