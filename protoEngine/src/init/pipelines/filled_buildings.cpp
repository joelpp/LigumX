#include "../../game.h"

void Game::init_pipelines_filledBuildings()
{
    const char* vertexShaderSource = " \
        #version 410 core\n \
        in /*layout(location=0)*/ vec2 pos;\n \
        uniform mat4 vpMat;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out vec2 texCoord;\n \
        void main() {\n \
            gl_Position = vpMat * vec4(pos, 0, 1);\n \
            texCoord = pos;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 410 core\n \
        in vec2 texCoord;\n \
        uniform /*layout(location=0)*/ sampler2D sampler;\n \
        out vec3 color;\n \
        void main() {\n \
            //color = vec3(1,1,1);\n \
            vec2 coord = mod(500*texCoord, 1);\n \
            //color = vec3(coord.x, coord.y,1);\n \
            color = texture(sampler, coord).xyz;\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);


    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);



    pPipelineBuildings = new ProgramPipeline();
    pPipelineBuildings->useVertexShader(pVertexShader);
    pPipelineBuildings->useFragmentShader(pFragmentShader);

    // VAO
    // glEnableVertexArrayAttrib(pPipelineBuildings->glidVao, 0);
    // glVertexArrayVertexBuffer(pPipelineBuildings->glidVao, 0, glidBufferBuildingTriangleVertices, 0, 3*4);
    // glVertexArrayAttribFormat(pPipelineBuildings->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glBindVertexArray(pPipelineBuildings->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidBufferBuildingTriangleVertices);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    
}
