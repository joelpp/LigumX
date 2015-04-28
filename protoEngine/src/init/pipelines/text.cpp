#include "../../game.h"

void Game::init_pipelines_text()
{
    PRINTSTRING("Creating text pipeline");

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            "../src/shaders/text/vertex.vsh", false);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            "../src/shaders/text/fragment.fsh", false);



    pPipelineText = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineText->useVertexShader(pVertexShader);
    pPipelineText->useFragmentShader(pFragmentShader);

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
    glGenBuffers(1, &textVBO);
    glBindVertexArray(pPipelineText->glidVao);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#endif


}
