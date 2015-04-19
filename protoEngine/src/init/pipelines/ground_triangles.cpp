#include "../../game.h"

void Game::init_pipelines_groundTriangles()
{
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec3 pos;\n \
        \n \
        uniform mat4 vpMat;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        \n \
        void main() {\n \
            gl_Position = vpMat * vec4(pos, 1);\n \
            \n \
        }";

    const char* fragmentShaderSource = " \
        #version 430 core\n \
        in vec3 vColor;\n \
        out vec4 color;\n \
        void main() {\n \
            color = vec4(0.1,0.1,0.1,1.0);\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);



    pPipelineGround = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineGround->useVertexShader(pVertexShader);
    pPipelineGround->useFragmentShader(pFragmentShader);

    // VAO
    glEnableVertexArrayAttrib(pPipelineGround->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineGround->glidVao, 0, glidGroundTrianglePositions, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineGround->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
}
