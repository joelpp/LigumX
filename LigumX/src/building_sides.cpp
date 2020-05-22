#include "renderer.h"

void Renderer::init_pipelines_buildingSides()
{
    const char* vertexShaderSource = " \
        #version 410 core\n \
        layout(location=0) in vec3 pos;\n \
        layout(location=1) in float distance;\n \
        uniform mat4 vpMat;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out float vDistance;\n \
        void main() {\n \
            gl_Position = vec4(pos.xyz, 1);\n \
            vDistance = distance;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 410 core\n \
        in vec2 gTexCoord;\n \
        uniform sampler2D sampler;\n \
        out vec3 color;\n \
        void main() {\n \
    //            vec2 coord = mod(500*texCoord, 1);\n \
            //color = vec3(coord.x, coord.y,1);\n \
            color = texture(sampler, gTexCoord).xyz;\n \
    //            color = vec3(gTexCoord.x, gTexCoord.y, 0.5);\n \
        }";
    ProgramPipeline::ShaderProgram* pVertexShader = new ProgramPipeline::ShaderProgram();
	pVertexShader->Initialize(GL_VERTEX_SHADER, "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/shaders/pipeline_buildingSides/vertex.vsh", false);
    ProgramPipeline::ShaderProgram* pGeometryShader = new ProgramPipeline::ShaderProgram();
	pGeometryShader->Initialize(GL_GEOMETRY_SHADER, "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/shaders/buildingSides.gsh", false);
	ProgramPipeline::ShaderProgram* pFragmentShader = new ProgramPipeline::ShaderProgram(); 
	pFragmentShader ->Initialize(GL_FRAGMENT_SHADER, "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/shaders/pipeline_buildingSides/fragment.fsh", false);
//#ifdef __APPLE__
//
//    ProgramPipeline::ShaderProgram* pVertexShader =
//            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
//            vertexShaderSource, true);
//
//    ProgramPipeline::ShaderProgram* pGeometryShader =
//            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
//            "../data/shaders/buildingSides.gsh", false);
//
//    ProgramPipeline::ShaderProgram* pFragmentShader =
//            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
//            fragmentShaderSource, true);
//#endif


    pPipelineBuildingSides = new ProgramPipeline();
    pPipelineBuildingSides->useVertexShader(pVertexShader);
    pPipelineBuildingSides->useGeometryShader(pGeometryShader);
    pPipelineBuildingSides->useFragmentShader(pFragmentShader);

    // // VAO

    glBindVertexArray(pPipelineBuildingSides->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidBufferBuildingLines);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, glidBufferBuildingLoopLengths);
    glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
//#ifdef __APPLE__
//
//    glEnableVertexArrayAttrib(pPipelineBuildingSides->glidVao, 0);
//    glVertexArrayVertexBuffer(pPipelineBuildingSides->glidVao, 0, glidBufferBuildingLines, 0, 3*4);
//    glVertexArrayAttribFormat(pPipelineBuildingSides->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
//    glEnableVertexArrayAttrib(pPipelineBuildingSides->glidVao, 1);
//    glVertexArrayVertexBuffer(pPipelineBuildingSides->glidVao, 1, glidBufferBuildingLoopLengths, 0, 1*4);
//    glVertexArrayAttribFormat(pPipelineBuildingSides->glidVao, 1, 1, GL_FLOAT, GL_FALSE, 0);
//#endif

}
