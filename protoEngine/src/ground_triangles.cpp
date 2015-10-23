#include "renderer.h"

void Renderer::init_pipelines_groundTriangles()
{
    pPipelineGround = new ProgramPipeline("Basic");

    // VAO

#ifdef __APPLE__
    glBindVertexArray(pPipelineGround->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidGroundTrianglePositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, glidGroundTriangleUVs);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


#else
     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 0);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 0, glidGroundTrianglePositions, 0, 3*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);

     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 1);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 1, glidGroundTriangleUVs, 0, 2*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
#endif



}
