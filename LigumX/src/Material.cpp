#include "stdafx.h"
#include "Material.h"
#include "fstream"
#include "property.h" // todo : make a mandatory includes code region

#pragma region  CLASS_SOURCE Material
#include "Material.h"
#include <cstddef>
const ClassPropertyData Material::g_MaterialProperties[] = 
{
{ "Albedo", offsetof(Material, m_Albedo), 0, LXType_glmvec3, false,  }, 
{ "AmbientColor", offsetof(Material, m_AmbientColor), 0, LXType_glmvec3, false,  }, 
{ "DiffuseColor", offsetof(Material, m_DiffuseColor), 0, LXType_glmvec3, false,  }, 
{ "SpecularColor", offsetof(Material, m_SpecularColor), 0, LXType_glmvec3, false,  }, 
{ "Shininess", offsetof(Material, m_Shininess), 0, LXType_float, false,  }, 
{ "DiffuseTexture", offsetof(Material, m_DiffuseTexture), 0, LXType_Texture, true,  }, 
{ "SpecularTexture", offsetof(Material, m_SpecularTexture), 0, LXType_Texture, true,  }, 
{ "ProgramPipeline", offsetof(Material, m_ProgramPipeline), 0, LXType_ProgramPipeline, true,  }, 
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
