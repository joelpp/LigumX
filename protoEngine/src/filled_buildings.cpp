#include "renderer.h"

void Renderer::init_pipelines_filledBuildings()
{

    pPipelineBuildings = new ProgramPipeline("filledBuildings");

    // VAO
#ifdef __APPLE__
    glBindVertexArray(pPipelineBuildings->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidBufferBuildingTriangleVertices);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, glidGroundTriangleTextureIDs);
    glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
#else
     glEnableVertexArrayAttrib(pPipelineBuildings->glidVao, 0);
     glVertexArrayVertexBuffer(pPipelineBuildings->glidVao, 0, glidBufferBuildingTriangleVertices, 0, 3*4);
     glVertexArrayAttribFormat(pPipelineBuildings->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
     glEnableVertexArrayAttrib(pPipelineBuildings->glidVao, 1);
     glVertexArrayVertexBuffer(pPipelineBuildings->glidVao, 1, glidGroundTriangleTextureIDs, 0, 1*4);
     glVertexArrayAttribFormat(pPipelineBuildings->glidVao, 1, 1, GL_FLOAT, GL_FALSE, 0);
#endif



}
