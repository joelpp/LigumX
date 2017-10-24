#pragma once
#include "Entity.h"

#pragma region  FORWARD_DECLARATIONS Component
#include "property.h"

class Component;
class Entity;


#pragma endregion  FORWARD_DECLARATIONS Component
class Component
{
#pragma region  HEADER Component
public:
static const int ClassID = 718440320;
static constexpr const char* ClassName = "Component";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
Entity* GetParentEntity() { return m_ParentEntity; }; 
void SetParentEntity(Entity* value) { m_ParentEntity = value; }; 
private:
int m_ObjectID;
std::string m_Name;
Entity* m_ParentEntity;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ComponentPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_ParentEntity,
};
void Serialize(bool writing);

#pragma endregion  HEADER Component

virtual void Update() = 0;

};
