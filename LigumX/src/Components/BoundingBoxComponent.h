#pragma once

#include "Component.h"
#include "AABB.h"

#pragma region  FORWARD_DECLARATIONS BoundingBoxComponent
#include "LXSystem.h"
class Serializer2;

class BoundingBoxComponent;


#pragma endregion  FORWARD_DECLARATIONS BoundingBoxComponent

#pragma region  HEADER BoundingBoxComponent
class BoundingBoxComponent : public Component
{
public:
static const int ClassID = -960579863;
static const LXType Type = LXType_BoundingBoxComponent;
static constexpr const char* ClassName = "BoundingBoxComponent";
virtual LXType GetLXType() { return LXType_BoundingBoxComponent; }
virtual const char* GetLXClassName() { return ClassName; }
typedef Component super;

const AABB& GetBoundingBox() { return m_BoundingBox; }; 
void SetBoundingBox(AABB value) { m_BoundingBox = value; }; 
bool GetUpdatesWithEntity() { return m_UpdatesWithEntity; }; 
void SetUpdatesWithEntity(bool value) { m_UpdatesWithEntity = value; }; 
glm::mat4& GetModelToWorldMatrix() { return m_ModelToWorldMatrix; }; 
void SetModelToWorldMatrix(const glm::mat4& value) { m_ModelToWorldMatrix = value; }; 
private:
AABB m_BoundingBox;
bool m_UpdatesWithEntity = false;
glm::mat4 m_ModelToWorldMatrix;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_BoundingBoxComponentPIDX
{
PIDX_BoundingBox,
PIDX_UpdatesWithEntity,
PIDX_ModelToWorldMatrix,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER BoundingBoxComponent

BoundingBoxComponent::BoundingBoxComponent();
void SetStartAndScale(const glm::vec3& start, const glm::vec3& scale);

void UpdateVertices();
void Update();
};