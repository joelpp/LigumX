#include "CPUBuffers.h"

#pragma region  CLASS_SOURCE CPUBuffers

#include "CPUBuffers.h"
#include "serializer.h"
const ClassPropertyData CPUBuffers::g_Properties[] = 
{
{ "VertexPositions", PIDX_VertexPositions, offsetof(CPUBuffers, m_VertexPositions), 0, LXType_stdvector, sizeof(std::vector<glm::vec3>), LXType_stdvector, false, LXType_glmvec3, false, 0, 0, 0, 0,}, 
{ "VertexNormals", PIDX_VertexNormals, offsetof(CPUBuffers, m_VertexNormals), 0, LXType_stdvector, sizeof(std::vector<glm::vec3>), LXType_stdvector, false, LXType_glmvec3, false, 0, 0, 0, 0,}, 
{ "VertexUVs", PIDX_VertexUVs, offsetof(CPUBuffers, m_VertexUVs), 0, LXType_stdvector, sizeof(std::vector<glm::vec2>), LXType_stdvector, false, LXType_glmvec2, false, 0, 0, 0, 0,}, 
{ "IndexBuffer", PIDX_IndexBuffer, offsetof(CPUBuffers, m_IndexBuffer), 0, LXType_stdvector, sizeof(std::vector<int>), LXType_stdvector, false, LXType_int, false, 0, 0, 0, 0,}, 
};
void CPUBuffers::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeVector(g_Properties[PIDX_VertexPositions], m_VertexPositions);
	serializer.SerializeVector(g_Properties[PIDX_VertexNormals], m_VertexNormals);
	serializer.SerializeVector(g_Properties[PIDX_VertexUVs], m_VertexUVs);
	serializer.SerializeVector(g_Properties[PIDX_IndexBuffer], m_IndexBuffer);
}
bool CPUBuffers::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool CPUBuffers::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_VertexPositions], m_VertexPositions  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_VertexNormals], m_VertexNormals  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_VertexUVs], m_VertexUVs  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_IndexBuffer], m_IndexBuffer  );
	return true;
}
void CPUBuffers::Clone(CPUBuffers& other)
{
	super::Clone(other);
	other.SetVertexPositions(m_VertexPositions);
	other.SetVertexNormals(m_VertexNormals);
	other.SetVertexUVs(m_VertexUVs);
	other.SetIndexBuffer(m_IndexBuffer);
}
const char* CPUBuffers::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE CPUBuffers


void CPUBuffers::AppendBuffer(const CPUBuffers& buffer)
{
	for (const glm::vec3& v : buffer.m_VertexPositions)
	{
		m_VertexPositions.push_back(v);
	}
	for (const glm::vec3& v : buffer.m_VertexNormals)
	{
		m_VertexNormals.push_back(v);
	}
	for (const glm::vec2& v : buffer.m_VertexUVs)
	{
		m_VertexUVs.push_back(v);
	}
	for (const int v : buffer.m_IndexBuffer)
	{
		m_IndexBuffer.push_back(v);	}
}
