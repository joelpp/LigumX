#pragma once


#pragma region  FORWARD_DECLARATIONS Material
#include "LXSystem.h"
class Serializer2;

class Material;
class Texture;

enum ShaderFamily
{
ShaderFamily_Basic,
ShaderFamily_SolidColor,
ShaderFamily_Envmap,
ShaderFamily_Terrain,
ShaderFamily_Roads,
};

extern const std::string EnumValues_ShaderFamily[5];
extern const ShaderFamily Indirection_ShaderFamily[5];
const int EnumLength_ShaderFamily = 5;


#pragma endregion  FORWARD_DECLARATIONS Material

class ProgramPipeline;

#pragma region  HEADER Material
class Material : public LXObject
{
public:
static const int ClassID = 58952128;
static const LXType Type = LXType_Material;
static constexpr const char* ClassName = "Material";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
glm::vec3& GetAmbientColor() { return m_AmbientColor; }; 
void SetAmbientColor(const glm::vec3& value) { m_AmbientColor = value; }; 
glm::vec3& GetDiffuseColor() { return m_DiffuseColor; }; 
void SetDiffuseColor(const glm::vec3& value) { m_DiffuseColor = value; }; 
glm::vec3& GetSpecularColor() { return m_SpecularColor; }; 
void SetSpecularColor(const glm::vec3& value) { m_SpecularColor = value; }; 
bool GetIsPBR() { return m_IsPBR; }; 
void SetIsPBR(bool value) { m_IsPBR = value; }; 
float GetShininess() { return m_Shininess; }; 
void SetShininess(float value) { m_Shininess = value; }; 
float GetMetallic() { return m_Metallic; }; 
void SetMetallic(float value) { m_Metallic = value; }; 
float GetRoughness() { return m_Roughness; }; 
void SetRoughness(float value) { m_Roughness = value; }; 
float GetAO() { return m_AO; }; 
void SetAO(float value) { m_AO = value; }; 
float GetEmissiveFactor() { return m_EmissiveFactor; }; 
void SetEmissiveFactor(float value) { m_EmissiveFactor = value; }; 
bool GetDiffuseTextureEnabled() { return m_DiffuseTextureEnabled; }; 
void SetDiffuseTextureEnabled(bool value) { m_DiffuseTextureEnabled = value; }; 
bool GetSpecularTextureEnabled() { return m_SpecularTextureEnabled; }; 
void SetSpecularTextureEnabled(bool value) { m_SpecularTextureEnabled = value; }; 
bool GetUnlit() { return m_Unlit; }; 
void SetUnlit(bool value) { m_Unlit = value; }; 
float GetRefractionIndex() { return m_RefractionIndex; }; 
void SetRefractionIndex(float value) { m_RefractionIndex = value; }; 
bool GetIsGlass() { return m_IsGlass; }; 
void SetIsGlass(bool value) { m_IsGlass = value; }; 
bool GetReflectEnvironment() { return m_ReflectEnvironment; }; 
void SetReflectEnvironment(bool value) { m_ReflectEnvironment = value; }; 
Texture*& GetDiffuseTexture() { return m_DiffuseTexture; }; 
void SetDiffuseTexture(Texture* value) { SetDiffuseTexture_Callback(value); }; 
void SetDiffuseTexture_Callback(Texture* value);
Texture*& GetSpecularTexture() { return m_SpecularTexture; }; 
void SetSpecularTexture(Texture* value) { SetSpecularTexture_Callback(value); }; 
void SetSpecularTexture_Callback(Texture* value);
Texture*& GetHeightfieldTexture() { return m_HeightfieldTexture; }; 
void SetHeightfieldTexture(Texture* value) { m_HeightfieldTexture = value; }; 
const ShaderFamily& GetShaderFamily() { return m_ShaderFamily; }; 
void SetShaderFamily(ShaderFamily value) { m_ShaderFamily = value; }; 
private:
bool m_Enabled = false;
glm::vec3 m_AmbientColor = glm::vec3(0, 0, 0);
glm::vec3 m_DiffuseColor = glm::vec3(0, 0, 0);
glm::vec3 m_SpecularColor = glm::vec3(0, 0, 0);
bool m_IsPBR = false;
float m_Shininess = 0.f;
float m_Metallic = 0.f;
float m_Roughness = 0.f;
float m_AO = 0.f;
float m_EmissiveFactor = 0.f;
bool m_DiffuseTextureEnabled = false;
bool m_SpecularTextureEnabled = false;
bool m_Unlit = false;
float m_RefractionIndex = 0.f;
bool m_IsGlass = false;
bool m_ReflectEnvironment = false;
Texture* m_DiffuseTexture = nullptr;
Texture* m_SpecularTexture = nullptr;
Texture* m_HeightfieldTexture = nullptr;
ShaderFamily m_ShaderFamily;
public:
static const int g_PropertyCount = 20;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MaterialPIDX
{
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
PIDX_HeightfieldTexture,
PIDX_ShaderFamily,
};
virtual void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER Material

public:

	Material();
	Material(ProgramPipeline* programPipeline);
	Material(ProgramPipeline* programPipeline, glm::vec3 albedo);
	Material(ShaderFamily family);

	void InitializeToDefaults();

	static void Initialize();
	static Material* Get(std::string materialName);
	static void Add(std::string materialName, Material* material);

	ProgramPipeline*& GetProgramPipeline() { return m_ProgramPipeline; };
	void SetProgramPipeline(ProgramPipeline* value) { m_ProgramPipeline = value; };

private:

	ProgramPipeline* m_ProgramPipeline = nullptr;


	static std::unordered_map<std::string, Material*> MaterialList;
};
typedef std::unordered_map<std::string, Material*> MaterialMap;
