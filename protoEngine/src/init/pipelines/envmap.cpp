#include "../../game.h"

void Game::init_pipelines_envmap()
{

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            "../src/shaders/pipeline_envmap/vertex.vsh", false);


    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            "../src/shaders/pipeline_envmap/fragment.fsh", false);



    pPipelineEnvmap = new ProgramPipeline();
    pPipelineEnvmap->useVertexShader(pVertexShader);
    pPipelineEnvmap->useFragmentShader(pFragmentShader);

    // VAO
    // nada.
}
