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
public:
static const int ClassID = 3419754368;
const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const bool& GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
const glm::vec3& GetAmbientColor() { return m_AmbientColor; }; 
void SetAmbientColor(glm::vec3 value) { m_AmbientColor = value; }; 
const glm::vec3& GetDiffuseColor() { return m_DiffuseColor; }; 
void SetDiffuseColor(glm::vec3 value) { m_DiffuseColor = value; }; 
const glm::vec3& GetSpecularColor() { return m_SpecularColor; }; 
void SetSpecularColor(glm::vec3 value) { m_SpecularColor = value; }; 
const bool& GetIsPBR() { return m_IsPBR; }; 
void SetIsPBR(bool value) { m_IsPBR = value; }; 
const float& GetShininess() { return m_Shininess; }; 
void SetShininess(float value) { m_Shininess = value; }; 
const float& GetMetallic() { return m_Metallic; }; 
void SetMetallic(float value) { m_Metallic = value; }; 
const float& GetRoughness() { return m_Roughness; }; 
void SetRoughness(float value) { m_Roughness = value; }; 
const float& GetAO() { return m_AO; }; 
void SetAO(float value) { m_AO = value; }; 
const float& GetEmissiveFactor() { return m_EmissiveFactor; }; 
void SetEmissiveFactor(float value) { m_EmissiveFactor = value; }; 
const bool& GetDiffuseTextureEnabled() { return m_DiffuseTextureEnabled; }; 
void SetDiffuseTextureEnabled(bool value) { m_DiffuseTextureEnabled = value; }; 
const bool& GetSpecularTextureEnabled() { return m_SpecularTextureEnabled; }; 
void SetSpecularTextureEnabled(bool value) { m_SpecularTextureEnabled = value; }; 
const bool& GetUnlit() { return m_Unlit; }; 
void SetUnlit(bool value) { m_Unlit = value; }; 
const float& GetRefractionIndex() { return m_RefractionIndex; }; 
void SetRefractionIndex(float value) { m_RefractionIndex = value; }; 
const bool& GetIsGlass() { return m_IsGlass; }; 
void SetIsGlass(bool value) { m_IsGlass = value; }; 
const bool& GetReflectEnvironment() { return m_ReflectEnvironment; }; 
void SetReflectEnvironment(bool value) { m_ReflectEnvironment = value; }; 
Texture* GetDiffuseTexture() { return m_DiffuseTexture; }; 
void SetDiffuseTexture(Texture* value) { SetDiffuseTextureCallback(value); }; 
void SetDiffuseTextureCallback(Texture* value);
Texture* GetSpecularTexture() { return m_SpecularTexture; }; 
void SetSpecularTexture(Texture* value) { SetSpecularTextureCallback(value); }; 
void SetSpecularTextureCallback(Texture* value);
ProgramPipeline* GetProgramPipeline() { return m_ProgramPipeline; }; 
void SetProgramPipeline(ProgramPipeline* value) { m_ProgramPipeline = value; }; 
private:
int m_ObjectID;
bool m_Enabled;
glm::vec3 m_AmbientColor;
glm::vec3 m_DiffuseColor;
glm::vec3 m_SpecularColor;
bool m_IsPBR;
float m_Shininess;
float m_Metallic;
float m_Roughness;
float m_AO;
float m_EmissiveFactor;
bool m_DiffuseTextureEnabled;
bool m_SpecularTextureEnabled;
bool m_Unlit;
float m_RefractionIndex;
bool m_IsGlass;
bool m_ReflectEnvironment;
Texture* m_DiffuseTexture;
Texture* m_SpecularTexture;
ProgramPipeline* m_ProgramPipeline;
public:
static const int g_PropertyCount = 20;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MaterialPIDX
{
PIDX_ObjectID,
PIDX_Enabled,
PIDX_AmbientColor,
PIDX_DiffuseColor,
PIDX_SpecularColor,
PIDX_IsPBR,
PIDX_Shininess,
PIDX_Metallic,
PIDX_Roughness,
PIDX_AO,
PIDX_EmissiveFactor,
PIDX_DiffuseTextureEnabled,
PIDX_SpecularTextureEnabled,
PIDX_Unlit,
PIDX_RefractionIndex,
PIDX_IsGlass,
PIDX_ReflectEnvironment,
PIDX_DiffuseTexture,
PIDX_SpecularTexture,
PIDX_ProgramPipeline,
};
void Serialize(bool writing);

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
