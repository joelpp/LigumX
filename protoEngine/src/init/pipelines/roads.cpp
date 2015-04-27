#include "../../game.h"

void Game::init_pipelines_roads()
{
    PRINTSTRING("Creating roads pipeline");
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);

    const char* vertexShaderSource = " \
        #version 410 core\n \
        layout(location=0) in vec3 pos;\n \
        layout(location=1) in float distance;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
            out float vDistance;\n \
        void main() {\n \
            gl_Position = vec4(pos, 1);\n \
                vDistance = distance;\n \
        }";

    const char* fragmentShaderSource1 = " \
        #version 410 core\n \
    //        in vec2 gTexCoord;\n \
        /*layout(location=0)*/ out vec3 fColor;\n \
        void main() {\n \
            float lineWidth = 0.15;\n \
            fColor = vec3(0.5, 0.5, 0.5);\n \
    ////            fColor = vec3(1,1,1);\n \
    ////            fColor = vec3(gTexCoord.x,gTexCoord.y,1);\n \
    //            if(abs(gTexCoord.x-0.5) < lineWidth/2.0) {\n \
    //                fColor = vec3(1,1,0);\n \
    //            } else {\n \
    //                fColor = vec3(0.3,0.3,0.3);\n \
    //            }\n \
        }";

    const char* fragmentShaderSource2 = " \
        #version 410 core\n \
    //        in vec2 gTexCoord;\n \
        //in vec2 gl_PointCoord;\n \
        /*layout(location=0)*/ out vec3 fColor;\n \
        void main() {\n \
            if(distance(gl_PointCoord, vec2(0.5,0.5)) < 0.5) {\n \
                fColor = vec3(0.5, 0.5, 0.5);\n \
            } else {\n \
                discard;\n \
            }\n \
        }";

    const char* fragmentShaderSource3 = " \
        #version 410 core\n \
        /*layout(location=0)*/ out vec3 fColor;\n \
        void main() {\n \
            fColor = vec3(1,1,0);\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    pGeometryShader1 =
            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/roads.gsh", false);

    pGeometryShader2 =
            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/roads2.gsh", false);

    pGeometryShader3 =
            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
            "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/src/shaders/roads3.gsh", false);

    pFragmentShader1 =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource1, true);

    pFragmentShader2 =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource2, true);

    pFragmentShader3 =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource3, true);



    pPipelineRoads = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineRoads->useVertexShader(pVertexShader);
    pPipelineRoads->useGeometryShader(pGeometryShader1);
    pPipelineRoads->useFragmentShader(pFragmentShader1);

    // VAO
    // glEnableVertexArrayAttrib(pPipelineRoads->glidVao, 0);
    // glVertexArrayVertexBuffer(pPipelineRoads->glidVao, 0, glidBufferRoadsPositions, 0, 3*4);
    // glVertexArrayAttribFormat(pPipelineRoads->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    //    glEnableVertexArrayAttrib(pPipelineRoads->glidVao, 1);
    //    glVertexArrayVertexBuffer(pPipelineRoads->glidVao, 1, glidBufferRoadsPositions, 0, 1*4);
    //    glVertexArrayAttribFormat(pPipelineRoads->glidVao, 1, 1, GL_FLOAT, GL_FALSE, 0);
    glBindVertexArray(pPipelineRoads->glidVao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, glidBufferRoadsPositions);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
