#include "game.h"

using std::cout;
using std::endl;

void Game::mainLoop()
{
//    camera->moveFromUserInput(pWindow);
    camera->handlePresetNewFrame(pWindow);

    if(!fancyDisplayMode) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // draw ground
        pPipelineGround->usePipeline();
        glProgramUniformMatrix4fv(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
        glDrawArrays(GL_TRIANGLES, 0, nbGroundVertices);

        // draw lines
        pPipelineLines->usePipeline();
        glProgramUniformMatrix4fv(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
        glDrawArrays(GL_LINES, 0, nbWaysVertices);

//        glDrawArrays(GL_POINTS, 0, nbWaysVertices);
    } else {

        glBindFramebuffer(GL_FRAMEBUFFER, glidFramebuffer);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw sky/ground
        pPipelineEnvmap->usePipeline();
        GLuint fragProg = pPipelineEnvmap->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
        glProgramUniform2f(fragProg, glGetUniformLocation(fragProg, "windowSize"), windowWidth, windowHeight);
        glProgramUniform3fv(fragProg, glGetUniformLocation(fragProg, "sunDir"), 1, value_ptr(sunDirection));
        glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewDir"), -camera->frontVec.x, -camera->frontVec.y, -camera->frontVec.z); // the frontVec for the camera points towards the eye, so we reverse it to get the view direction.
        glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewRight"), camera->rightVec.x, camera->rightVec.y, camera->rightVec.z);
        glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewUp"), camera->upVec.x, camera->upVec.y, camera->upVec.z);
        glProgramUniform2f(fragProg, glGetUniformLocation(fragProg, "viewAngles"), camera->totalViewAngleY*glm::pi<float>()/180.0, camera->aspectRatio*camera->totalViewAngleY*glm::pi<float>()/180.0);
        glProgramUniform1f(fragProg, glGetUniformLocation(fragProg, "viewNear"), camera->near);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glClear(GL_DEPTH_BUFFER_BIT);

        // draw roads
        glBindTexture(GL_TEXTURE_2D, glidTextureScreenRoads);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "qweqwe" << endl;
        pPipelineRoads->usePipeline();
        pPipelineRoads->useGeometryShader(pGeometryShader1);
        pPipelineRoads->useFragmentShader(pFragmentShader1);
        glProgramUniformMatrix4fv(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
        glMultiDrawArrays(GL_LINE_STRIP_ADJACENCY, firstVertexForEachRoad.data(), nbVerticesForEachRoad.data(), nbRoads);
        // draw circles on road nodes
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "qweqwe" << endl;
        pPipelineRoads->usePipeline();
        pPipelineRoads->useGeometryShader(pGeometryShader2);
        pPipelineRoads->useFragmentShader(pFragmentShader2);
        glProgramUniformMatrix4fv(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
        glMultiDrawArrays(GL_LINE_STRIP_ADJACENCY, firstVertexForEachRoad.data(), nbVerticesForEachRoad.data(), nbRoads);
        // draw road lines
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "qweqwe" << endl;
        pPipelineRoads->usePipeline();
        pPipelineRoads->useGeometryShader(pGeometryShader3);
        pPipelineRoads->useFragmentShader(pFragmentShader3);
        glProgramUniformMatrix4fv(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
        glMultiDrawArrays(GL_LINE_STRIP_ADJACENCY, firstVertexForEachRoad.data(), nbVerticesForEachRoad.data(), nbRoads);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//                glBindTexture(GL_TEXTURE_2D, pBuildingTex->glidTexture);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pPipelineScreenQuad->usePipeline();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glClear(GL_DEPTH_BUFFER_BIT);

//        // draw buildings
        if(!drawBuildingSides) {
            glBindTexture(GL_TEXTURE_2D, pBuildingTex->glidTexture);
            pPipelineBuildings->usePipeline();
            glProgramUniformMatrix4fv(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
//            glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
            glDrawArrays(GL_TRIANGLES, 0, nbBuildingTriangles);
        } else {
            glBindTexture(GL_TEXTURE_2D, pBuildingTex->glidTexture);

            pPipelineBuildingSides->usePipeline();

            glProgramUniformMatrix4fv(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
            glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uBuildingHeight"), buildingHeight);
            glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uScaleFactor"), buildingSideScaleFactor);

            glDrawArrays(GL_LINES, 0, nbBuildingLines);
        }
    }

    if(showTweakBar) TwDraw();

}
