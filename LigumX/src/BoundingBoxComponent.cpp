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

void BoundingBoxComponent::UpdateVertices()
{
	m_BoundingBox.UpdateVertices(m_ModelToWorldMatrix);
}

void BoundingBoxComponent::Update()
{
	if (GetParentEntity()->GetHasMoved())
	{
		GetParentEntity()->UpdateAABB();

		glm::vec3 position;

		position = GetParentEntity()->GetPosition();

		position += m_BoundingBox.GetStartPoint() + m_BoundingBox.GetScale() / 2.f;

		glm::mat4x4 toWorld = glm::mat4(1.0f); 
		toWorld = glm::translate(toWorld, position);
		toWorld = glm::rotate(toWorld, GetParentEntity()->GetRotationAngle(), GetParentEntity()->GetRotationAxis());
		toWorld = glm::scale(toWorld, GetParentEntity()->GetScale() * m_BoundingBox.GetScale());

		m_ModelToWorldMatrix = toWorld;

		UpdateVertices();
	}
}

void BoundingBoxComponent::SetStartAndScale(const glm::vec3& start, const glm::vec3& scale)
{
	m_BoundingBox.SetStartPoint(start);
	m_BoundingBox.SetScale(scale);
}
