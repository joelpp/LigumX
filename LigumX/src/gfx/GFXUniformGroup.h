#pragma once

#pragma region  FORWARD_DECLARATIONS GFXUniformGroup
#include "LXSystem.h"
class Serializer2;

class GFXUniformGroup;

enum UniformGroupType
{
UniformGroupType_ShadowMap,
UniformGroupType_LightingOptions,
UniformGroupType_View,
};

extern const std::string EnumValues_UniformGroupType[3];
extern const UniformGroupType Indirection_UniformGroupType[3];
const int EnumLength_UniformGroupType = 3;


#pragma endregion  FORWARD_DECLARATIONS GFXUniformGroup

enum GFXShaderStage
{
	GFXShaderStage_Vertex,
	GFXShaderStage_Fragment,
	GFXShaderStage_Count,
};

class ProgramPipeline;

#include "GFXUniformDescription.h"

#pragma region  HEADER GFXUniformGroup
class GFXUniformGroup : public LXObject
{
public:
static const int ClassID = -2034811565;
static const LXType Type = LXType_GFXUniformGroup;
static constexpr const char* ClassName = "GFXUniformGroup";
virtual LXType GetLXType() { return LXType_GFXUniformGroup; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

const std::string& GetGroupName() { return m_GroupName; }; 
void SetGroupName(std::string value) { m_GroupName = value; }; 
const UniformGroupType& GetUniformGroupType() { return m_UniformGroupType; }; 
void SetUniformGroupType(UniformGroupType value) { m_UniformGroupType = value; }; 
private:
std::string m_GroupName;
UniformGroupType m_UniformGroupType;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_GFXUniformGroupPIDX
{
PIDX_GroupName,
PIDX_UniformGroupType,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER GFXUniformGroup

public:

	GFXUniformGroup();

	void AddUniform(GFXShaderStage stage, const char* name, LXType type);
	void AddUniform(GFXShaderStage stage, GFXUniformDescription& desc);
	GFXUniformDescription* GetUniformDescription(GFXShaderStage stage, const char* name);

	LXVector<GFXUniformDescription>& GetUniforms(GFXShaderStage stage) { return m_Uniforms[(int)stage]; }
	
	void GetLocationsFromShader(ProgramPipeline* pipeline);

private:
	LXVector<LXVector<GFXUniformDescription>> m_Uniforms;

};
