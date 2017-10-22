#include "stdafx.h"
#include "Material.h"
#include "fstream"
#include "property.h" // todo : make a mandatory includes code region

#pragma region  CLASS_SOURCE Material
#include "Material.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData Material::g_Properties[] = 
{
{ "ObjectID", offsetof(Material, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", offsetof(Material, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Enabled", offsetof(Material, m_Enabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "AmbientColor", offsetof(Material, m_AmbientColor), 0, LXType_glmvec3, false, LXType_None, 0, 0, 1, }, 
{ "DiffuseColor", offsetof(Material, m_DiffuseColor), 0, LXType_glmvec3, false, LXType_None, 0, 0, 1, }, 
{ "SpecularColor", offsetof(Material, m_SpecularColor), 0, LXType_glmvec3, false, LXType_None, 0, 0, 1, }, 
{ "IsPBR", offsetof(Material, m_IsPBR), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Shininess", offsetof(Material, m_Shininess), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "Metallic", offsetof(Material, m_Metallic), 0, LXType_float, false, LXType_None, 0, 0, 1, }, 
{ "Roughness", offsetof(Material, m_Roughness), 0, LXType_float, false, LXType_None, 0, 0, 1, }, 
{ "AO", offsetof(Material, m_AO), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "EmissiveFactor", offsetof(Material, m_EmissiveFactor), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "DiffuseTextureEnabled", offsetof(Material, m_DiffuseTextureEnabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "SpecularTextureEnabled", offsetof(Material, m_SpecularTextureEnabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Unlit", offsetof(Material, m_Unlit), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RefractionIndex", offsetof(Material, m_RefractionIndex), 0, LXType_float, false, LXType_None, 0, 0, 1, }, 
{ "IsGlass", offsetof(Material, m_IsGlass), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ReflectEnvironment", offsetof(Material, m_ReflectEnvironment), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DiffuseTexture", offsetof(Material, m_DiffuseTexture), 0, LXType_Texture, true, LXType_None, PropertyFlags_SetCallback, 0, 0, }, 
{ "SpecularTexture", offsetof(Material, m_SpecularTexture), 0, LXType_Texture, true, LXType_None, PropertyFlags_SetCallback, 0, 0, }, 
{ "ProgramPipeline", offsetof(Material, m_ProgramPipeline), 0, LXType_ProgramPipeline, true, LXType_None, PropertyFlags_Transient, 0, 0, }, 
};
void Material::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE Material

MaterialMap Material::MaterialList;


Material* Material::Get(std::string materialName)
{
	return MaterialList.at(materialName);
}



Material::Material()
{
	// todo : handle this once we have default constructors from gen files
	m_Enabled = true;

	m_EmissiveFactor = 0.0f;

	m_ObjectID = g_ObjectIDManager->GetObjectID();;
}


Material::Material(ProgramPipeline* programPipeline, glm::vec3 albedo)
{
	m_Enabled = true;
	m_ProgramPipeline = programPipeline;
	SetDiffuseColor(albedo);

	m_EmissiveFactor = 0.0f;
}

void Material::SetDiffuseTextureCallback(Texture* value)
{
	m_DiffuseTexture = value;

	m_DiffuseTextureEnabled = (value != nullptr);
}


void Material::SetSpecularTextureCallback(Texture* value)
{
	m_SpecularTexture = value;

	m_SpecularTextureEnabled = (value != nullptr);
}

