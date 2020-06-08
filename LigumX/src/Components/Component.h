#pragma once

#include "Entity.h"

#pragma region  FORWARD_DECLARATIONS Component
#include "LXSystem.h"
class Serializer2;

class Component;
class Entity;


#pragma endregion  FORWARD_DECLARATIONS Component

#pragma region  HEADER Component
class Component : public LXObject
{
public:
static const int ClassID = 390213760;
static const LXType Type = LXType_Component;
static constexpr const char* ClassName = "Component";
virtual LXType GetLXType() { return LXType_Component; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
Entity*& GetParentEntity() { return m_ParentEntity; }; 
void SetParentEntity(Entity* value) { m_ParentEntity = value; }; 
private:
bool m_Enabled = true;
Entity* m_ParentEntity = nullptr;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ComponentPIDX
{
PIDX_Enabled,
PIDX_ParentEntity,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER Component

virtual void Update();
virtual void DebugDisplay() { };

};
