

#include "Mesh.h"
#include "LigumX.h"
#include "Renderer.h"
#include "Model.h" // todo rmove horrible hack to load objs
using namespace std;
#pragma region  CLASS_SOURCE Mesh

#include "Mesh.h"
#include "serializer.h"
const ClassPropertyData Mesh::g_Properties[] = 
{
{ "UsesIndexBuffer", PIDX_UsesIndexBuffer, offsetof(Mesh, m_UsesIndexBuffer), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "WireframeRendering", PIDX_WireframeRendering, offsetof(Mesh, m_WireframeRendering), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "PointRendering", PIDX_PointRendering, offsetof(Mesh, m_PointRendering), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void Mesh::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_UsesIndexBuffer], m_UsesIndexBuffer);
	serializer.SerializeBool(g_Properties[PIDX_WireframeRendering], m_WireframeRendering);
	serializer.SerializeBool(g_Properties[PIDX_PointRendering], m_PointRendering);
}
bool Mesh::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Mesh::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UsesIndexBuffer], &m_UsesIndexBuffer  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_WireframeRendering], &m_WireframeRendering  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PointRendering], &m_PointRendering  );
	return true;
}
void Mesh::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Mesh* other = (Mesh*) otherObj;
	other->SetUsesIndexBuffer(m_UsesIndexBuffer);
	other->SetWireframeRendering(m_WireframeRendering);
	other->SetPointRendering(m_PointRendering);
}
const char* Mesh::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Mesh

Mesh::Mesh()
{
    m_WireframeRendering = false;
    m_UsesIndexBuffer = false;
    m_PointRendering = false;
	m_PrimitiveMode = GL::PrimitiveMode::Triangles;
}

Mesh::Mesh(const std::vector<glm::vec3>& vertices, GL::PrimitiveMode primitiveMode)
{
  m_buffers.SetVertexPositions(vertices);

  m_PrimitiveMode = primitiveMode;
  m_WireframeRendering = false;
  m_UsesIndexBuffer = false;
  m_PointRendering = false;

  padBuffer(VERTEX_UVS);
  CreateBuffers();
}


Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, GL::PrimitiveMode primitiveMode, bool usePointRendering)
{
	m_buffers.SetVertexUVs(uvs);
	m_buffers.SetVertexPositions(vertices);

	m_PrimitiveMode = primitiveMode;
	m_WireframeRendering = false;
	m_UsesIndexBuffer = false;
	m_PointRendering = false;

	CreateBuffers();
}


Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<int>& indices, GL::PrimitiveMode primitiveMode, bool usePointRendering)
{
	m_buffers.SetVertexPositions(vertices);
	m_buffers.SetIndexBuffer(indices);

	m_PrimitiveMode = primitiveMode;
	m_PointRendering = usePointRendering;

	m_UsesIndexBuffer = true;

	padBuffer(VERTEX_UVS);

	CreateBuffers();
}


Mesh::Mesh(const std::vector<glm::vec3>& vertices, GL::PrimitiveMode primitiveMode, bool usePointRendering)
{
  m_buffers.SetVertexPositions(vertices);
  m_PrimitiveMode = primitiveMode;
  m_PointRendering = usePointRendering;
  padBuffer(VERTEX_UVS);
  CreateBuffers();
}

void Mesh::padBuffer(EBufferType bufferType)
{
  int numToFill;
  if (bufferType == VERTEX_UVS)
  {
    numToFill = (int) (m_buffers.GetVertexPositions().size() - m_buffers.GetVertexUVs().size());
    for (int i = 0; i < numToFill; ++i)
    {
      m_buffers.AddTo_VertexUVs(glm::vec2(0,0));
    }
  }
}

Mesh::Mesh(const CPUBuffers& cpuBuffers, GL::PrimitiveMode primitiveMode, bool usePointRendering)
{
	m_buffers = cpuBuffers;

	m_PrimitiveMode = primitiveMode;
	m_PointRendering = usePointRendering;
	m_WireframeRendering = false;

	m_UsesIndexBuffer = m_buffers.GetIndexBuffer().size() > 0;

	if (m_buffers.GetVertexUVs().size() == 0)
	{
		padBuffer(VERTEX_UVS);
	}

	CreateBuffers();
}


void Mesh::CreateBuffers()
{
	// TODO: I'm not quite convinced this belongs here. or does it?
    LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, m_VBOs.glidPositions, m_buffers.GetVertexPositions());
    // Renderer::createGLBuffer(m_VBOs.glidNormals,   m_buffers.m_vertexNormals);
	LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, m_VBOs.glidUVs,			m_buffers.GetVertexUVs());
	
    glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidUVs);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	if (m_buffers.GetVertexNormals().size() > 0)
	{
		LigumX::GetInstance().m_Renderer->createGLBuffer(GL_ARRAY_BUFFER, m_VBOs.glidNormals, m_buffers.GetVertexNormals());
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidNormals);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	if (m_buffers.GetIndexBuffer().size() > 0)
	{
		glGenBuffers(1, &m_VBOs.glidIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs.glidIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_buffers.GetIndexBuffer().size() * sizeof(int), m_buffers.GetIndexBuffer().data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


}
