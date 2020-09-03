#pragma once

#pragma region  FORWARD_DECLARATIONS ShadowParameters
#include "LXSystem.h"
class Serializer2;

class ShadowParameters;


#pragma endregion  FORWARD_DECLARATIONS ShadowParameters
#pragma region  HEADER ShadowParameters
class ShadowParameters : public LXObject
{
public:
static const int ClassID = -688385419;
static const LXType Type = LXType_ShadowParameters;
static constexpr const char* ClassName = "ShadowParameters";
virtual LXType GetLXType() { return LXType_ShadowParameters; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

glm::vec4& GetCascadeDistances() { return m_CascadeDistances; }; 
void SetCascadeDistances(const glm::vec4& value) { m_CascadeDistances = value; }; 
int GetFirstCascadeToRender() { return m_FirstCascadeToRender; }; 
void SetFirstCascadeToRender(int value) { m_FirstCascadeToRender = value; }; 
int GetLastCascadeToRender() { return m_LastCascadeToRender; }; 
void SetLastCascadeToRender(int value) { m_LastCascadeToRender = value; }; 
glm::vec3& GetLookAtTarget() { return m_LookAtTarget; }; 
void SetLookAtTarget(const glm::vec3& value) { m_LookAtTarget = value; }; 
private:
glm::vec4 m_CascadeDistances;
int m_FirstCascadeToRender = 0;
int m_LastCascadeToRender = 0;
glm::vec3 m_LookAtTarget = glm::vec3(0, 0, 0);
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ShadowParametersPIDX
{
PIDX_CascadeDistances,
PIDX_FirstCascadeToRender,
PIDX_LastCascadeToRender,
PIDX_LookAtTarget,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER ShadowParameters



};
