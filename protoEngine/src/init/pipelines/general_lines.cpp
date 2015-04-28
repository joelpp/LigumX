#include "../../game.h"

void Game::init_pipelines_generalLines()
{
    PRINTSTRING("Creating general lines pipeline");

    const char* vertexShaderSource = " \
        #version 410 core\n \
        layout(location=0) in vec3 pos;\n \
        //layout(location=1) in vec3 color;\n \
        uniform vec3 color;\n \
        uniform mat4 vpMat;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out vec3 vColor;\n \
        void main() {\n \
            gl_Position = vpMat * vec4(pos, 1);\n \
            vColor = color;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 410 core\n \
        in vec3 vColor;\n \
        out vec3 color;\n \
        void main() {\n \
            color = vColor;\n \
        }";
#ifdef __APPLE__

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/general_lines/vertex.vsh", false);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/general_lines/fragment.fsh", false);
#else
    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);
#endif


    pPipelineLines = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineLines->useVertexShader(pVertexShader);
    pPipelineLines->useFragmentShader(pFragmentShader);

    // VAO
#ifdef __APPLE__
    glBindVertexArray(pPipelineLines->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidWaysPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(pPipelineLines->glidVao);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, glidWaysColors);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
#else
    glEnableVertexArrayAttrib(pPipelineLines->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineLines->glidVao, 0, glidWaysPositions, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineLines->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineLines->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineLines->glidVao, 1, glidWaysColors, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineLines->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);
#endif

//    


    // glBindAttribLocation (pPipelineLines->glidShaderProgram, 0, "pos");
    // glBindAttribLocation (pPipelineLines->glidShaderProgram, 1, "color");
}
