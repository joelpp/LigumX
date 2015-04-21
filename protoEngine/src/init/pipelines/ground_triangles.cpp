#include "../../game.h"

void Game::init_pipelines_groundTriangles()
{
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec3 pos;\n \
        in layout(location=1) vec2 texCoord;\n \
        in int gl_VertexID;\n \
        \n \
        uniform mat4 vpMat;\n \
        out vec2 myTexCoord;\n \
        out float height;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        \n \
        void main() {\n \
            gl_Position = vpMat * vec4(pos, 1);\n \
            myTexCoord = texCoord;\n \
            height = pos.z;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 430 core\n \
        in vec3 vColor;\n \
        in vec2 myTexCoord;\n \
        in float height;\n \
        out vec4 color;\n \
        uniform layout(location=0) sampler2D sampler;\n\
        uniform layout(location=1) sampler2D sampler1;\n\
        void main() {\n \
//            color = vec4(1.0,0.1,0.1,1.0);\n \
//            color = vec4(myTexCoord ,0.0,1.0);\n \
           if (height < 0.01) color = vec4(texture(sampler, myTexCoord).xyz,1.0);\n \
           else color = vec4(texture(sampler1, myTexCoord).xyz,1.0);\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
             "../src/shaders/pipeline_ground/vertex.vsh", false);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            "../src/shaders/pipeline_ground/fragment.fsh", false);



    pPipelineGround = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineGround->useVertexShader(pVertexShader);
    pPipelineGround->useFragmentShader(pFragmentShader);

    // VAO
    glEnableVertexArrayAttrib(pPipelineGround->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineGround->glidVao, 0, glidGroundTrianglePositions, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineGround->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(pPipelineGround->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineGround->glidVao, 1, glidGroundTriangleUVs, 0, 2*4);
    glVertexArrayAttribFormat(pPipelineGround->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
}
