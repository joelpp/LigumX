#include "game.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include "FreeImage.h"

using namespace glm;
using std::cout;
using std::endl;

const double static_dt = 1.0 / 100.0;

void Game::mainLoop()
{
    static double dt = 0.0;
    static double curr_time = glfwGetTime();
    static double physic_accumulator = 0.0;



    physic_accumulator += dt;

//    camera->moveFromUserInput(pWindow);
    camera->handlePresetNewFrame(pWindow);

    // Physic update step
    while ( physic_accumulator >= static_dt ) {
        entityManager.Update(static_dt);
        physic_accumulator -= static_dt;
    }

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
        glBindTexture(GL_TEXTURE_2D, textureMap["rock"]->glidTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureMap["grass"]->glidTexture);


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
#ifdef __APPLE__
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindBuffer(GL_ARRAY_BUFFER, glidWaysNodesPositions[it->first]);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
#else
        glVertexArrayVertexBuffer(pPipelineLines->glidVao, 0, glidWaysNodesPositions[it->first], 0, 3*4);
#endif
            vec3 color = typeColorMap[it->first];
            glProgramUniform3f(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "color"), color.x, color.y, color.z);
            glProgramUniformMatrix4fv(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
            glDrawArrays(GL_LINES, 0, it->second.size());
        }

//        pPipelineLines->usePipeline();
//        glProgramUniformMatrix4fv(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
//        glDrawArrays(GL_LINES, 0, nbWaysVertices);

        // draw entities
#ifndef __APPLE__
        entityManager.Render(camera->mvpMat);
#endif
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



       // draw buildings
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureMap["rock"]->glidTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureMap["grass"]->glidTexture);

        pPipelineBuildings->usePipeline();
        glProgramUniformMatrix4fv(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
//            glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
        glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
        glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler1"), 1);
        
        glDrawArrays(GL_TRIANGLES, 0, nbBuildingTriangles);

        // draw building sides
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, textureMap["bricks"]->glidTexture);

        pPipelineBuildingSides->usePipeline();

        glProgramUniformMatrix4fv(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
        glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uBuildingHeight"), buildingHeight);
        glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uScaleFactor"), buildingSideScaleFactor);

        glDrawArrays(GL_LINES, 0, nbBuildingLines);
    

    }
    std::stringstream fpsString;
    fpsString << round(1.f/dt);
    fpsString << " fps";
    RenderText(fpsString.str(), 750.0f, 750.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f), false);
//    RenderText("LIGUMX BITCHES", -0.7f, 0.5f, 0.0001f, glm::vec3(0.5, 0.8f, 0.2f), true);

//    int filter = OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_UNCLASSIFIED;
//    for (auto it = theWays.begin(); it != theWays.end(); ++it){
//        Way* way = it->second;

//        if ((way->eType & filter) == 0) continue;
//        std::string name;
//        try{name = way->tags.at("name");}
//        catch(...){continue;}

//        vec2 xy = (way->nodes[0]->getLatLong() + way->nodes[1]->getLatLong()) / vec2(2,2);

//        RenderText(name.c_str(), xy.x, xy.y, 0.000001f, glm::vec3(0.5, 0.8f, 0.2f), true);

//    }

    if (renderText)
        for (int i = 0; i < texts.size(); i++){
//            RenderText(texts[i].text.c_str(), texts[i].position.x, texts[i].position.y, 0.000001f, glm::vec3(0.5, 0.8f, 0.2f), texts[i].projected);
            RenderText(texts[i]);
        }
    if(showTweakBar) TwDraw();

    // screenshot
    if(saveScreenshot) {
        static unsigned int frameCount = 0;
        ++frameCount;
        std::stringstream filename;
        filename.clear();
        filename << "../output/frame" << std::setfill('0') << std::setw(5) << frameCount << ".png";

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
    double new_time = glfwGetTime();
    dt = new_time - curr_time;
    curr_time = new_time;


}

void Game::RenderText(Text t){
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Activate corresponding render state
    pPipelineText->usePipeline();
    GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
    glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
    glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
    if (t.projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(camera->mvpMat));
    else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f)));

    std::string text = t.text;
    std::string::const_iterator c;
    GLfloat uvs[6][2] = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0},

        {0.0, 0.0},
        {1.0, 1.0},
        {1.0, 0.0}

    };
    int index = 0;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(t.quads[index].vertices), t.quads[index].vertices);

        glBindBuffer(GL_ARRAY_BUFFER, textUvsVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uvs), uvs);

//        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        index++;
    }
//    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

void Game::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, bool projected)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Activate corresponding render state
    pPipelineText->usePipeline();
    GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
    glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
    glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
    if (projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(camera->mvpMat));
    else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f)));

    glActiveTexture(GL_TEXTURE0);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][3] = {
            { xpos,     ypos + h,  0.0001 },
            { xpos,     ypos,      0.0001 },
            { xpos + w, ypos,      0.0001 },

            { xpos,     ypos + h,  0.0001 },
            { xpos + w, ypos,      0.0001 },
            { xpos + w, ypos + h,  0.0001 }
        };
        GLfloat uvs[6][2] = {
            {0.0, 0.0},
            {0.0, 1.0},
            {1.0, 1.0},

            {0.0, 0.0},
            {1.0, 1.0},
            {1.0, 0.0}

        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, textUvsVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uvs), uvs);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

}
