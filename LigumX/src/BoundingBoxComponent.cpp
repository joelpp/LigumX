#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE BoundingBoxComponent
#include "BoundingBoxComponent.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData BoundingBoxComponent::g_Properties[] = 
{
{ "ObjectID", offsetof(BoundingBoxComponent, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "BoundingBox", offsetof(BoundingBoxComponent, m_BoundingBox), 0, LXType_AABB, false, LXType_None, 0, 0, 0, }, 
{ "UpdatesWithEntity", offsetof(BoundingBoxComponent, m_UpdatesWithEntity), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ModelToWorldMatrix", offsetof(BoundingBoxComponent, m_ModelToWorldMatrix), 0, LXType_glmmat4, false, LXType_None, 0, 0, 0, }, 
};
void BoundingBoxComponent::Serialize(bool writing)
{
	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
	std::string fileName = "BoundingBoxComponent_" + std::to_string(m_ObjectID) + ".LXobj";

	int fileMask = writing ? std::ios::out : std::ios::in;
	std::fstream objectStream(basePath + fileName, fileMask);

	if (objectStream.is_open())
	{
		Serializer::SerializeObject(this, objectStream, writing);
	}
}

#pragma endregion  CLASS_SOURCE BoundingBoxComponent

BoundingBoxComponent::BoundingBoxComponent()
{
	m_BoundingBox.SetScale(glm::vec3(1, 1, 1));

	m_ObjectID = rand();
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
