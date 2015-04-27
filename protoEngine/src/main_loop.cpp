#include "game.h"
#include <chrono>
#include "FreeImage.h"
#include <sstream>
#include <iomanip>

using namespace std;

void Game::mainLoop()
{
//    camera->moveFromUserInput(pWindow);
    camera->handlePresetNewFrame(pWindow);

    // move sun
    if(sunMoveAuto) {
        sunTime += sunSpeed/1000.0;
    }


    if(!fancyDisplayMode) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // draw ground
        if (drawGround){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureMap["grass"]->glidTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureMap["rock"]->glidTexture);

        pPipelineGround->usePipeline();
        glProgramUniformMatrix4fv(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
        glProgramUniform1i(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
        glProgramUniform1i(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler1"), 1);
        glDrawArrays(GL_TRIANGLES, 0, groundTrianglesPositions.size());
        }
        // draw lines
        pPipelineLines->usePipeline();
        for ( auto it = waysNodesPositionsMap.begin(); it != waysNodesPositionsMap.end(); ++it ){
            try{ if (!displayElementType.at(it->first)) continue; }
            catch(...){continue;};
            // glVertexArrayVertexBuffer(pPipelineLines->glidVao, 0, glidWaysNodesPositions[it->first], 0, 3*4);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glBindBuffer(GL_ARRAY_BUFFER, glidWaysNodesPositions[it->first]);
            glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            vec3 color = typeColorMap[it->first];
            glProgramUniform3f(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "color"), color.x, color.y, color.z);
            glProgramUniformMatrix4fv(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
            glDrawArrays(GL_LINES, 0, it->second.size());
        }

    } else {

        glBindFramebuffer(GL_FRAMEBUFFER, glidFramebuffer);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw sky/ground
        pPipelineEnvmap->usePipeline();
        GLuint fragProg = pPipelineEnvmap->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
        glProgramUniform2f(fragProg, glGetUniformLocation(fragProg, "windowSize"), windowWidth, windowHeight);
        glProgramUniform1f(fragProg, glGetUniformLocation(fragProg, "sunOrientation"), sunOrientation);
        glProgramUniform1f(fragProg, glGetUniformLocation(fragProg, "sunTime"), sunTime);
        glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewDir"), -camera->frontVec.x, -camera->frontVec.y, -camera->frontVec.z); // the frontVec for the camera points towards the eye, so we reverse it to get the view direction.
        glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewRight"), camera->rightVec.x, camera->rightVec.y, camera->rightVec.z);
        glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewUp"), camera->upVec.x, camera->upVec.y, camera->upVec.z);
        glProgramUniform2f(fragProg, glGetUniformLocation(fragProg, "viewAngles"), camera->totalViewAngleY*glm::pi<float>()/180.0, camera->aspectRatio*camera->totalViewAngleY*glm::pi<float>()/180.0);
        glProgramUniform1f(fragProg, glGetUniformLocation(fragProg, "viewNear"), camera->near);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glClear(GL_DEPTH_BUFFER_BIT);



        if (drawGround){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureMap["grass"]->glidTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureMap["rock"]->glidTexture);
            pPipelineGround->usePipeline();
            glProgramUniformMatrix4fv(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
            glProgramUniform1i(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
            glProgramUniform1i(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler1"), 1);
            glDrawArrays(GL_TRIANGLES, 0, nbGroundVertices);
        }



        glActiveTexture(GL_TEXTURE0);

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
               // glBindTexture(GL_TEXTURE_2D, pBuildingTex->glidTexture);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pPipelineScreenQuad->usePipeline();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glClear(GL_DEPTH_BUFFER_BIT);



    //        // draw buildings
            if(!drawBuildingSides) {
                glBindTexture(GL_TEXTURE_2D, textureMap["bricks"]->glidTexture);
                pPipelineBuildings->usePipeline();
                glProgramUniformMatrix4fv(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
    //            glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
                glDrawArrays(GL_TRIANGLES, 0, nbBuildingTriangles);
            } else {
                glActiveTexture(GL_TEXTURE0);

                glBindTexture(GL_TEXTURE_2D, textureMap["bricks"]->glidTexture);

                pPipelineBuildingSides->usePipeline();

                glProgramUniformMatrix4fv(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
                glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uBuildingHeight"), buildingHeight);
                glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uScaleFactor"), buildingSideScaleFactor);

                glDrawArrays(GL_LINES, 0, nbBuildingLines);
            }

    }

    if(showTweakBar) TwDraw();

    // screenshot
    if(saveScreenshot) {
        static unsigned int frameCount = 0;
        ++frameCount;
        std::stringstream filename;
        filename.clear();
        filename << "../output/frame" << setfill('0') << setw(5) << frameCount << ".png";

        BYTE* pixels = new BYTE[4 * windowWidth * windowHeight];
        glReadPixels(0, 0, windowWidth, windowHeight, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
        FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, 4 * windowWidth, 32, 0x0000FF, 0xFF0000, 0x00FF00, false);
        if(FreeImage_Save(FIF_BMP, image, filename.str().c_str(), 0)) {
            std::cout << "screenshot '" << filename.str() << "' taken." << std::endl;
        } else {
            std::cout << "screenshot failed. Did you create the 'protoEngine/output' directory?" << std::endl;
        }
        FreeImage_Unload(image);
        delete [] pixels;
    }



}
