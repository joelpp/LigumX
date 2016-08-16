#include "stdafx.h"

#include "Material.h"
#include "fstream"

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
	m_programPipeline = programPipeline;
	m_albedo = albedo;
}