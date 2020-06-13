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
	UniformGroupType_PostEffects,
	UniformGroupType_Picking,
};

extern const std::string EnumValues_UniformGroupType[5];
extern const UniformGroupType Indirection_UniformGroupType[5];
const int EnumLength_UniformGroupType = 5;


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

	const UniformGroupType& GetUniformGroupType() { return m_UniformGroupType; };
	void SetUniformGroupType(UniformGroupType value) { m_UniformGroupType = value; };
private:
	UniformGroupType m_UniformGroupType;
public:
	static const int g_PropertyCount = 1;
	static const ClassPropertyData g_Properties[g_PropertyCount];

	enum g_GFXUniformGroupPIDX
	{
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

class PostEffectsUniformGroup : public GFXUniformGroup
{
public:
	PostEffectsUniformGroup()
	{
		SetUniformGroupType(UniformGroupType_PostEffects);

		AddUniform(GFXShaderStage_Fragment, "g_GammaCorrectionEnabled", LXType_bool);
		AddUniform(GFXShaderStage_Fragment, "g_GammaCorrectionExponent", LXType_float);
		AddUniform(GFXShaderStage_Fragment, "g_ToneMappingEnabled", LXType_bool);
	}
};

class PickingUniformGroup : public GFXUniformGroup
{
public:
	PickingUniformGroup()
	{
		SetUniformGroupType(UniformGroupType_Picking);

		AddUniform(GFXShaderStage_Fragment, "g_PickingID", LXType_float);
	}
};
