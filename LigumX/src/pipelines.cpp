#include "stdafx.h"

#include "renderer.h"

void Renderer::init_pipelines()
{
//	std::cout << "GL Version: " <<glGetString(GL_VERSION) << "\n";
//	std::cout << "GLSL Version: "<< glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
//	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
//    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    init_pipelines_text();
    //init_pipelines_buildingSides();
	Renderer::outputGLError(__func__, __LINE__);
	//init_pipelines_filledBuildings();
	Renderer::outputGLError(__func__, __LINE__);
	//init_pipelines_roads();
	Renderer::outputGLError(__func__, __LINE__);
	init_pipelines_screenQuad();

	Renderer::outputGLError(__func__, __LINE__);


    pPipelineBasic = new ProgramPipeline("Basic");
    pPipelineBasicUV = new ProgramPipeline("BasicUV");
	pPipelineShadowMap = new ProgramPipeline("ShadowMap");
	pPipelineLines = new ProgramPipeline("general_lines");
    pPipelineGround = new ProgramPipeline("Terrain");
	pPipelineEnvmap = new ProgramPipeline("Envmap");
	pPipelineScreenSpaceTexture = new ProgramPipeline("ScreenSpaceTexture");
    pPipelineNodes = new ProgramPipeline("nodes");
	Renderer::outputGLError(__func__, __LINE__);

}

