#include "stdafx.h"
#include "Material.h"
#include "fstream"
#include "property.h" // todo : make a mandatory includes code region

#pragma region  CLASS_SOURCE Material
#include "Material.h"
#include <cstddef>
const ClassPropertyData Material::g_Properties[] = 
{
{ "Enabled", offsetof(Material, m_Enabled), 0, LXType_bool, false, LXType_None,  }, 
{ "Albedo", offsetof(Material, m_Albedo), 0, LXType_glmvec3, false, LXType_None,  }, 
{ "AmbientColor", offsetof(Material, m_AmbientColor), 0, LXType_glmvec3, false, LXType_None,  }, 
{ "DiffuseColor", offsetof(Material, m_DiffuseColor), 0, LXType_glmvec3, false, LXType_None,  }, 
{ "SpecularColor", offsetof(Material, m_SpecularColor), 0, LXType_glmvec3, false, LXType_None,  }, 
{ "Shininess", offsetof(Material, m_Shininess), 0, LXType_float, false, LXType_None,  }, 
{ "DiffuseTexture", offsetof(Material, m_DiffuseTexture), 0, LXType_Texture, true, LXType_None,  }, 
{ "SpecularTexture", offsetof(Material, m_SpecularTexture), 0, LXType_Texture, true, LXType_None,  }, 
{ "ProgramPipeline", offsetof(Material, m_ProgramPipeline), 0, LXType_ProgramPipeline, true, LXType_None,  }, 
};

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
}


Material::Material(ProgramPipeline* programPipeline, glm::vec3 albedo)
{
	m_Enabled = true;
	m_ProgramPipeline = programPipeline;
	SetAlbedo(albedo);
}
