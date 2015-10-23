#include "renderer.h"

void Renderer::init_pipelines_nodes()
{

    pPipelineNodes = new ProgramPipeline("nodes");

    // VAO

#ifdef __APPLE__
    glBindVertexArray(pPipelineNodes->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidNodesPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

#else
     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 0);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 0, glidGroundTrianglePositions, 0, 3*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);

     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 1);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 1, glidGroundTriangleUVs, 0, 2*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
#endif



}
