#include "../../renderer.h"

void Renderer::init_pipelines_nodes()
{

#ifdef __APPLE__
        ProgramPipeline::ShaderProgram* pVertexShader =
                new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
                 "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/nodes/vertex.vsh", false);

        ProgramPipeline::ShaderProgram* pFragmentShader =
                new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
                "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/nodes/fragment.fsh", false);
                // fragmentShaderSource, true);
#else
        ProgramPipeline::ShaderProgram* pVertexShader =
                new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
                 "../src/shaders/pipeline_ground/vertex.vsh", false);

        ProgramPipeline::ShaderProgram* pFragmentShader =
                new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
                "../src/shaders/pipeline_ground/fragment.fsh", false);
#endif




    pPipelineNodes = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineNodes->useVertexShader(pVertexShader);
    pPipelineNodes->useFragmentShader(pFragmentShader);
    // VAO

#ifdef __APPLE__
    glBindVertexArray(pPipelineNodes->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidNodesPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, glidGroundTriangleUVs);
    // glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


#else
     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 0);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 0, glidGroundTrianglePositions, 0, 3*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);

     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 1);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 1, glidGroundTriangleUVs, 0, 2*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
#endif



}
