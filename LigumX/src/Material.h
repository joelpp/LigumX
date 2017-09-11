#pragma once

#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

class ProgramPipeline;
class Material;
typedef std::unordered_map<std::string, Material*> MaterialMap;
class Texture;
class Material
{

public:

	Material();
	Material(ProgramPipeline* programPipeline, glm::vec3 albedo);


	static void Initialize();
	static Material* Get(std::string materialName);
	static void Add(std::string materialName, Material* material);

	ProgramPipeline* m_programPipeline;
	glm::vec3 m_albedo;
	glm::vec3 m_AmbientColor;
	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;
	float m_Shininess;

	Texture* m_DiffuseTexture;
	Texture* m_SpecularTexture;
private:

	static MaterialMap MaterialList;
};