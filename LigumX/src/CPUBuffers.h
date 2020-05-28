#pragma once

#include "glm\glm.hpp"

#pragma region  FORWARD_DECLARATIONS CPUBuffers
#include "LXSystem.h"
class Serializer2;

class CPUBuffers;


#pragma endregion  FORWARD_DECLARATIONS CPUBuffers
#pragma region  HEADER CPUBuffers
class CPUBuffers : public LXObject
{
public:
static const int ClassID = 2143754276;
static const LXType Type = LXType_CPUBuffers;
static constexpr const char* ClassName = "CPUBuffers";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

std::vector<glm::vec3>& GetVertexPositions() { return m_VertexPositions; }; 
void SetVertexPositions(std::vector<glm::vec3> value) { m_VertexPositions = value; }; 
void AddTo_VertexPositions(glm::vec3 value) { m_VertexPositions.push_back(value); };
std::vector<glm::vec3>& GetVertexNormals() { return m_VertexNormals; }; 
void SetVertexNormals(std::vector<glm::vec3> value) { m_VertexNormals = value; }; 
void AddTo_VertexNormals(glm::vec3 value) { m_VertexNormals.push_back(value); };
std::vector<glm::vec2>& GetVertexUVs() { return m_VertexUVs; }; 
void SetVertexUVs(std::vector<glm::vec2> value) { m_VertexUVs = value; }; 
void AddTo_VertexUVs(glm::vec2 value) { m_VertexUVs.push_back(value); };
std::vector<int>& GetIndexBuffer() { return m_IndexBuffer; }; 
void SetIndexBuffer(std::vector<int> value) { m_IndexBuffer = value; }; 
void AddTo_IndexBuffer(int value) { m_IndexBuffer.push_back(value); };
private:
std::vector<glm::vec3> m_VertexPositions;
std::vector<glm::vec3> m_VertexNormals;
std::vector<glm::vec2> m_VertexUVs;
std::vector<int> m_IndexBuffer;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CPUBuffersPIDX
{
PIDX_VertexPositions,
PIDX_VertexNormals,
PIDX_VertexUVs,
PIDX_IndexBuffer,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(CPUBuffers& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER CPUBuffers

public:

	void AppendBuffer(const CPUBuffers& buffer);


};
