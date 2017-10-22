#include <string> // todo this is bs only to make the lxgen work

#pragma region  CLASS_SOURCE AABB
#include "AABB.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData AABB::g_Properties[] = 
{
{ "ObjectID", offsetof(AABB, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Offset", offsetof(AABB, m_Offset), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "Scale", offsetof(AABB, m_Scale), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
};
void AABB::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE AABB

AABB::AABB()
{
	m_ObjectID = rand();
}
