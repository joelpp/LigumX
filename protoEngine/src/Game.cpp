#include "Game.h"
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <glm/gtc/random.hpp>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include <typeinfo>
#include <chrono>
#include "glm/gtx/string_cast.hpp"
#include "Texture.h"

#define TIME(x)    {auto begin = std::chrono::high_resolution_clock::now();\
                    x;\
                    auto end = std::chrono::high_resolution_clock::now();\
                    std::cout << "Time to run \"" << #x << "\" : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;}

using namespace glm;
using namespace std;

Game* game;

int main()
{
    game = new Game();
    while(game->running) {
        glfwPollEvents();

        game->mainLoop();
        glfwSwapBuffers(game->pWindow);
    }
    glfwTerminate();
    return 0;
}

void Game::init()
{
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    glEnable(GL_POINT_SPRITE);

    //=============================================================================
    // Parameters, camera setup.
    //=============================================================================
    running = true;
    windowWidth = 800;
    windowHeight = 800;
    windowTitle = "LigumX";

    camera = new Camera();
    camera->translateTo(vec2(0,0));
    camera->setViewSize(1);
    draggingCamera = false;


    //=============================================================================
    // create window and GLcontext, register callbacks.
    //=============================================================================

    // set window paramaters
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


    // Initialise GLFW
    if( !glfwInit() )
    {
        cerr << "Failed to initialize GLFW.\n";
        return;
    } else {
        clog << "Initialized GLFW." << endl;
    }

    // create GLFW window
    pWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), 0, NULL);
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
        clog << "Initialized GLEW." << endl;
    }

    // register GLFW and GLdebug callbacks
    glfwSetMouseButtonCallback( pWindow, glfwMouseButtonCallback );
    glfwSetKeyCallback( pWindow, glfwKeyCallback );
    glfwSetCharCallback( pWindow, glfwCharCallback );
    glfwSetCursorPosCallback( pWindow, glfwMousePositionCallback );
    glfwSetCursorEnterCallback( pWindow, glfwMouseEntersCallback );
    glfwSetScrollCallback( pWindow, glfwMouseScrollCallback );
    glfwSetWindowPosCallback( pWindow, glfwWindowPositionCallback );
    glfwSetWindowSizeCallback( pWindow, glfwWindowSizeCallback );
    glfwSetWindowCloseCallback( pWindow, glfwWindowClosedCallback );
    glfwSetWindowRefreshCallback( pWindow, glfwWindowRefreshCallback );
    glfwSetWindowFocusCallback( pWindow, glfwWindowFocusCallback );
    glfwSetWindowIconifyCallback( pWindow, glfwWindowIconifyCallback );
    glfwSetFramebufferSizeCallback( pWindow, glfwWindowFramebufferSizeCallback );
//    glDebugMessageCallback(Game::debugCallback, NULL);
//    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    //=============================================================================
    // Load world data.
    //=============================================================================

    vector<vec2> waysNodesPositions; // positions of nodes forming ways, possibly contains duplicates.
    vector<vec3> waysNodesColors;
    vector<vec2> roadsPositions;
    vector<vec2> buildingTrianglePositions;

    TIME(loadXML("../data/result.xml"));
    TIME(fillBuffers(&waysNodesPositions, &waysNodesColors, &roadsPositions, &buildingTrianglePositions));


    //=============================================================================
    // Screen quad data.
    //=============================================================================

    vec2 screenQuadPos[4] = {vec2(1,-1), vec2(1,1), vec2(-1,-1), vec2(-1,1)};
    vec2 screenQuadTexCoords[4] = {vec2(1,0), vec2(1,1), vec2(0,0), vec2(0,1)};

    //=============================================================================
    // create and fill VBOs.
    //=============================================================================

    nbWaysVertices = waysNodesPositions.size();
    nbBuildingTriangles = buildingTrianglePositions.size();


    glCreateBuffers(1, &glidWaysPositions);
    glNamedBufferStorage(glidWaysPositions, nbWaysVertices * 2 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidWaysPositions, 0, nbWaysVertices * 2 * 4, waysNodesPositions.data());


    glCreateBuffers(1, &glidWaysColors);
    glNamedBufferStorage(glidWaysColors, nbWaysVertices * 3 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidWaysColors, 0, nbWaysVertices * 3 * 4, waysNodesColors.data());


    glCreateBuffers(1, &glidBufferRoadsPositions);
    glNamedBufferStorage(glidBufferRoadsPositions, roadsPositions.size() * 2 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidBufferRoadsPositions, 0, roadsPositions.size() * 2 * 4, roadsPositions.data());


    glCreateBuffers(1, &glidScreenQuadPositions);
    glNamedBufferStorage(glidScreenQuadPositions, 4 * 2 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidScreenQuadPositions, 0, 4 * 2 * 4, screenQuadPos);


    glCreateBuffers(1, &glidScreenQuadTexCoords);
    glNamedBufferStorage(glidScreenQuadTexCoords, 4 * 2 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidScreenQuadTexCoords, 0, 4 * 2 * 4, screenQuadTexCoords);

    glCreateBuffers(1, &glidBufferBuildingTriangleVertices);
    glNamedBufferStorage(glidBufferBuildingTriangleVertices, nbBuildingTriangles * 2 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidBufferBuildingTriangleVertices, 0, nbBuildingTriangles * 2 * 4, buildingTrianglePositions.data());


    //=============================================================================
    // Textures, framebuffer, renderbuffer
    //=============================================================================


    pBuildingTex = new Texture("../data/face.png");

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





    //=============================================================================
    // initialize graphics, VAO
    //=============================================================================

    //--------------------------------------------------------------------------
    // general lines pipeline
    //--------------------------------------------------------------------------

    {
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec2 pos;\n \
        in layout(location=1) vec3 color;\n \
        uniform mat4 vpMat;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out vec3 vColor;\n \
        void main() {\n \
            gl_Position = vpMat * vec4(pos, 0, 1);\n \
            vColor = color;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 430 core\n \
        in vec3 vColor;\n \
        out vec3 color;\n \
        void main() {\n \
            color = vColor;\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);



    pPipelineLines = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipelineLines->useVertexShader(pVertexShader);
    pPipelineLines->useFragmentShader(pFragmentShader);

    // VAO
    glEnableVertexArrayAttrib(pPipelineLines->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineLines->glidVao, 0, glidWaysPositions, 0, 2*4);
    glVertexArrayAttribFormat(pPipelineLines->glidVao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineLines->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineLines->glidVao, 1, glidWaysColors, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineLines->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);
    }

    //--------------------------------------------------------------------------
    // roads pipeline
    //--------------------------------------------------------------------------

    {
            glEnable(GL_PROGRAM_POINT_SIZE);
            glEnable(GL_POINT_SPRITE);
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec2 pos;\n \
        in layout(location=1) float distance;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
//        out float vDistance;\n \
        void main() {\n \
            gl_Position = vec4(pos, 0, 1);\n \
//            vDistance = distance;\n \
        }";

    const char* fragmentShaderSource1 = " \
        #version 430 core\n \
//        in vec2 gTexCoord;\n \
        layout(location=0) out vec3 fColor;\n \
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
        #version 430 core\n \
//        in vec2 gTexCoord;\n \
        in vec2 gl_PointCoord;\n \
        layout(location=0) out vec3 fColor;\n \
        void main() {\n \
            if(distance(gl_PointCoord, vec2(0.5,0.5)) < 0.5) {\n \
                fColor = vec3(0.5, 0.5, 0.5);\n \
            } else {\n \
                discard;\n \
            }\n \
        }";

    const char* fragmentShaderSource3 = " \
        #version 430 core\n \
        layout(location=0) out vec3 fColor;\n \
        void main() {\n \
            fColor = vec3(1,1,0);\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    pGeometryShader1 =
            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
            "../src/shaders/roads.gsh", false);

    pGeometryShader2 =
            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
            "../src/shaders/roads2.gsh", false);

    pGeometryShader3 =
            new ProgramPipeline::ShaderProgram(GL_GEOMETRY_SHADER,
            "../src/shaders/roads3.gsh", false);

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
    glEnableVertexArrayAttrib(pPipelineRoads->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineRoads->glidVao, 0, glidBufferRoadsPositions, 0, 2*4);
    glVertexArrayAttribFormat(pPipelineRoads->glidVao, 0, 2, GL_FLOAT, GL_FALSE, 0);
//    glEnableVertexArrayAttrib(pPipelineRoads->glidVao, 1);
//    glVertexArrayVertexBuffer(pPipelineRoads->glidVao, 1, glidBufferRoadsPositions, 0, 1*4);
//    glVertexArrayAttribFormat(pPipelineRoads->glidVao, 1, 1, GL_FLOAT, GL_FALSE, 0);
    }


    //--------------------------------------------------------------------------
    // screen quad pipeline
    //--------------------------------------------------------------------------

    {
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec2 pos;\n \
        in layout(location=1) vec2 texCoord;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out vec2 vTexCoord;\n \
        void main() {\n \
            gl_Position = vec4(pos, 0, 1);\n \
            vTexCoord = texCoord;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 430 core\n \
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
    glEnableVertexArrayAttrib(pPipelineScreenQuad->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineScreenQuad->glidVao, 0, glidScreenQuadPositions, 0, 2*4);
    glVertexArrayAttribFormat(pPipelineScreenQuad->glidVao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineScreenQuad->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineScreenQuad->glidVao, 1, glidScreenQuadTexCoords, 0, 2*4);
    glVertexArrayAttribFormat(pPipelineScreenQuad->glidVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
    }

    //--------------------------------------------------------------------------
    // filled buildings pipeline
    //--------------------------------------------------------------------------

    {
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec2 pos;\n \
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
        #version 430 core\n \
        in vec2 texCoord;\n \
        uniform layout(location=0) sampler2D sampler;\n \
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
    glEnableVertexArrayAttrib(pPipelineBuildings->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineBuildings->glidVao, 0, glidBufferBuildingTriangleVertices, 0, 2*4);
    glVertexArrayAttribFormat(pPipelineBuildings->glidVao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    }



}


void Game::mainLoop()
{
    camera->moveFromUserInput(pWindow);

    if(showWhat) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        // draw lines
        pPipelineLines->usePipeline();
        glProgramUniformMatrix4fv(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
        glDrawArrays(GL_LINES, 0, nbWaysVertices);
//        glDrawArrays(GL_POINTS, 0, nbWaysVertices);
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, glidFramebuffer);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw roads
        glBindTexture(GL_TEXTURE_2D, glidTextureScreenRoads);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "qweqwe" << endl;
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
//                glBindTexture(GL_TEXTURE_2D, pBuildingTex->glidTexture);
        glClear(GL_COLOR_BUFFER_BIT);
        pPipelineScreenQuad->usePipeline();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//        // draw buildings
        glBindTexture(GL_TEXTURE_2D, pBuildingTex->glidTexture);
        pPipelineBuildings->usePipeline();
        glProgramUniformMatrix4fv(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
//        glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
        glDrawArrays(GL_TRIANGLES, 0, nbBuildingTriangles);
    }
}



void Game::insertDebugMessage(string message, GLenum severity, GLuint id)
{
    // TODO: make glDebug work.
//    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR,
//                         id, severity, GLsizei(message.length()), message.c_str());
    cout << message << endl;
}

//void Game::debugCallback(GLenum, GLenum, GLuint, GLenum, GLsizei length, const GLchar* message, const void *)
//{
//    cout << "message :"  << endl;
//    for(int i=0; i<length; ++i) {
//        cout << message[i] << endl;
//    }
//}

void Game::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    game->running = false;
}

void Game::glfwMouseScrollCallback(GLFWwindow* /*pWindow*/, double xOffset, double yOffset)
{
    static const float factor = 1.1;
    if(yOffset < 0) {
        game->camera->multViewSizeBy(factor);
    } else {
        game->camera->multViewSizeBy(1.f/factor);
    }
}

void Game::glfwKeyCallback(GLFWwindow* /*pGlfwWindow*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    if(action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
         game->showWhat = !game->showWhat;
    }
}

void Game::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
    // Left-click
    if (button == GLFW_MOUSE_BUTTON_1){
        if (action == GLFW_PRESS){
            game->draggingCamera = true;
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);
            game->oldMousePosition = vec2(x,y);
        }
        else if (action == GLFW_RELEASE){
            game->draggingCamera = false;
        }
    }
    //Right Click
    else if (button == GLFW_MOUSE_BUTTON_2){
        game->camera->translateTo(vec2(0,0));
    }


}

void Game::glfwMousePositionCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
    if (game->draggingCamera){
        double x; double y;
        glfwGetCursorPos(pWindow, &x, &y);
        vec2 offset = vec2(x,y) - game->oldMousePosition;
        offset.y *= -1; // reversed controls? this should be an option

        game->camera->translateBy(offset/100.f);

        game->oldMousePosition = vec2(x,y);

    }
}

void Game::loadXML(string path){
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());
    tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();
    cout << docRoot->Value() << "\n";

    for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        if (string(child->Value()).compare("bound") == 0){
            string box = child->ToElement()->FindAttribute("box")->Value();
            std::istringstream ss(box);
            std::string token;

            std::getline(ss, token, ','); viewRectLeft = atof(token.c_str());
            std::getline(ss, token, ','); viewRectBottom = atof(token.c_str());
            std::getline(ss, token, ','); viewRectRight = atof(token.c_str());
            std::getline(ss, token, ','); viewRectTop = atof(token.c_str());

            viewRectBottomLeft = vec2(viewRectLeft, viewRectBottom);
            viewRectTopRight = vec2(viewRectRight, viewRectTop);
            viewRectVecDiago = viewRectBottomLeft - viewRectTopRight;
        }
        if (string(child->Value()).compare("node") == 0){
//            cout << "Looking at a node \n";
//            cout << child->ToElement()->FindAttribute("id")->Value() << "\n";

            string id = child->ToElement()->FindAttribute("id")->Value();
            float latitude = atof(child->ToElement()->FindAttribute("lat")->Value());
            float longitude = atof(child->ToElement()->FindAttribute("lon")->Value());
            //double latitude = strtod(child->ToElement()->FindAttribute("lat")->Value(), NULL);
            //double longitude = strtod(child->ToElement()->FindAttribute("lon")->Value(), NULL);

            Node* node = new Node(id, latitude, longitude);
            for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement()){
                string key = tag->ToElement()->FindAttribute("k")->Value();
                string value = tag->ToElement()->FindAttribute("v")->Value();
                node -> addTag(key, value);
            }
            theNodes.emplace(id, node);
        }

        else if (string(child->Value()).compare("way") == 0){
//            cout << "Looking at a way \n";
            string id = child->ToElement()->FindAttribute("id")->Value();
            Way* way = new Way(id);

            for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement()){
                if (string(way_child->Value()).compare("nd") == 0){
                    string ref = way_child->ToElement()->FindAttribute("ref")->Value();
                    way -> addRef(theNodes[ref]);
                }
                else if (string(way_child->Value()).compare("tag") == 0){
                    string key = way_child->ToElement()->FindAttribute("k")->Value();
                    string value = way_child->ToElement()->FindAttribute("v")->Value();
                    way -> addTag(key, value);
                }
            }
            theWays.emplace(id, way);
        }
        else if (string(child->Value()).compare("relation") == 0){
            string id = child->ToElement()->FindAttribute("id")->Value();
            Relation *relation = new Relation(id);

            for (tinyxml2::XMLNode* relation_child = child->FirstChildElement(); relation_child != NULL; relation_child = relation_child->NextSiblingElement()){
                if (string(relation_child->Value()).compare("member") == 0){
                    string type = relation_child->ToElement()->FindAttribute("type")->Value();
                    string id = relation_child->ToElement()->FindAttribute("ref")->Value();
                }
                else if (string(relation_child->Value()).compare("tag") == 0){
                    string key = relation_child->ToElement()->FindAttribute("k")->Value();
                    string value = relation_child->ToElement()->FindAttribute("v")->Value();
                }
            }

        }
    }
}

void Game::fillBuffers(vector<vec2> *waysNodesPositions,
                       vector<vec3> *waysNodesColors,
                       vector<vec2> *roadsPositions,
                       vector<vec2> *buildingTrianglePositions){
    bool first;
    bool second;
    vec3 white = vec3(1.0f,1.0f,1.0f);

    unsigned int nbSuccessLoops = 0;
    unsigned int nbFailedLoops = 0;

    static unsigned int nbBuildings = 0;

    nbRoads = 0;
    GLint firstVertexForThisRoad = 0;
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        first = true;
        second = false;
        GLsizei nbVertexForThisRoad = 0;
        vec2 point, oldPoint, oldOldPoint;
        vec3 color = vec3(glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f));
        Way* way = it->second;

        if (way->hasTagAndValue("highway", "trunk")) color = vec3(1,1,1);
        else if (way->hasTagAndValue("highway", "primary")) color = vec3(0.9,0.9,0.9);
        else if (way->hasTagAndValue("highway", "secondary")) color = vec3(0.8,0.8,0.8);
        else if (way->hasTagAndValue("highway", "residential")) color = vec3(0.5,0.5,0.5);
        else if (way->hasTag("building")) color = vec3(0,0,1);
        else if (way->hasTag("railway")) color = vec3(1,0,1);
        else if (way->hasTag("natural")) color = vec3(0,0.5,0);
        else if (way->hasTagAndValue("leisure", "park")) color = vec3(0,1,0);
        else continue;

        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt){
            Node* node = *nodeIt;
            point = vec2(node->latitude, node->longitude);
            point = (point - viewRectBottomLeft)/viewRectVecDiago;
            point = -2.0f*vec2(point.y, point.x) - vec2(1,1);

            if (!first){
                waysNodesPositions->push_back(oldPoint);
                waysNodesColors->push_back(color);
                waysNodesPositions->push_back(point);
                waysNodesColors->push_back(color);

                // create roads subgroup
                //if (way->hasTagAndValue("highway", "residential")) {
                if (way->hasTag("highway")) {
                    if(second) {
                        second = false;
                        // first point for line adjacency, fake straight line
                        roadsPositions->push_back(oldPoint - (point-oldPoint));
                        roadsPositions->push_back(oldPoint);
                        nbVertexForThisRoad += 2;
                    }
                    roadsPositions->push_back(point);
                    ++nbVertexForThisRoad;
                }
            }
            else
            {
                first = false;
                //if (way->hasTagAndValue("highway", "residential")) {
                if (way->hasTag("highway")) {
                    second = true;
//                    roadsPositions->push_back(point); // first point for line adjacency
//                    ++nbVertexForThisRoad;
                }
            }

            oldOldPoint = oldPoint;
            oldPoint = point;
        }

        // push last road node for line adjacency, fake straight line
        //if (way->hasTagAndValue("highway", "residential")) {
        if (way->hasTag("highway")) {
            roadsPositions->push_back(point + (point-oldOldPoint));
            ++nbVertexForThisRoad;
            ++nbRoads;

            firstVertexForEachRoad.push_back(firstVertexForThisRoad);
            nbVerticesForEachRoad.push_back(nbVertexForThisRoad);

            // set first vertex index for next road
            firstVertexForThisRoad += nbVertexForThisRoad;
        }

        // triangulate building loops
        Node first(*(way->nodes.front()));
        Node last(*(way->nodes.back()));
        if(way->hasTag("building") && way->nodes.size() >= 3 && first == last) {

//            cout << "building loop" << endl;

            // Note: supposed all loops are given in counter-clockwise order.
            unsigned int nbTriangles = 0;

            // first try clockwise loops, then try counterclockwise if it failed.
            vector<vec2> tempTriangleVertices;
            bool failedLoop;
            for(int clockwiseness=-1; clockwiseness<=1; clockwiseness += 2) {

                // copy node loop (not repeating the last node)
                vector<Node*> loopNodes;
                for(auto n = way->nodes.begin(); n != way->nodes.end()-1; ++n) {
                    loopNodes.push_back(*n);
                }

                unsigned int nbLoops = 0;

                // recursively transform loop into triangles until fully consumed.
                auto nodeIt1 = loopNodes.begin();
                auto nodeIt2 = loopNodes.begin() + 1;
                auto nodeIt3 = loopNodes.begin() + 2;
                failedLoop = false;
                do {
                    if(nbLoops++ > 2*loopNodes.size()) {
    //                    cout << "missed triangle." << endl;
                        failedLoop = true;
                        break;
                    }
                    Node* n1 = *(nodeIt1);
                    Node* n2 = *(nodeIt2);
                    Node* n3 = *(nodeIt3);
                    vec2 p1 = vec2(n1->latitude, n1->longitude);
                    vec2 p2 = vec2(n2->latitude, n2->longitude);
                    vec2 p3 = vec2(n3->latitude, n3->longitude);
                    if(p1==p2 || p1==p3) cout << "bad precision" << endl;
                    vec2 v12 = p2 - p1;
                    vec2 v13 = p3 - p1;

                    vec3 v12_3D = vec3(v12.x, v12.y, 0);
                    vec3 v23_3D = vec3(p3.x - p2.x, p3.y - p2.y, 0);
                    vec3 v31_3D = vec3(p1.x - p3.x, p1.y - p3.y, 0);

                    bool isGoodTriangle;
                    // check clockwiseness
                    isGoodTriangle = clockwiseness * glm::cross(vec3(v12.x, v12.y, 0.f) , vec3(v13.x, v13.y, 0.f)).z > 0.f;
                    if(isGoodTriangle) {
                        // make sure it doesn't include another active node
                        for(Node* node : loopNodes) {
                            if(node == n1 || node == n2 || node == n3) continue;
                            vec3 n_3D = vec3(node->latitude, node->longitude, 0);
                            if(clockwiseness * glm::cross(v12_3D, n_3D - vec3(p1.x, p1.y, 0)).z > 0.f &&
                               clockwiseness * glm::cross(v23_3D, n_3D - vec3(p2.x, p2.y, 0)).z > 0.f &&
                               clockwiseness * glm::cross(v31_3D, n_3D - vec3(p3.x, p3.y, 0)).z > 0.f ) {
                                isGoodTriangle = false;
                                break;
                            }
                        }
                    }
                    if(isGoodTriangle) {
                        // create triangle
                        vec2 p1 = vec2(n1->latitude, n1->longitude);
                        vec2 p2 = vec2(n2->latitude, n2->longitude);
                        vec2 p3 = vec2(n3->latitude, n3->longitude);

                        vec2 point;
                        point = (p1 - viewRectBottomLeft)/viewRectVecDiago;
                        point = -2.0f*vec2(point.y, point.x) - vec2(1,1);
                        //buildingTrianglePositions->push_back(point);
                        tempTriangleVertices.push_back(point);
                        point = (p2 - viewRectBottomLeft)/viewRectVecDiago;
                        point = -2.0f*vec2(point.y, point.x) - vec2(1,1);
//                        buildingTrianglePositions->push_back(point);
                        tempTriangleVertices.push_back(point);
                        point = (p3 - viewRectBottomLeft)/viewRectVecDiago;
                        point = -2.0f*vec2(point.y, point.x) - vec2(1,1);
//                        buildingTrianglePositions->push_back(point);
                        tempTriangleVertices.push_back(point);

    //                    buildingTrianglePositions->push_back(p1);
    //                    buildingTrianglePositions->push_back(p2);
    //                    buildingTrianglePositions->push_back(p3);

                        // delete node from loop. std::vector reassigns positions to all elements after the one deleted, so we must reassign the iterators acordingly.
                        if(nodeIt2 == loopNodes.begin()) {
                            loopNodes.erase(nodeIt2);
                            nodeIt3 = loopNodes.begin();
                            nodeIt2 = loopNodes.end() - 1;
                            nodeIt1 = nodeIt2 - 1;
                        } else {
                            loopNodes.erase(nodeIt2);
                            nodeIt2 = nodeIt1 + 1;
                            if(nodeIt2 == loopNodes.end()) nodeIt2 = loopNodes.begin();
                            nodeIt3 = nodeIt2 + 1;
                            if(nodeIt3 == loopNodes.end()) nodeIt3 = loopNodes.begin();
                        }
                        nbLoops = 0;
                    } else {
                        // no triangle, check next
                        nodeIt1 = nodeIt2;
                        nodeIt2 = nodeIt3;
                        ++nodeIt3;
                        if(nodeIt3 == loopNodes.end()) nodeIt3 = loopNodes.begin();
                    }

                } while(loopNodes.size() >= 3);
                if(!failedLoop) {
                    break; // do not check other clockwiseness, the first guess worked.
                } else {
                    tempTriangleVertices.clear();
                }
            }
            if(failedLoop) {
                nbFailedLoops++;
            } else {
                nbSuccessLoops++;
                // keep and copy the triangles we created
                for(vec2 v : tempTriangleVertices) {
                    buildingTrianglePositions->push_back(v);
                }
            }
        }

    }

    cout << "succeeded loops: " << nbSuccessLoops << endl;
    cout << "failed loops: " << nbFailedLoops << endl;
}
