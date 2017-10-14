#include <string> // todo this is bs only to make the lxgen work

#pragma region  CLASS_SOURCE AABB
#include "AABB.h"
#include <cstddef>
const ClassPropertyData AABB::g_Properties[] = 
{
{ "Offset", offsetof(AABB, m_Offset), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "Scale", offsetof(AABB, m_Scale), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
};

#pragma endregion  CLASS_SOURCE AABB
