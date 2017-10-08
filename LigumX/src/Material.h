#pragma once

#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

#pragma region  FORWARD_DECLARATIONS Material
#include "property.h"
class Texture;
class ProgramPipeline;

#pragma endregion  FORWARD_DECLARATIONS Material

class Material
{
#pragma region  HEADER Material
static const int ClassID = 3419754368;
public:
const bool& GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
const glm::vec3& GetAmbientColor() { return m_AmbientColor; }; 
void SetAmbientColor(glm::vec3 value) { m_AmbientColor = value; }; 
const glm::vec3& GetDiffuseColor() { return m_DiffuseColor; }; 
void SetDiffuseColor(glm::vec3 value) { m_DiffuseColor = value; }; 
const glm::vec3& GetSpecularColor() { return m_SpecularColor; }; 
void SetSpecularColor(glm::vec3 value) { m_SpecularColor = value; }; 
const float& GetShininess() { return m_Shininess; }; 
void SetShininess(float value) { m_Shininess = value; }; 
const float& GetEmissiveFactor() { return m_EmissiveFactor; }; 
void SetEmissiveFactor(float value) { m_EmissiveFactor = value; }; 
const bool& GetDiffuseTextureEnabled() { return m_DiffuseTextureEnabled; }; 
void SetDiffuseTextureEnabled(bool value) { m_DiffuseTextureEnabled = value; }; 
const bool& GetSpecularTextureEnabled() { return m_SpecularTextureEnabled; }; 
void SetSpecularTextureEnabled(bool value) { m_SpecularTextureEnabled = value; }; 
const bool& GetUnlit() { return m_Unlit; }; 
void SetUnlit(bool value) { m_Unlit = value; }; 
Texture* GetDiffuseTexture() { return m_DiffuseTexture; }; 
void SetDiffuseTexture(Texture* value) { SetDiffuseTextureCallback(value); }; 
void SetDiffuseTextureCallback(Texture* value);
Texture* GetSpecularTexture() { return m_SpecularTexture; }; 
void SetSpecularTexture(Texture* value) { m_SpecularTexture = value; }; 
ProgramPipeline* GetProgramPipeline() { return m_ProgramPipeline; }; 
void SetProgramPipeline(ProgramPipeline* value) { m_ProgramPipeline = value; }; 
private:
bool m_Enabled;
glm::vec3 m_AmbientColor;
glm::vec3 m_DiffuseColor;
glm::vec3 m_SpecularColor;
float m_Shininess;
float m_EmissiveFactor;
bool m_DiffuseTextureEnabled;
bool m_SpecularTextureEnabled;
bool m_Unlit;
Texture* m_DiffuseTexture;
Texture* m_SpecularTexture;
ProgramPipeline* m_ProgramPipeline;
public:
static const int g_MaterialPropertyCount = 12;
static const ClassPropertyData g_Properties[g_MaterialPropertyCount];


#pragma endregion  HEADER Material

public:

	Material();
	Material(ProgramPipeline* programPipeline, glm::vec3 albedo);


	static void Initialize();
	static Material* Get(std::string materialName);
	static void Add(std::string materialName, Material* material);

private:

	static std::unordered_map<std::string, Material*> MaterialList;
};
typedef std::unordered_map<std::string, Material*> MaterialMap;
