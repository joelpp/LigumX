#include <string> // todo this is bs only to make the lxgen work

#pragma region  CLASS_SOURCE AABB

#include "AABB.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData AABB::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(AABB, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(AABB, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "StartPoint", PIDX_StartPoint, offsetof(AABB, m_StartPoint), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "Scale", PIDX_Scale, offsetof(AABB, m_Scale), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
};
bool AABB::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE AABB


AABB::AABB()
{
	m_ObjectID = g_ObjectManager->GetNewObjectID();
	m_Vertices.resize(8);
}

AABB AABB::BuildFromStartPointAndScale(const glm::vec2& startPoint, float scale)
{
	AABB aabb;
	aabb.SetScale(glm::vec3(scale));
	aabb.SetStartPoint(glm::vec3(startPoint, 0));

	return aabb;
}

AABB AABB::BuildFromStartPointAndScale(const glm::vec2& startPoint, const glm::vec3& scale)
{
	AABB aabb;
	aabb.SetScale(scale);
	aabb.SetStartPoint(glm::vec3(startPoint, 0));

	return aabb;
}

AABB AABB::BuildFromStartPointAndScale(const glm::vec3& startPoint, float scale)
{
	AABB aabb;
	aabb.SetScale(glm::vec3(scale));
	aabb.SetStartPoint(startPoint);

	return aabb;
}

AABB AABB::BuildFromStartPointAndScale(const glm::vec3& startPoint, const glm::vec3& scale)
{
	AABB aabb;
	aabb.SetScale(scale);
	aabb.SetStartPoint(startPoint);

	return aabb;
}

void AABB::UpdateVertices()
{
	m_Vertices[0] = m_StartPoint + m_Scale * glm::vec3(0, 0, 0);
	m_Vertices[1] = m_StartPoint + m_Scale * glm::vec3(0, 0, 1);
	m_Vertices[2] = m_StartPoint + m_Scale * glm::vec3(0, 1, 0);
	m_Vertices[3] = m_StartPoint + m_Scale * glm::vec3(0, 1, 1);
	m_Vertices[4] = m_StartPoint + m_Scale * glm::vec3(1, 0, 0);
	m_Vertices[5] = m_StartPoint + m_Scale * glm::vec3(1, 0, 1);
	m_Vertices[6] = m_StartPoint + m_Scale * glm::vec3(1, 1, 0);
	m_Vertices[7] = m_StartPoint + m_Scale * glm::vec3(1, 1, 1);
}
