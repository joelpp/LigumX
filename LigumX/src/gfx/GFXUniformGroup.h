#pragma once

#pragma region  FORWARD_DECLARATIONS GFXUniformGroup
#include "LXSystem.h"
class Serializer2;

class GFXUniformGroup;

enum UniformGroupType
{
UniformGroupType_ShadowMap,
UniformGroupType_LightingOptions,
UniformGroupType_DisplayOptions,
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
private:
std::string m_GroupName;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_GFXUniformGroupPIDX
{
PIDX_GroupName,
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
	GFXUniformDescription& GetUniformDescription(GFXShaderStage stage, const char* name);
	int GetUniformLocation(GFXShaderStage stage, const char* name);
	LXMap<LXString, GFXUniformDescription>& GetUniformMap(GFXShaderStage stage) { return m_Uniforms[(int)stage]; }

private:
	LXVector<LXMap<LXString, GFXUniformDescription>> m_Uniforms;

};
