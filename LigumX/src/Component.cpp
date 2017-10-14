#include "Component.h"

#pragma region  CLASS_SOURCE Component
#include "Component.h"
#include <cstddef>
const ClassPropertyData Component::g_Properties[] = 
{
{ "ParentEntity", offsetof(Component, m_ParentEntity), 0, LXType_Entity, true, LXType_None, 0, 0, 0, }, 
};

#pragma endregion  CLASS_SOURCE Component
