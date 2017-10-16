#include "Component.h"

#pragma region  CLASS_SOURCE Component
#include "Component.h"
#include "serializer.h"
#include <cstddef>
const ClassPropertyData Component::g_Properties[] = 
{
{ "ObjectID", offsetof(Component, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "ParentEntity", offsetof(Component, m_ParentEntity), 0, LXType_Entity, true, LXType_None, 0, 0, 0, }, 
};
void Component::Serialize(bool writing)
{
	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
	std::string fileName = "Component_" + std::to_string(m_ObjectID) + ".LXobj";

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

#pragma endregion  CLASS_SOURCE Component

