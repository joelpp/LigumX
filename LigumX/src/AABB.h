#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#pragma region  FORWARD_DECLARATIONS AABB
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS AABB

class AABB
{
public:
public:
public:
#pragma region  HEADER AABB
public:
static const int ClassID = 1726995287;
static constexpr const char* ClassName = "AABB";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const glm::vec3& GetOffset() { return m_Offset; }; 
void SetOffset(glm::vec3 value) { m_Offset = value; }; 
const glm::vec3& GetScale() { return m_Scale; }; 
void SetScale(glm::vec3 value) { m_Scale = value; }; 
private:
int m_ObjectID;
glm::vec3 m_Offset;
glm::vec3 m_Scale;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_AABBPIDX
{
PIDX_ObjectID,
PIDX_Offset,
PIDX_Scale,
};
void Serialize(bool writing);

#pragma endregion  HEADER AABB

AABB::AABB();

};
