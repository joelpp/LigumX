#include "../../game.h"

void Game::init_pipelines_groundTriangles()
{
    PRINTSTRING("Creating ground triangles pipeline");
    const char* vertexShaderSource = " \
        #version 410 core\n \
        /*layout(location=0)*/ in vec3 pos;\n \
        /*layout(location=1)*/ in vec2 texCoord;\n \
        //in int gl_VertexID;\n \
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
        #version 410 core\n \
        out vec4 color;\n \
        void main() {\n \
            color = vec4(1.0,0.1,0.1,1.0);\n \
        }";
#ifdef __APPLE__
        ProgramPipeline::ShaderProgram* pVertexShader =
                new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
                 "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/pipeline_ground/vertex.vsh", false);

        ProgramPipeline::ShaderProgram* pFragmentShader =
                new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
                "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/pipeline_ground/fragment.fsh", false);
                // fragmentShaderSource, true);
#else
        ProgramPipeline::ShaderProgram* pVertexShader =
                new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
                 "../src/shaders/pipeline_ground/vertex.vsh", false);

        ProgramPipeline::ShaderProgram* pFragmentShader =
                new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
                "../src/shaders/pipeline_ground/fragment.fsh", false);
#endif




    pPipelineGround = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineGround->useVertexShader(pVertexShader);
    pPipelineGround->useFragmentShader(pFragmentShader);
    // VAO

#ifdef __APPLE__
    glBindVertexArray(pPipelineGround->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidGroundTrianglePositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, glidGroundTriangleUVs);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


#else
     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 0);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 0, glidGroundTrianglePositions, 0, 3*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);

     glEnableVertexArrayAttrib(pPipelineGround->glidVao, 1);
     glVertexArrayVertexBuffer(pPipelineGround->glidVao, 1, glidGroundTriangleUVs, 0, 2*4);
     glVertexArrayAttribFormat(pPipelineGround->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
#endif



}
