#include "renderer.h"

void Renderer::init_pipelines_generalLines()
{

    pPipelineLines = new ProgramPipeline("general_lines");


    // VAO
#ifdef __APPLE__
    glBindVertexArray(pPipelineLines->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidWaysPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(pPipelineLines->glidVao);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, glidWaysColors);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
#else
    glEnableVertexArrayAttrib(pPipelineLines->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineLines->glidVao, 0, glidWaysPositions, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineLines->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineLines->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineLines->glidVao, 1, glidWaysColors, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineLines->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);
#endif

//    


    // glBindAttribLocation (pPipelineLines->glidShaderProgram, 0, "pos");
    // glBindAttribLocation (pPipelineLines->glidShaderProgram, 1, "color");
}
