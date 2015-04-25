#include "../../game.h"

void Game::init_pipelines_groundTriangles()
{

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
             "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/pipeline_ground/vertex.vsh", false);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/pipeline_ground/fragment.fsh", false);



    pPipelineGround = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineGround->useVertexShader(pVertexShader);
    pPipelineGround->useFragmentShader(pFragmentShader);

    // VAO
    // glEnableVertexArrayAttrib(pPipelineGround->glidVao, 0);
    // glVertexArrayVertexBuffer(pPipelineGround->glidVao, 0, glidGroundTrianglePositions, 0, 3*4);
    // glVertexArrayAttribFormat(pPipelineGround->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    // glEnableVertexArrayAttrib(pPipelineGround->glidVao, 1);
    // glVertexArrayVertexBuffer(pPipelineGround->glidVao, 1, glidGroundTriangleUVs, 0, 2*4);
    // glVertexArrayAttribFormat(pPipelineGround->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);

    glBindVertexArray(pPipelineGround->glidVao);
    glBindBuffer(GL_ARRAY_BUFFER, glidGroundTrianglePositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(pPipelineGround->glidVao);
    glBindBuffer(GL_ARRAY_BUFFER, glidGroundTriangleUVs);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
}
