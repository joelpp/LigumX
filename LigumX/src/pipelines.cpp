#include "stdafx.h"

#include "renderer.h"
#include "Material.h"

void Renderer::init_pipelines()
{

	// todo : instead parse through the shaders folder and init automatically families

//	std::cout << "GL Version: " <<glGetString(GL_VERSION) << "\n";
//	std::cout << "GLSL Version: "<< glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
//	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
//    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    init_pipelines_text();
	Renderer::outputGLError(__func__, __LINE__);


    pPipelineBasic = new ProgramPipeline("Basic");
    pPipelineBasicUV = new ProgramPipeline("BasicUV");
	pPipelineShadowMap = new ProgramPipeline("ShadowMap");
	pPipelineLines = new ProgramPipeline("general_lines");
    pPipelineGround = new ProgramPipeline("Terrain");
	pPipelineEnvmap = new ProgramPipeline("Envmap");
	pPipelineScreenSpaceTexture = new ProgramPipeline("ScreenSpaceTexture");
	pPipelineNodes = new ProgramPipeline("nodes");
	pPipelinePicking = new ProgramPipeline("picking");
	pPipelinePickingCompute = new ProgramPipeline("picking_compute", true);
	pPipelineUVEdges = new ProgramPipeline("UVEdges");
	pPipelineSolidColor = new ProgramPipeline("SolidColor");
	pPipelineBlur = new ProgramPipeline("blur");
	pPipelineGrid = new ProgramPipeline("Grid");
	pPipelineAxisGizmo = new ProgramPipeline("AxisGizmo");
	Renderer::outputGLError(__func__, __LINE__);

	m_Pipelines.clear();
	for (int i = 0; i < EnumLength_ShaderFamily; ++i)
	{
		m_Pipelines.push_back(new ProgramPipeline(EnumValues_ShaderFamily[i]));
	}

	PRINTSTRING("Successfully built all shaders!");

}

