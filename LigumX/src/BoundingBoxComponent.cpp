#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE BoundingBoxComponent
#include "BoundingBoxComponent.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData BoundingBoxComponent::g_Properties[] = 
{
{ "ObjectID", offsetof(BoundingBoxComponent, m_ObjectID), 0, LXType_int, false, LXType_None, PropertyFlags_NonEditable, 0, 0, }, 
{ "BoundingBox", offsetof(BoundingBoxComponent, m_BoundingBox), 0, LXType_AABB, false, LXType_None, 0, 0, 0, }, 
{ "UpdatesWithEntity", offsetof(BoundingBoxComponent, m_UpdatesWithEntity), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ModelToWorldMatrix", offsetof(BoundingBoxComponent, m_ModelToWorldMatrix), 0, LXType_glmmat4, false, LXType_None, 0, 0, 0, }, 
};
void BoundingBoxComponent::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE BoundingBoxComponent

BoundingBoxComponent::BoundingBoxComponent()
{
	m_BoundingBox.SetScale(glm::vec3(1, 1, 1));

	m_ObjectID = g_ObjectIDManager->GetObjectID();
}

void BoundingBoxComponent::Update()
{
	glm::vec3 position;
	if (m_UpdatesWithEntity)
	{
		position = GetParentEntity()->GetPosition();
	}
	position += m_BoundingBox.GetOffset();

	glm::mat4x4 toWorld = glm::translate(glm::mat4(1.0), position);
	toWorld = glm::scale(toWorld, m_BoundingBox.GetScale());

	m_ModelToWorldMatrix = toWorld;
}
