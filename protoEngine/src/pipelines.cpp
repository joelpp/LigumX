#include "renderer.h"

void Renderer::init_pipelines()
{
//	std::cout << "GL Version: " <<glGetString(GL_VERSION) << "\n";
//	std::cout << "GLSL Version: "<< glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
//	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
//    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    init_pipelines_text();
    init_pipelines_buildingSides();
    init_pipelines_filledBuildings();
    init_pipelines_roads();
    init_pipelines_screenQuad();



    pPipelineBasic = new ProgramPipeline("Basic");
    pPipelineBasicUV = new ProgramPipeline("BasicUV");
    pPipelineLines = new ProgramPipeline("general_lines");
    pPipelineGround = new ProgramPipeline("Terrain");
    pPipelineEnvmap = new ProgramPipeline("Envmap");
    pPipelineNodes = new ProgramPipeline("nodes");

}

