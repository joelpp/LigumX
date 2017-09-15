#include "stdafx.h"
#include "Material.h"
#include "fstream"
#include "property.h" // todo : make a mandatory includes code region

#pragma region  CLASS_SOURCE Material
const ClassPropertyData Material::g_MaterialProperties[] = 
{
{ "Albedo", offsetof(Material, m_Albedo), 1 },
{ "AmbientColor", offsetof(Material, m_AmbientColor), 1 },
{ "DiffuseColor", offsetof(Material, m_DiffuseColor), 1 },
{ "SpecularColor", offsetof(Material, m_SpecularColor), 1 },
{ "Shininess", offsetof(Material, m_Shininess), 0 },
{ "DiffuseTexture", offsetof(Material, m_DiffuseTexture), 0 },
{ "SpecularTexture", offsetof(Material, m_SpecularTexture), 0 },
{ "ProgramPipeline", offsetof(Material, m_ProgramPipeline), 0 },
};

#pragma endregion  CLASS_SOURCE Material

MaterialMap Material::MaterialList;


Material* Material::Get(std::string materialName)
{
	return MaterialList.at(materialName);
}



Material::Material()
{

}


Material::Material(ProgramPipeline* programPipeline, glm::vec3 albedo)
{
	m_ProgramPipeline = programPipeline;
	SetAlbedo(albedo);
}
