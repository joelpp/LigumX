#pragma once

#include "Component.h"
#include "AABB.h"

#pragma region  FORWARD_DECLARATIONS BoundingBoxComponent
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS BoundingBoxComponent


class BoundingBoxComponent : public Component
{
#pragma region  HEADER BoundingBoxComponent
public:
static const int ClassID = 1140622857;
static constexpr const char* ClassName = "BoundingBoxComponent";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const AABB& GetBoundingBox() { return m_BoundingBox; }; 
void SetBoundingBox(AABB value) { m_BoundingBox = value; }; 
const bool& GetUpdatesWithEntity() { return m_UpdatesWithEntity; }; 
void SetUpdatesWithEntity(bool value) { m_UpdatesWithEntity = value; }; 
const glm::mat4& GetModelToWorldMatrix() { return m_ModelToWorldMatrix; }; 
void SetModelToWorldMatrix(glm::mat4 value) { m_ModelToWorldMatrix = value; }; 
private:
int m_ObjectID;
AABB m_BoundingBox;
bool m_UpdatesWithEntity;
glm::mat4 m_ModelToWorldMatrix;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_BoundingBoxComponentPIDX
{
PIDX_ObjectID,
PIDX_BoundingBox,
PIDX_UpdatesWithEntity,
PIDX_ModelToWorldMatrix,
};
void Serialize(bool writing);

#pragma endregion  HEADER BoundingBoxComponent

BoundingBoxComponent::BoundingBoxComponent();


void Update();
};
