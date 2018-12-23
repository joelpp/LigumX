#pragma once

#pragma region  FORWARD_DECLARATIONS AABB
#include "LXSystem.h"

class AABB;


#pragma endregion  FORWARD_DECLARATIONS AABB

#pragma region  HEADER AABB
class AABB : public LXObject
{
public:
static const int ClassID = 1726995287;
static const LXType Type = LXType_AABB;
static constexpr const char* ClassName = "AABB";
typedef LXObject super;

glm::vec3& GetStartPoint() { return m_StartPoint; }; 
void SetStartPoint(const glm::vec3& value) { m_StartPoint = value; }; 
glm::vec3& GetScale() { return m_Scale; }; 
void SetScale(const glm::vec3& value) { m_Scale = value; }; 
private:
glm::vec3 m_StartPoint = glm::vec3(0, 0, 0);
glm::vec3 m_Scale = glm::vec3(0, 0, 0);
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_AABBPIDX
{
PIDX_StartPoint,
PIDX_Scale,
};
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();

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
