#pragma once
#include "Entity.h"

#pragma region  FORWARD_DECLARATIONS Component
#include "property.h"
class Entity;

#pragma endregion  FORWARD_DECLARATIONS Component
class Component
{
#pragma region  HEADER Component
public:
static const int ClassID = 718440320;
const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
Entity* GetParentEntity() { return m_ParentEntity; }; 
void SetParentEntity(Entity* value) { m_ParentEntity = value; }; 
private:
int m_ObjectID;
Entity* m_ParentEntity;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ComponentPIDX
{
PIDX_ObjectID,
PIDX_ParentEntity,
};
void Serialize(bool writing);

#pragma endregion  HEADER Component

virtual void Update() = 0;

};
