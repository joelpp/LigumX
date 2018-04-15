#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE BoundingBoxComponent

#include "BoundingBoxComponent.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData BoundingBoxComponent::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(BoundingBoxComponent, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(BoundingBoxComponent, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "BoundingBox", PIDX_BoundingBox, offsetof(BoundingBoxComponent, m_BoundingBox), 0, LXType_AABB, false, LXType_None, 0, 0, 0, }, 
{ "UpdatesWithEntity", PIDX_UpdatesWithEntity, offsetof(BoundingBoxComponent, m_UpdatesWithEntity), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ModelToWorldMatrix", PIDX_ModelToWorldMatrix, offsetof(BoundingBoxComponent, m_ModelToWorldMatrix), 0, LXType_glmmat4, false, LXType_None, 0, 0, 0, }, 
};
bool BoundingBoxComponent::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE BoundingBoxComponent

BoundingBoxComponent::BoundingBoxComponent()
{
	m_BoundingBox.SetScale(glm::vec3(1, 1, 1));

	m_ObjectID = g_ObjectManager->GetNewObjectID();
}

void BoundingBoxComponent::Update()
{
	glm::vec3 position;
	if (m_UpdatesWithEntity)
	{
		position = GetParentEntity()->GetPosition();
	}
	position += m_BoundingBox.GetStartPoint();

	glm::mat4x4 toWorld = glm::translate(glm::mat4(1.0), position);
	toWorld = glm::scale(toWorld, m_BoundingBox.GetScale());

	m_ModelToWorldMatrix = toWorld;
}

void BoundingBoxComponent::SetStartAndScale(const glm::vec3& start, const glm::vec3& scale)
{
	m_BoundingBox.SetStartPoint(start);
	m_BoundingBox.SetScale(scale);
	Update();

	m_BoundingBox.UpdateVertices();
}