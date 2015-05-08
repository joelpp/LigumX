#include "../../renderer.h"

void Renderer::init_pipelines_screenQuad()
{
    const char* vertexShaderSource = " \
        #version 410 core\n \
        layout(location=0) in vec2 pos;\n \
        layout(location=1) in vec2 texCoord;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out vec2 vTexCoord;\n \
        void main() {\n \
            gl_Position = vec4(pos, 0, 1);\n \
            vTexCoord = texCoord;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 410 core\n \
        in vec2 vTexCoord;\n \
        uniform sampler2D sampler;\n \
        out vec3 color;\n \
        void main() {\n \
            color = texture(sampler, vTexCoord).rgb;\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);



    pPipelineScreenQuad = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineScreenQuad->useVertexShader(pVertexShader);
    pPipelineScreenQuad->useFragmentShader(pFragmentShader);

    // VAO
#ifdef __APPLE__
    glBindVertexArray(pPipelineScreenQuad->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidScreenQuadPositions);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, glidScreenQuadTexCoords);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
#else
     glEnableVertexArrayAttrib(pPipelineScreenQuad->glidVao, 0);
     glVertexArrayVertexBuffer(pPipelineScreenQuad->glidVao, 0, glidScreenQuadPositions, 0, 2*4);
     glVertexArrayAttribFormat(pPipelineScreenQuad->glidVao, 0, 2, GL_FLOAT, GL_FALSE, 0);
     glEnableVertexArrayAttrib(pPipelineScreenQuad->glidVao, 1);
     glVertexArrayVertexBuffer(pPipelineScreenQuad->glidVao, 1, glidScreenQuadTexCoords, 0, 2*4);
     glVertexArrayAttribFormat(pPipelineScreenQuad->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
#endif


}
