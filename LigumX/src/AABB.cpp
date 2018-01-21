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
{ "Offset", PIDX_Offset, offsetof(AABB, m_Offset), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
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
}

void AABB::SetMidPoint(const glm::vec3& midPoint)
{
	m_Offset = midPoint + (m_Scale / 2.f);
}
