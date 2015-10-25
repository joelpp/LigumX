#include "Mesh.h"
#include "Renderer.h"
using namespace std;

Mesh::Mesh()
{
	
    glGenVertexArrays(1, &m_VAO);
	  glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidNormals);
    // glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidUVs);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    m_wireframeRendering = false;
    m_usesIndexBuffer = false;
}

// Mesh::Mesh(vector<Vertex> vertices)
// {
// 	m_vertexArray = vertices;
// 	createBuffers();
// }
// 
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
    Renderer::GetInstance().createGLBuffer(GL_ARRAY_BUFFER, m_VBOs.glidUVs, 	   m_buffers.m_vertexUVs);
	
    glGenVertexArrays(1, &m_VAO);
	  glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidNormals);
    // glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs.glidUVs);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &m_VBOs.glidIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs.glidIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_buffers.indexBuffer.size() * sizeof(int), m_buffers.indexBuffer.data(), GL_DYNAMIC_DRAW); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLenum err;
      while ((err = glGetError()) != GL_NO_ERROR) 
      {
        string error;

        switch(err) 
        {
          case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
          case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
          case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
          case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
          case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }

        PRINT(error.c_str());
        err=glGetError();    
      }
 
}