#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#pragma region  FORWARD_DECLARATIONS AABB
#include "property.h"

class AABB;


#pragma endregion  FORWARD_DECLARATIONS AABB

class AABB
{
public:
public:
public:
#pragma region  HEADER AABB
public:
static const int ClassID = 1726995287;
static const LXType Type = LXType_AABB;
static constexpr const char* ClassName = "AABB";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const glm::vec3& GetStartPoint() { return m_StartPoint; }; 
void SetStartPoint(glm::vec3 value) { m_StartPoint = value; }; 
const glm::vec3& GetScale() { return m_Scale; }; 
void SetScale(glm::vec3 value) { m_Scale = value; }; 
private:
int m_ObjectID;
std::string m_Name;
glm::vec3 m_StartPoint = glm::vec3(0, 0, 0);
glm::vec3 m_Scale = glm::vec3(0, 0, 0);
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_AABBPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_StartPoint,
PIDX_Scale,
};
bool Serialize(bool writing);

#pragma endregion  HEADER AABB

AABB::AABB();
void UpdateVertices(const glm::mat4x4& toWorld);

static AABB AABB::BuildFromStartPointAndScale(const glm::vec2& startPoint, float scale);
static AABB AABB::BuildFromStartPointAndScale(const glm::vec2& startPoint, const glm::vec3& scale);
static AABB AABB::BuildFromStartPointAndScale(const glm::vec3& startPoint, float scale);
static AABB AABB::BuildFromStartPointAndScale(const glm::vec3& startPoint, const glm::vec3& scale);

const std::vector<glm::vec3>& GetVertices() const { return m_Vertices; }

private:

	std::vector<glm::vec3> m_Vertices;
};
