#include "../../renderer.h"

void Renderer::init_pipelines_envmap()
{
#ifdef __APPLE__

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/pipeline_envmap/vertex.vsh", false);


    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/pipeline_envmap/fragment.fsh", false);
#else
    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            "../src/shaders/pipeline_envmap/vertex.vsh", false);


    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            "../src/shaders/pipeline_envmap/fragment.fsh", false);
#endif


    pPipelineEnvmap = new ProgramPipeline();
    pPipelineEnvmap->useVertexShader(pVertexShader);
    pPipelineEnvmap->useFragmentShader(pFragmentShader);

    // VAO
    // nada.
}
