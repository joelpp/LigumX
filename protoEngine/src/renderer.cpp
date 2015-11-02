#include "renderer.h"
#include "program_pipeline.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"

using namespace glm;
using namespace std;

#define FLUSH_ERRORS() outputGLError(__func__, __LINE__);



void Renderer::Initialize(){

    fancyDisplayMode = false;
    showText = false;
    windowWidth = 800;
    windowHeight = 800;
    windowTitle = "LigumX";
    fps = 300;
    dt = 0.1;
    //=============================================================================
    // create window and GLcontext, register callbacks.
    //=============================================================================

    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

//    glfwSetErrorCallback(test_error_cb);


    // Initialise GLFW
    if( !glfwInit() )
    {
        PRINT("Failed to initialize GLFW.\n");
        return;
    } else {
        PRINT("Initialized GLFW.");
    }


    // set window paramaters
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif

    // create GLFW window
//    pWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
    pWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), 0, NULL);

    glfwSetWindowPos(pWindow, 700, 200);
    glfwMakeContextCurrent(pWindow);
    if( pWindow == NULL )
    {
        cerr << "Failed to open GLFW window.\n";
        glfwTerminate();
        return;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "Failed to initialize GLEW\n"
             << glewGetErrorString(err) << endl;
        return;
    } else {
        PRINT("Initialized GLEW.");
    }
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    std::string texturePath = "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/";
#ifdef __APPLE__
    textureMap.emplace("bricks", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/brickles.png"));
    textureMap.emplace("grass", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/grass.png"));
    textureMap.emplace("rock", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/rock.png"));
    textureMap.emplace("ATLAS", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/Atlas.png"));
    textureMap.emplace("asphalt", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/asphalt.jpg"));
    textureMap.emplace("roof", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/roof_rgba.png"));
    textureMap.emplace("building_side1", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/textures/building_side1_rgba.png"));

#else
    textureMap.emplace("bricks", new Texture("../textures/brickles.png"));
    textureMap.emplace("grass", new Texture("../textures/grass.png"));
    textureMap.emplace("rock", new Texture("../textures/rock.png"));
    textureMap.emplace("ATLAS", new Texture("../textures/Atlas.png"));
//    textureMap.emplace("asphalt", new Texture("../textures/asphalt.jpg"));
//    textureMap.emplace("roof", new Texture("../textures/roof_rgba.png"));
//    textureMap.emplace("building_side1", new Texture("../textures/building_side1_rgba.png"));
#endif

////    pBuildingTex = new Texture("../data/face.png");

//    glCreateFramebuffers(1, &glidFramebuffer);
    glGenFramebuffers(1, &glidFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glidFramebuffer);

//    glCreateTextures(GL_TEXTURE_2D, 1, &glidTextureScreenRoads);
    glGenTextures(1, &glidTextureScreenRoads);
    glBindTexture(GL_TEXTURE_2D, glidTextureScreenRoads);
//    glTextureStorage2D(glidTextureScreenRoads, 1, GL_RED, windowWidth, windowHeight);
//    glClearTexImage(glidTextureScreenRoads, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
//    glGenerateTextureMipmap(glidTextureScreenRoads);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, windowWidth, windowHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, glidTextureScreenRoads, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
//    glBindFramebuffer(GL_FRAMEBUFFER, glidFramebuffer);
//    glNamedFramebufferDrawBuffers(glidFramebuffer, 1, drawBuffers);
//    glNamedFramebufferTexture(glidFramebuffer, GL_COLOR_ATTACHMENT0, glidTextureScreenRoads, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, glidFramebuffer);

//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glBindTexture(GL_TEXTURE_2D, 0);



   FT_Library ft;
   if (FT_Init_FreeType(&ft))
       std::cout << "ERROR::FREETYPE: Could nolololt init FreeType Library" << std::endl;

   FT_Face face;
#ifdef __APPLE__
   if (FT_New_Face(ft, "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/fonts/arial.ttf",0,&face))
#else
   if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face))
#endif
       std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
   else
       PRINT("Loaded Freetype font! yayy");
   FT_Set_Pixel_Sizes(face, 0, 48);
   if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
       std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

   for (GLubyte c = 0; c < 128; c++)
   {
       // Load character glyph
       if (FT_Load_Char(face, c, FT_LOAD_RENDER))
       {
           std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
           continue;
       }
       // Generate texture
       GLuint texture;
       glGenTextures(1, &texture);
       glBindTexture(GL_TEXTURE_2D, texture);
       glTexImage2D(
           GL_TEXTURE_2D,
           0,
           GL_RED,
           face->glyph->bitmap.width,
           face->glyph->bitmap.rows,
           0,
           GL_RED,
           GL_UNSIGNED_BYTE,
           face->glyph->bitmap.buffer
       );
       // Set texture options
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       // Now store character for later use
       Character character = {
           texture,
           glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
           glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
#ifdef __APPLE__
           static_cast<GLuint>(face->glyph->advance.x)
#else
           face->glyph->advance.x
#endif
       };
       Characters.insert(std::pair<GLchar, Character>(c, character));
   }
   FT_Done_Face(face);
   FT_Done_FreeType(ft);




}

void Renderer::DrawModel(Model* model)
{


  for (int i = 0; i < model->m_meshes.size(); ++i)
  {
    activePipeline = model->m_materialList[i]->m_programPipeline;
    glBindProgramPipeline(activePipeline->glidProgramPipeline);
    glProgramUniformMatrix4fv(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
    glProgramUniformMatrix4fv(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(activePipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "modelMatrix"), 1, false, value_ptr(model->m_modelMatrix));

    DrawMesh(model->m_meshes[i], model->m_materialList[i]);
    outputGLError(model->name, __LINE__);
  }
}

void Renderer::DrawMesh(Mesh* mesh, Material* material)
{
  glBindVertexArray(mesh->m_VAO);
  activePipeline->setUniform("lineColor", material->m_albedo);
  
  if (mesh->m_wireframeRendering)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (mesh->m_pointRendering)
  {
    glEnable(GL_PROGRAM_POINT_SIZE);
  }

  if (mesh->m_usesIndexBuffer){ 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_VBOs.glidIndexBuffer);
    glDrawElements(mesh->m_renderingMode, mesh->m_buffers.indexBuffer.size(), GL_UNSIGNED_INT, 0);
  }
  else
  {
    glDrawArrays(mesh->m_renderingMode, 0, mesh->m_buffers.vertexPositions.size());
  }

  if (mesh->m_pointRendering)
  {
    glDisable(GL_PROGRAM_POINT_SIZE);
  }

  if (mesh->m_wireframeRendering)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

}


/**
 * [Renderer::render description]
 * @param camera [description]
 */
void Renderer::render()
{

    if(!fancyDisplayMode)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        RenderSky();

        for (Model* model: m_debugModels)
        {
          DrawModel(model);
        }



        // draw entities
#ifndef __APPLE__
//        entityManager.Render(camera->vpMat);
#endif
    } else {
      
        glBindFramebuffer(GL_FRAMEBUFFER, glidFramebuffer);
        glClear(GL_COLOR_BUFFER_BIT);

        if (drawGround){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureMap["grass"]->glidTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureMap["rock"]->glidTexture);
            pPipelineGround->usePipeline();
            glProgramUniformMatrix4fv(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
            glProgramUniform1i(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
            glProgramUniform1i(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler1"), 1);
            glDrawArrays(GL_TRIANGLES, 0, nbGroundVertices);
        }


        glActiveTexture(GL_TEXTURE0);

        // draw roads
        glBindTexture(GL_TEXTURE_2D, glidTextureScreenRoads);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "qweqwe" << endl;
        pPipelineRoads->usePipeline();
        pPipelineRoads->useGeometryShader(pGeometryShader1);
        pPipelineRoads->useFragmentShader(pFragmentShader1);
        glProgramUniformMatrix4fv(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
        glMultiDrawArrays(GL_LINE_STRIP_ADJACENCY, firstVertexForEachRoad.data(), nbVerticesForEachRoad.data(), nbRoads);
        // draw circles on road nodes
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "qweqwe" << endl;
        pPipelineRoads->usePipeline();
        pPipelineRoads->useGeometryShader(pGeometryShader2);
        pPipelineRoads->useFragmentShader(pFragmentShader2);
        glProgramUniformMatrix4fv(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
        glMultiDrawArrays(GL_LINE_STRIP_ADJACENCY, firstVertexForEachRoad.data(), nbVerticesForEachRoad.data(), nbRoads);
        // draw road lines
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "qweqwe" << endl;
        pPipelineRoads->usePipeline();
        pPipelineRoads->useGeometryShader(pGeometryShader3);
        pPipelineRoads->useFragmentShader(pFragmentShader3);
        glProgramUniformMatrix4fv(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineRoads->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
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
        outputGLError(__func__, __LINE__);

//         pPipelineBuildings->usePipeline();
//         glProgramUniformMatrix4fv(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
// // /           glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
//         glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
//         glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineGround->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler1"), 1);
//         glDrawArrays(GL_TRIANGLES, 0, nbBuildingTriangles);

        // draw building sides
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, textureMap["bricks"]->glidTexture);
        // pPipelineBuildingSides->usePipeline();
        // glProgramUniformMatrix4fv(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
        // glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uBuildingHeight"), 3);
        // glProgramUniform1f(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildingSides->getShader(GL_GEOMETRY_SHADER)->glidShaderProgram, "uScaleFactor"), 1);
        // glDrawArrays(GL_LINES, 0, nbBuildingLines);
    }
  RenderText("test", 0.5f, 0.5f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), false);
  RenderText("LIGUMX BITCHES", -0.7f, 0.5f, 0.0001f, glm::vec3(0.5, 0.8f, 0.2f), true);

  RenderFPS();

   // if (showText)
   //     for (int i = 0; i < texts.size(); i++){
   //         RenderText(texts[i]);
   //     }

    // screenshot
//    if(saveScreenshot) {
//        static unsigned int frameCount = 0;
//        ++frameCount;
//        std::stringstream filename;
//        filename.clear();
//        filename << "../output/frame" << std::setfill('0') << std::setw(5) << frameCount << ".png";

//        BYTE* pixels = new BYTE[4 * windowWidth * windowHeight];
//        glReadPixels(0, 0, windowWidth, windowHeight, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
//        FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, 4 * windowWidth, 32, 0x0000FF, 0xFF0000, 0x00FF00, false);
//        if(FreeImage_Save(FIF_BMP, image, filename.str().c_str(), 0)) {
//            std::cout << "screenshot '" << filename.str() << "' taken." << std::endl;
//        } else {
//            std::cout << "screenshot failed. Did you create the 'protoEngine/output' directory?" << std::endl;
//        }
//        FreeImage_Unload(image);
//        delete [] pixels;
//    }
//    

    FLUSH_ERRORS();


}

void Renderer::RenderFPS()
{
  std::stringstream fpsString;
  float smoothing = 0.99; // larger=more smoothing
  fps = (fps * smoothing) + (1.f/dt * (1.0-smoothing));
  fpsString << round(fps);
  fpsString << " fps";
  RenderText(fpsString.str(), 750.0f, 775.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f), false);

  double new_time = glfwGetTime();
  dt = new_time - curr_time;
  curr_time = new_time;
}

void Renderer::RenderSky()
{
       pPipelineEnvmap->usePipeline();
       GLuint fragProg = pPipelineEnvmap->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram;
       glProgramUniform2f(fragProg, glGetUniformLocation(fragProg, "windowSize"), windowWidth, windowHeight);
       glProgramUniform1f(fragProg, glGetUniformLocation(fragProg, "sunOrientation"), 0);
       glProgramUniform1f(fragProg, glGetUniformLocation(fragProg, "sunTime"), 0);
       glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewDir"), -camera->frontVec.x, -camera->frontVec.y, -camera->frontVec.z); // the frontVec for the camera points towards the eye, so we reverse it to get the view direction.
       glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewRight"), camera->rightVec.x, camera->rightVec.y, camera->rightVec.z);
       glProgramUniform3f(fragProg, glGetUniformLocation(fragProg, "viewUp"), camera->upVec.x, camera->upVec.y, camera->upVec.z);
       glProgramUniform2f(fragProg, glGetUniformLocation(fragProg, "viewAngles"), camera->totalViewAngleY*glm::pi<float>()/180.0, camera->aspectRatio*camera->totalViewAngleY*glm::pi<float>()/180.0);
       glProgramUniform1f(fragProg, glGetUniformLocation(fragProg, "viewNear"), camera->near);
       glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
       glClear(GL_DEPTH_BUFFER_BIT);
}


void Renderer::RenderText(Text t)
{
   glEnable(GL_CULL_FACE);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // Activate corresponding render state
   pPipelineText->usePipeline();
   GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
   glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
   glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
   if (t.projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(camera->vpMat));
   else glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f)));

   std::string text = t.text;
   GLfloat uvs[6][2] = {
       {0.0, 0.0},
       {0.0, 1.0},
       {1.0, 1.0},

       {0.0, 0.0},
       {1.0, 1.0},
       {1.0, 0.0}

   };
   int index = 0;
   GLfloat x = t.position.x;
   GLfloat y = t.position.y;
   GLfloat scale = t.scale;

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
       // std::cout << "\n";
       // std::cout << xpos << "\n";
       // std::cout << ypos << "\n";
       // std::cout << h << "\n";
       // std::cout << w << "\n";
       // std::cout << "\n";
       // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
       x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
   }
//    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);

   glDisable(GL_BLEND);
   glDisable(GL_CULL_FACE);
}

void Renderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, bool projected)
{
   glEnable(GL_CULL_FACE);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // Activate corresponding render state
   pPipelineText->usePipeline();
   GLuint prog = pPipelineText->getShader(GL_VERTEX_SHADER)->glidShaderProgram;
   glm::vec3 myColor = glm::vec3(1.0,1.0,1.0);
   glProgramUniform3f(prog, glGetUniformLocation(prog, "textColor"), myColor.x, myColor.y, myColor.z);
   if (projected) glProgramUniformMatrix4fv(prog, glGetUniformLocation(prog, "projection"), 1, false, value_ptr(camera->vpMat));
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



void Renderer::outputGLError(std::string func, int line)
{
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) 
  {
    string error;

    switch(err) 
    {
      case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
      case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
      case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
      case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
    }
    std::stringstream ss;

    ss << func << " : " << line;
    PRINTSTRING(ss.str());
    PRINTSTRING(error.c_str());
    err=glGetError();    
  }
}
