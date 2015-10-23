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
}

// Mesh::Mesh(vector<Vertex> vertices)
// {
// 	m_vertexArray = vertices;
// 	createBuffers();
// }

void Mesh::createBuffers()
{

	PRINTVEC3VECTOR(m_buffers.m_vertexPositions);
	// TODO: I'm not quite convinced this belongs here. or does it?
    
    Renderer::GetInstance().createGLBuffer(m_VBOs.glidPositions, m_buffers.m_vertexPositions);
    // Renderer::createGLBuffer(m_VBOs.glidNormals,   m_buffers.m_vertexNormals);
    
    Renderer::GetInstance().createGLBuffer(m_VBOs.glidUVs, 	   m_buffers.m_vertexUVs);
	
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