#include "stdafx.h"

#include "Mesh.h"
#include "Renderer.h"
#include "Model.h" // todo rmove horrible hack to load objs
using namespace std;

Mesh::Mesh()
{
    m_wireframeRendering = false;
    m_usesIndexBuffer = false;
    m_pointRendering = false;
}

Mesh::Mesh(std::vector<glm::vec3> vertices, GLenum renderingMode)
{
  m_buffers.vertexPositions = vertices;
  m_renderingMode = renderingMode;

  padBuffer(VERTEX_UVS);
  createBuffers();
}

Mesh::Mesh(std::vector<glm::vec3> vertices, GLenum renderingMode, bool usePointRendering)
{
  m_buffers.vertexPositions = vertices;
  m_renderingMode = renderingMode;
  m_pointRendering = usePointRendering;
  padBuffer(VERTEX_UVS);
  createBuffers();
}

void Mesh::padBuffer(EBufferType bufferType)
{
  int numToFill;
  if (bufferType == VERTEX_UVS)
  {
    numToFill = m_buffers.vertexPositions.size() - m_buffers.m_vertexUVs.size();
    for (int i = 0; i < numToFill; ++i)
    {
      m_buffers.m_vertexUVs.push_back(glm::vec2(0,0));
    }
  }
}

void Mesh::createBuffers()
{
	// TODO: I'm not quite convinced this belongs here. or does it?
    Renderer::GetInstance().createGLBuffer(GL_ARRAY_BUFFER, m_VBOs.glidPositions, m_buffers.vertexPositions);
    // Renderer::createGLBuffer(m_VBOs.glidNormals,   m_buffers.m_vertexNormals);
	Renderer::GetInstance().createGLBuffer(GL_ARRAY_BUFFER, m_VBOs.glidUVs,			m_buffers.m_vertexUVs);
	Renderer::GetInstance().createGLBuffer(GL_ARRAY_BUFFER, m_VBOs.glidNormals, 	   m_buffers.m_vertexNormals);
	
    glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidUVs);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidNormals);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &m_VBOs.glidIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs.glidIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_buffers.indexBuffer.size() * sizeof(int), m_buffers.indexBuffer.data(), GL_DYNAMIC_DRAW); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

DefaultMeshes* g_DefaultMeshes;

void DefaultMeshes::InitializeDefaultCube()
{
//	DefaultCubeMesh = new Mesh();
//
//	std::vector<glm::vec3>& points = DefaultQuadMesh->m_buffers.vertexPositions;
//	std::vector<glm::vec2>& UVs = DefaultQuadMesh->m_buffers.m_vertexUVs;
//	std::vector<glm::vec3>& normals = DefaultQuadMesh->m_buffers.m_vertexNormals;
//	std::vector<int>& indexBuffer = DefaultQuadMesh->m_buffers.indexBuffer;
//
//	/*
// points.push_back(glm::vec3(-0.5, 0.5, -0.5);
// points.push_back(glm::vec3(-0.5, 0.5, 0.5);
// points.push_back(glm::vec3(0.5, 0.5, 0.5);
// points.push_back(glm::vec3(0.5, 0.5, -0.5);
// points.push_back(glm::vec3(-0.5, -0.5, -0.5);
// points.push_back(glm::vec3(-0.5 ,-0.5, 0.5);
// points.push_back(glm::vec3(0.5, -0.5, 0.5);
// points.push_back(glm::vec3(0.5, -0.5, -0.5);
//
//vt 0 1
//vt 0 0
//vt 1 0
//vt 1 1
//
//vn 0 1 0
//vn -1 0 0
//vn 1 0 0
//vn 0 0 -1
//vn 0 0 1
//vn 0 -1 0
//*/
//
//	// bottom
//	points.push_back(glm::vec3(-0.5, 0.5, -0.5));
//	points.push_back(glm::vec3(-0.5, 0.5, 0.5));
//	points.push_back(glm::vec3(0.5, 0.5, 0.5));
//	points.push_back(glm::vec3(0.5, 0.5, -0.5));
//	points.push_back(glm::vec3(-0.5, -0.5, -0.5));
//	points.push_back(glm::vec3(-0.5, -0.5, 0.5));
//	points.push_back(glm::vec3(0.5, -0.5, 0.5));
//	points.push_back(glm::vec3(0.5, -0.5, -0.5));
//
//
//	UVs.push_back(glm::vec2(0, 1));
//	UVs.push_back(glm::vec2(0, 0));
//	UVs.push_back(glm::vec2(1, 0));
//	UVs.push_back(glm::vec2(1, 1));
//
//	vn 0 1 0
//		vn - 1 0 0
//		vn 1 0 0
//		vn 0 0 - 1
//		vn 0 0 1
//		vn 0 - 1 0
//
//	normals.push_back(glm::vec3(0, 1, 0));
//	normals.push_back(glm::vec3(-1, 0, 0));
//	normals.push_back(glm::vec3(1, 0, 0));
//	normals.push_back(glm::vec3(0, 0, -1));
//	normals.push_back(glm::vec3(0, 0, 1));
//	normals.push_back(glm::vec3(0,-1, 0));
//
//
//
//	indexBuffer.push_back(0);
//	indexBuffer.push_back(1);
//	indexBuffer.push_back(2);
//	indexBuffer.push_back(2);
//	indexBuffer.push_back(1);
//	indexBuffer.push_back(3);
//
//	DefaultQuadMesh->createBuffers();
//	DefaultQuadMesh->m_usesIndexBuffer = true;
//	DefaultQuadMesh->m_renderingMode = GL_TRIANGLES;

	Model* testModel = new Model("Nanosuit", "C:/Users/Joel/Documents/LigumX/LigumX/data/models/cube/cube.obj");
	DefaultCubeMesh = testModel->m_meshes[0];

}


void DefaultMeshes::InitializeDefaultQuad()
{
	DefaultQuadMesh = new Mesh();

	// todo try morgan's cube obj in here
	std::vector<glm::vec3>& points = DefaultQuadMesh->m_buffers.vertexPositions;
	std::vector<glm::vec2>& UVs = DefaultQuadMesh->m_buffers.m_vertexUVs;
	std::vector<glm::vec3>& normals = DefaultQuadMesh->m_buffers.m_vertexNormals;
	std::vector<int>& indexBuffer = DefaultQuadMesh->m_buffers.indexBuffer;

	points.push_back(glm::vec3(0, 0, 0));
	points.push_back(glm::vec3(0, 1, 0));
	points.push_back(glm::vec3(1, 0, 0));
	points.push_back(glm::vec3(1, 1, 0));

	UVs.push_back(glm::vec2(0, 0));
	UVs.push_back(glm::vec2(0, 1));
	UVs.push_back(glm::vec2(1, 0));
	UVs.push_back(glm::vec2(1, 1));

	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));

	indexBuffer.push_back(0);
	indexBuffer.push_back(1);
	indexBuffer.push_back(2);
	indexBuffer.push_back(2);
	indexBuffer.push_back(1);
	indexBuffer.push_back(3);

	DefaultQuadMesh->createBuffers();
	DefaultQuadMesh->m_usesIndexBuffer = true;
	DefaultQuadMesh->m_renderingMode = GL_TRIANGLES;
}

void DefaultMeshes::InitializeDefaultSphere()
{
	DefaultSphereMesh = new Mesh();

}

DefaultMeshes::DefaultMeshes()
{
	InitializeDefaultQuad();
	InitializeDefaultSphere();
	InitializeDefaultCube();

}
