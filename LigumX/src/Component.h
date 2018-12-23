#pragma once

#include "Entity.h"

#pragma region  FORWARD_DECLARATIONS Component
#include "LXSystem.h"

class Component;
class Entity;


#pragma endregion  FORWARD_DECLARATIONS Component

#pragma region  HEADER Component
class Component : public LXObject
{
public:
static const int ClassID = 718440320;
static const LXType Type = LXType_Component;
static constexpr const char* ClassName = "Component";
typedef LXObject super;

Entity*& GetParentEntity() { return m_ParentEntity; }; 
void SetParentEntity(Entity* value) { m_ParentEntity = value; }; 
private:
Entity* m_ParentEntity = nullptr;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ComponentPIDX
{
PIDX_ParentEntity,
};
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();

#pragma endregion  HEADER Component

virtual void Update() = 0;

};
