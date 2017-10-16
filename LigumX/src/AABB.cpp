#include <string> // todo this is bs only to make the lxgen work

#pragma region  CLASS_SOURCE AABB
#include "AABB.h"
#include "serializer.h"
#include <cstddef>
const ClassPropertyData AABB::g_Properties[] = 
{
{ "ObjectID", offsetof(AABB, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Offset", offsetof(AABB, m_Offset), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "Scale", offsetof(AABB, m_Scale), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
};
void AABB::Serialize(bool writing)
{
	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
	std::string fileName = "AABB_" + std::to_string(m_ObjectID) + ".LXobj";

	int fileMask = writing ? std::ios::out : std::ios::in;
	std::fstream objectStream(basePath + fileName, fileMask);

	if (objectStream.is_open())
	{
		if (objectStream.is_open())
		{
			Serializer::SerializeObject(this, objectStream, writing);
		}
	}
}

#pragma endregion  CLASS_SOURCE AABB

AABB::AABB()
{
	m_ObjectID = rand();
}