#include "../../game.h"

void Game::init_pipelines_buildingSides()
{
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec3 pos;\n \
        in layout(location=1) float distance;\n \
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
        #version 430 core\n \
        in vec2 gTexCoord;\n \
        uniform layout(location=0) sampler2D sampler;\n \
        out vec3 color;\n \
        void main() {\n \
    //            vec2 coord = mod(500*texCoord, 1);\n \
            //color = vec3(coord.x, coord.y,1);\n \
            color = texture(sampler, gTexCoord).xyz;\n \
    //            color = vec3(gTexCoord.x, gTexCoord.y, 0.5);\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    ProgramPipeline::ShaderProgram* pGeometryShader =
            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
            "../src/shaders/buildingSides.gsh", false);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);



    pPipelineBuildingSides = new ProgramPipeline();
    pPipelineBuildingSides->useVertexShader(pVertexShader);
    pPipelineBuildingSides->useGeometryShader(pGeometryShader);
    pPipelineBuildingSides->useFragmentShader(pFragmentShader);

    // VAO
    glEnableVertexArrayAttrib(pPipelineBuildingSides->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineBuildingSides->glidVao, 0, glidBufferBuildingLines, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineBuildingSides->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineBuildingSides->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineBuildingSides->glidVao, 1, glidBufferBuildingLoopLengths, 0, 1*4);
    glVertexArrayAttribFormat(pPipelineBuildingSides->glidVao, 1, 1, GL_FLOAT, GL_FALSE, 0);
}
