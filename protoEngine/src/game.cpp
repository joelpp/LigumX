#include <sstream>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <typeinfo>
#include <chrono>

#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "game.h"
#include "texture.h"


#define TIME(x)    {auto begin = std::chrono::high_resolution_clock::now();\
                    x;\
                    auto end = std::chrono::high_resolution_clock::now();\
                    std::cout << "Time to run \"" << #x << "\" : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;}
#define PRINTVEC2VECTOR(v) for (int _index_ = 0; _index_ < v.size(); _index_++) std::cout << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y << "\n";
#define PRINTVEC2(v) std::cout << #v << ": x=" << v.x << " y=" << v.y << "\n";
#define PRINTVEC3(v) std::cout << #v << ": x=" << v.x << " y=" << v.y << " z=" << v.z << "\n";
#define PRINT(f) std::cout << #f << ": = " << f << "\n";
#define PRINTSTRING(f) std::cout << f << "\n";
#define PRINTBOOL(B) string b = B==1?"TRUE":"FALSE"; std::cout << #B << " : " << b << "\n";
#define PRINTINT(i) std::cout << #i << ": x=" << i << "\n";
#define PRINTVECTOR(e) std::cout << "Element Vector: " << #e << "\n"; \
                       for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_] << "\n";
#define PRINTELEMENT(e) std::cout << #e << " " << e.toString() << "\n";
#define PRINTELEMENTPTR(e) std::cout << #e << " " << e->toString() << "\n";
#define PRINTELEMENTVECTOR(e) std::cout << "Element Vector: " << #e << "\n"; \
                              for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_]->toString() << "\n";

#define string_pair std::pair<std::string,std::string>

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
    camera->setViewSize(0.03);
    draggingCamera = false;
    fancyDisplayMode = false;

    selectedWay.way = NULL;
    unsuccessfulInterpolations = 0;
    coordinateInflationFactor = 1;
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



    //Initialize AntTweakBar
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(windowWidth, windowHeight);

    myBar = TwNewBar("NameOfMyTweakBar");
    TwAddVarRW(myBar, "Fancy Display Mode", TW_TYPE_BOOLCPP, &fancyDisplayMode, NULL);

    TwEnumVal CameraTypeEV[] = { {Camera::CameraType::AIRPLANE, "Airplane"}, {Camera::CameraType::AROUND_TARGET, "Around Target"}, {Camera::CameraType::CYLINDRICAL, "Cynlindrical"}, {Camera::CameraType::TOP_2D, "Top 2D"}, {Camera::CameraType::TOP_3D, "Top 3D"} };
    TwType CameraTwType;
    CameraTwType = TwDefineEnum("CameraType", CameraTypeEV, 5);
    TwAddVarRW(myBar, "Camera Type", CameraTwType, &(camera->cameraType), NULL);

    TwEnumVal ControlTypeEV[] = { {Camera::ControlType::QWEASDZXC_DRAG, "QWEASDZXC Drag"}, {Camera::ControlType::QWEASDZXC_CONTINUOUS, "QWEASDZXC Continuous"}, {Camera::ControlType::QWEASDZXC_ARROWS, "QWEASDZXC Arrows"} };
    TwType ControlTwType;
    ControlTwType = TwDefineEnum("ControlType", ControlTypeEV, 3);
    TwAddVarRW(myBar, "Control Type", ControlTwType, &(camera->controlType), NULL);




    //=============================================================================
    // Load world data.
    //=============================================================================

    vector<vec3> waysNodesPositions; // positions of nodes forming ways, possibly contains duplicates.
    vector<vec3> waysNodesColors;
    vector<vec2> roadsPositions;
    vector<vec2> buildingTrianglePositions;
    interpolateContours = false;
    TIME(loadXML("../data/srtm.xml"));
    TIME(loadXML("../data/result.xml"));
    TIME(extrudeAddrInterps());

    //TIME(fillBuffers(&waysNodesPositions, &waysNodesColors, &roadsPositions));
//    camera->translateTo(vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0.1));
    camera->translateTo(vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0) + vec3(coordinateInflationFactor / 10.f)*camera->frontVec);
    camera->lookAtTargetPos = vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0);
//    TIME(generateGridLines());
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
    glNamedBufferStorage(glidWaysPositions, nbWaysVertices * 3 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidWaysPositions, 0, nbWaysVertices * 3 * 4, waysNodesPositions.data());


    glCreateBuffers(1, &glidWaysColors);
    glNamedBufferStorage(glidWaysColors, nbWaysVertices * 3 * 4, // nbWaysNodes * vec3 * float
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
        in layout(location=0) vec3 pos;\n \
        in layout(location=1) vec3 color;\n \
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
    glVertexArrayVertexBuffer(pPipelineLines->glidVao, 0, glidWaysPositions, 0, 3*4);
    glVertexArrayAttribFormat(pPipelineLines->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
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
//    camera->moveFromUserInput(pWindow);
    camera->handlePresetNewFrame(pWindow);

    if(!fancyDisplayMode) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        // draw lines
        pPipelineLines->usePipeline();
        glProgramUniformMatrix4fv(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineLines->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
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
        glClear(GL_COLOR_BUFFER_BIT);
        pPipelineScreenQuad->usePipeline();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//        // draw buildings
        glBindTexture(GL_TEXTURE_2D, pBuildingTex->glidTexture);
        pPipelineBuildings->usePipeline();
        glProgramUniformMatrix4fv(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->mvpMat));
//        glProgramUniform1i(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineBuildings->getShader(GL_FRAGMENT_SHADER)->glidShaderProgram, "sampler"), 0);
        glDrawArrays(GL_TRIANGLES, 0, nbBuildingTriangles);
    }

    if(showTweakBar) TwDraw();

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
    if(!TwEventMouseWheelGLFW(yOffset)) {
        static const float factor = 1.1;
        if(yOffset < 0) {
            game->camera->multViewSizeBy(factor);
        } else {
            game->camera->multViewSizeBy(1.f/factor);
        }
    }
}

void Game::glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if(!TwEventKeyGLFW(key, action)) {
        if(action == GLFW_PRESS){
            if (key == GLFW_KEY_SPACE) { game->showTweakBar = !game->showTweakBar; }
            if (key == GLFW_KEY_ESCAPE) {
                if(game->camera->controlType == Camera::ControlType::QWEASDZXC_CONTINUOUS) game->camera->controlType = Camera::ControlType::QWEASDZXC_DRAG;
            }
        }
        game->camera->handlePresetKey(pWindow, key, scancode, action, mods);
    }
}

void Game::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{

    if(!TwEventMouseButtonGLFW(button, action)) {
        // Left-click
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS){
            double x,y;
            glfwGetCursorPos(pWindow, &x, &y);
            vec2 worldPos = game->windowPosToWorldPos(vec2(x,y));
            int index = 0;
            std::vector<Way*> closests;
            std::vector<double> distances;
            std::vector<vec2> directions;
            vector<std::pair<Node*, Node*>> nodePairs;
            int filter = OSMElement::CONTOUR;
            TIME(closests = game->findNClosestWays(2, worldPos, filter, distances, directions, nodePairs));

            if (closests[0] == NULL) return;
            TIME(game->updateSelectedWay(closests[0]));
            PRINTELEMENTVECTOR(closests);
        }
        //Right Click
        else if (button == GLFW_MOUSE_BUTTON_2){
//            if (action == GLFW_PRESS){
//                game->draggingCamera = true;
//                double x; double y;
//                glfwGetCursorPos(pWindow, &x, &y);
//                game->oldMousePosition = vec2(x,y);
//            }
//            else if (action == GLFW_RELEASE){
//                game->draggingCamera = false;
//            }
        }

        game->camera->handlePresetMouseButton(pWindow, button, action, mods);
    }
}

void Game::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
    if(!TwEventMousePosGLFW(x, y)) {
        if (game->draggingCamera){
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);
            vec2 offset = vec2(x,y) - game->oldMousePosition;
            offset.y *= -1; // reversed controls? this should be an option

            game->camera->translateBy(vec3(offset/1000.f,0));

            game->oldMousePosition = vec2(x,y);

        }
        game->camera->handlePresetCursorPos(pWindow, x, y);
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

            std::getline(ss, token, ','); viewRectBottom = atof(token.c_str()) * coordinateInflationFactor;
            std::getline(ss, token, ','); viewRectLeft = atof(token.c_str()) * coordinateInflationFactor;
            std::getline(ss, token, ','); viewRectTop = atof(token.c_str()) * coordinateInflationFactor;
            std::getline(ss, token, ','); viewRectRight = atof(token.c_str()) * coordinateInflationFactor;

            PRINT(viewRectBottom);
            PRINT(viewRectLeft);
            PRINT(viewRectTop);
            PRINT(viewRectRight);
            viewRectBottomLeft = vec2(viewRectLeft,viewRectBottom);
            viewRectTopRight = vec2(viewRectRight,viewRectTop);
            viewRectVecDiago = viewRectBottomLeft - viewRectTopRight;
        }
        if (string(child->Value()).compare("node") == 0){
//            cout << "Looking at a node \n";
//            cout << child->ToElement()->FindAttribute("id")->Value() << "\n";

            string id = child->ToElement()->FindAttribute("id")->Value();
            float latitude = atof(child->ToElement()->FindAttribute("lat")->Value()) * coordinateInflationFactor;
            float longitude = atof(child->ToElement()->FindAttribute("lon")->Value()) * coordinateInflationFactor;
            //double latitude = strtod(child->ToElement()->FindAttribute("lat")->Value(), NULL);
            //double longitude = strtod(child->ToElement()->FindAttribute("lon")->Value(), NULL);

            Node* node = new Node(id, longitude, latitude);
            node->elevation = 0;
//            if (path.compare("../data/result.xml") == 0) node->elevation = contourLineInterpolate(vec2(node->longitude, node->latitude));
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
            way->eType = OSMElement::NOT_IMPLEMENTED;
            for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement()){
                if (string(way_child->Value()).compare("nd") == 0){
                    string ref = way_child->ToElement()->FindAttribute("ref")->Value();
                    way -> addRef(theNodes[ref]);
                }
                else if (string(way_child->Value()).compare("tag") == 0){
                    string key = way_child->ToElement()->FindAttribute("k")->Value();
                    string value = way_child->ToElement()->FindAttribute("v")->Value();
                    way -> addTag(key, value);

                    OSMElement::ElementType eType = typeFromStrings(key, value);
                    if (eType != OSMElement::NOT_IMPLEMENTED) way->eType = eType;
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
                    string elementId = relation_child->ToElement()->FindAttribute("ref")->Value();

                    //Odd. Ucommenting this crashes the program when trying to fill the buffers.
                    if (type.compare("node") == 0) relation->addMember(theNodes[elementId]);
//                    else if (type.compare("way") == 0) relation->addMember(theWays[elementId]);
                    else if (type.compare("relation") == 0) relation->addMember(theRelations[elementId]);

                }
                else if (string(relation_child->Value()).compare("tag") == 0){
                    string key = relation_child->ToElement()->FindAttribute("k")->Value();
                    string value = relation_child->ToElement()->FindAttribute("v")->Value();
                    relation->addTag(key, value);
                }
                theRelations.emplace(id, relation);
            }

        }
    }
}

void Game::fillBuffers(vector<vec3> *waysNodesPositions,
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
        vec3 pt, oldpt, oldoldpt;
        vec2 point, oldPoint, oldOldPoint;
        vec3 color = vec3(glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f));
        Way* way = it->second;

        color = colorFromTags(way);
        if (color == vec3(-1,-1,-1)) continue;
        if (way->eType == OSMElement::ADDR_INTERPOLATION) continue;

        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt){
            Node* node = *nodeIt;
            point = vec2(node->longitude, node->latitude);
            pt = vec3(node->longitude, node->latitude, node->elevation);
            if (!first){
                waysNodesPositions->push_back(oldpt);
                waysNodesColors->push_back(color);
                waysNodesPositions->push_back(pt);
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

            oldoldpt = oldpt;
            oldpt = pt;
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
        if((way->eType==OSMElement::BUILDING_UNMARKED || way->eType==OSMElement::BUILDING_ADDRINTERP) && way->nodes.size() >= 3 && first == last) {

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
//                    vec2 p1 = vec2(n1->latitude, n1->longitude);
//                    vec2 p2 = vec2(n2->latitude, n2->longitude);
//                    vec2 p3 = vec2(n3->latitude, n3->longitude);
                    vec2 p1 = vec2(n1->longitude, n1->latitude);
                    vec2 p2 = vec2(n2->longitude, n2->latitude);
                    vec2 p3 = vec2(n3->longitude, n3->latitude);
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
                            //vec3 n_3D = vec3(node->latitude, node->longitude, 0);
                            vec3 n_3D = vec3(node->longitude, node->latitude, 0);
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
//                        vec2 p1 = vec2(n1->latitude, n1->longitude);
//                        vec2 p2 = vec2(n2->latitude, n2->longitude);
//                        vec2 p3 = vec2(n3->latitude, n3->longitude);
                        vec2 p1 = vec2(n1->longitude, n1->latitude);
                        vec2 p2 = vec2(n2->longitude, n2->latitude);
                        vec2 p3 = vec2(n3->longitude, n3->latitude);

                        vec2 point;
                        //point = (p1 - viewRectBottomLeft)/viewRectVecDiago;
                        point = p1;
//                        point = -2.0f*vec2(point.y, point.x) - vec2(1,1);
                        //buildingTrianglePositions->push_back(point);
                        tempTriangleVertices.push_back(point);
//                        point = (p2 - viewRectBottomLeft)/viewRectVecDiago;
                        point = p2;
//                        point = -2.0f*vec2(point.y, point.x) - vec2(1,1);
//                        buildingTrianglePositions->push_back(point);
                        tempTriangleVertices.push_back(point);
//                        point = (p3 - viewRectBottomLeft)/viewRectVecDiago;
                        point = p3;
//                        point = -2.0f*vec2(point.y, point.x) - vec2(1,1);
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

Node* Game::findClosestNode(vec2 xy){
    Node* closest;
    double bestDist = 99999;
    for ( auto it = theNodes.begin(); it != theNodes.end(); ++it ){
        Node* n = it->second;

        vec2 point = vec2(n->longitude, n->latitude);

        double dist = glm::distance(point, xy);

        if (dist < bestDist){
            closest = n;
            bestDist = dist;
        }
    }
    return closest;
}


std::vector<Way*> Game::findNClosestWays(int n, vec2 xy, int filter, vector<double> &distances, vector<vec2> &_directions, vector<std::pair<Node*, Node*>> &_nodePairs){
    Way* closest = NULL;
    std::vector<Way*> closests;
    std::vector<double> bestDists;
    std::vector<vec2> directions;
    std::pair<Node*, Node*> a;
    vector<std::pair<Node*, Node*>> nodePairs;
    for (int i = 0; i < n; i++){
        bestDists.push_back(99999);
        closests.push_back(NULL);
        directions.push_back(vec2(0,0));
        nodePairs.push_back(a);
    }

    //Look at all the ways
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;
//        if (!way->selectable) continue;

        // This way's type not yet implemented :( get to it!
        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

        // A specific filter has been chosen. Check if this way passes it!
        if (filter != OSMElement::ANY_TYPE)
            if ((way->eType & filter) == 0) continue;

        Node* firstNode;
        Node* secondNode;
        int counter = 1;
        bool first = true;
        vec2 p1, p2;
        double bestDist = 99999;
        vec2 direction;
        std::pair<Node*, Node*> aPair;

        //Look at all this way's nodes
        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt){

            // Keep first node and continue
            if (first){
                firstNode = *nodeIt;
                first = false;
                continue;
            }

            // Prepare next pair by putting the old second in the new first
            if (counter == 0) firstNode = secondNode;
            // Look at next pair by looking at new second node
            else{
                secondNode = *nodeIt;
                p1 = vec2(firstNode->longitude, firstNode->latitude);
                p2 = vec2(secondNode->longitude, secondNode->latitude);
                vec2 dir;
                double dist = pointLineSegmentDistance(xy, p2, p1, dir);

                if (dist < bestDist){
                    closest = way;
                    bestDist = dist;
                    direction = dir;
                    aPair = std::pair<Node*, Node*>(firstNode, secondNode);
                }
            }

            //flip-flop
            counter = (counter + 1) % 2;
        }
        //Now you're done looking at all the nodes.
        //Iterate over all current closest list, from closest to farthest
        for (int i = 0; i < n; i++){

            // Otherwise, we found a smaller distance (the way may have been seen or not)
            if (bestDist <= bestDists[i]){


                // Move everything behind this element farther by 1
                for (int j = n-1; j > i; j--){
                    bestDists[j] = bestDists[j-1];
                    closests[j] = closests[j-1];
                    directions[j] = directions[j-1];
                    nodePairs[j] = nodePairs[j-1];
                }

                // Keep the way and the distance.
                bestDists[i] = bestDist;
                closests[i] = way;
                directions[i] = direction;
                nodePairs[i] = aPair;
                break; //get out! you don't want to keep going in the array.

            }

        }

    }
    _directions = directions;
    distances = bestDists;
    _nodePairs = nodePairs;
    return closests;
}

vec2 Game::windowPosToWorldPos(vec2 ij){
    //if viewsize == x, you see between -x and x
    //Find position according to the window, between -1 and 1
    vec2 relativeToWindow;
    relativeToWindow.x = 2*(ij.x / windowWidth)-1;
    relativeToWindow.y = 2*(ij.y / windowHeight)-1;

    //Required because maps have y going up while the screen has y going down
    relativeToWindow.y *= -1;

    //World position is then the camera position + the relative
    //widnow position, scaled by the viewsize.
    vec2 worldPos;
    worldPos = vec2(camera->position.x,camera->position.y) + relativeToWindow * camera->viewSize;

    return worldPos;

}

double Game::pointLineSegmentDistance(vec2 p, vec2 v, vec2 w, vec2 &direction) {
  // Return minimum distance between line segment vw and point p
  const double l2 = glm::length(w-v) * glm::length(w-v);  // i.e. |w-v|^2 -  avoid a sqrt
  if (l2 == 0.0){
      direction = v - p;
      return glm::distance(p, v);   // v == w case
  }
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line.
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  const double t = glm::dot(p - v, w - v) / l2;
  if (t < 0.0){
      direction = v - p;
      return glm::distance(p, v); // Beyond the 'v' end of the segment
  }
  else if (t > 1.0){
      direction = w - p;
      return glm::distance(p, w);  // Beyond the 'w' end of the segment
  }
  const vec2 projection = v + (vec2(t,t) * (w - v));  // Projection falls on the segment
  direction = projection - p;
  return glm::distance(p, projection);
}


OSMElement::ElementType Game::typeFromStrings(string key, string value){

    if (key.compare("highway") == 0){
        if (value.compare("trunk") == 0) return OSMElement::HIGHWAY_TRUNK;
        else if (value.compare("primary") == 0) return OSMElement::HIGHWAY_PRIMARY;
        else if (value.compare("secondary") == 0) return OSMElement::HIGHWAY_SECONDARY;
        else if (value.compare("tertiary") == 0) return OSMElement::HIGHWAY_TERTIARY;
        else if (value.compare("residential") == 0) return OSMElement::HIGHWAY_RESIDENTIAL;
//        if (value.compare("service") == 0) return OSMElement::HIGHWAY_SERVICE;
    }
    else if (key.compare("natural") == 0){
        if (value.compare("wood") == 0) return OSMElement::NATURAL_WOOD;
    }
    else if (key.compare("building") == 0){
        if (value.compare("yes") == 0) return OSMElement::BUILDING_UNMARKED;
        else if (value.compare("school") == 0) return OSMElement::BUILDING_SCHOOL;
    }
    else if (key.compare("contour") == 0) return OSMElement::CONTOUR;
    else if (key.compare("leisure") == 0){
        if (value.compare("park") == 0) return OSMElement::LEISURE_PARK;
    }
    else if (key.compare("addr:interpolation") == 0){ return OSMElement::ADDR_INTERPOLATION; }
    return OSMElement::NOT_IMPLEMENTED;

}


void Game::updateSelectedWay(Way* myWay){ //or the highway

    if (selectedWay.way != NULL){
        vector<vec3> toWrite;

        for (int i = 0; i < (selectedWay.way->nodes.size() * 2) - 2; i++) toWrite.push_back(colorFromTags(selectedWay.way));

        // push it to the buffer
        glNamedBufferSubData(glidWaysColors, selectedWay.numberOfBytesBefore, selectedWay.numberOfBytesToWrite, toWrite.data());

    }

    // Figure out how many bytes into the buffer we have to start writing
    int numberOfBytesBefore = 0;
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;

//        if (!way->selectable) continue;
        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

        // If we havent yet reached the way we're looking for, add the current way's number of nodes to the offset
        // Each color was pushed twice per node, except for the first and last which accounted for 1 push (so * 2 - 2)
        if (way != myWay) numberOfBytesBefore += way->nodes.size() * 2 - 2;
        // we found our way! get out.
        else break;
    }
    //Offset is thus numberOfNodesBefore * sizeof(vec3) * sizeof(float)
    numberOfBytesBefore *= 3 * 4;

    // We want to write as many vec3's as there are nodes (-1) in the way
    int numberOfBytesToWrite = (myWay->nodes.size()*2 - 2 ) * 3 * 4;

    //Create the data to write
    vector<vec3> toWrite;
    for (int i = 0; i < (myWay->nodes.size() * 2) - 2; i++) toWrite.push_back(vec3(1.f,1.f,1.f));

    // push it to the buffer
    glNamedBufferSubData(glidWaysColors, numberOfBytesBefore, numberOfBytesToWrite, toWrite.data());

    selectedWay.way = myWay;
    selectedWay.numberOfBytesBefore = numberOfBytesBefore;
    selectedWay.numberOfBytesToWrite = numberOfBytesToWrite;
}

vec3 Game::colorFromTags(Way* way){
    if (way->eType == OSMElement::HIGHWAY_TRUNK) return vec3(1,1,1);
    else if (way->eType == OSMElement::HIGHWAY_PRIMARY) return vec3(0.8,0.8,0.8);
    else if (way->eType == OSMElement::HIGHWAY_SECONDARY) return vec3(0.7,0.7,0.7);
    else if (way->eType == OSMElement::HIGHWAY_TERTIARY) return vec3(0.6,0.6,0.6);
    else if (way->eType == OSMElement::HIGHWAY_RESIDENTIAL) return vec3(0.5,0.5,0.5);
    else if (way->eType == OSMElement::BUILDING_UNMARKED) return vec3(0,0,1);
    else if (way->eType == OSMElement::BUILDING_SCHOOL) return vec3(0,0,1);
    else if (way->eType == OSMElement::BUILDING_ADDRINTERP) return vec3(1,0,0);
    else if (way->eType == OSMElement::RAILWAY_SUBWAY) return vec3(1,0,1);
    else if (way->eType == OSMElement::NATURAL_WOOD) return vec3(0,0.5,0);
    else if (way->eType == OSMElement::LEISURE_PARK) return vec3(0,1,0);
    else if (way->eType == OSMElement::ADDR_INTERPOLATION) return vec3(0,0,0);
    else if (way->eType == OSMElement::CONTOUR) return vec3(0,0.5,0.5);
    else if (way->eType == OSMElement::GRID_LINE) return vec3(0.4,0.4,0.4);

    else return vec3(-1,-1,-1);
}
void Game::generateGridLines(){

    double step = 0.0005;

    int lonCounter = 0;
    int latCounter = 0;
    std::vector<Way*> latitudeLines;
    std::vector<Way*> longitudeLines;

    // Start by making horizontal lines
    for( double lon = viewRectLeft; lon <= viewRectRight; lon += step){
        Way* way = new Way();

        way->eType = OSMElement::GRID_LINE;
        latCounter = 0;

//        For each line generate a bunch of nodes
        for (double lat = viewRectBottom; lat <= viewRectTop; lat += step){

            Node* n = new Node();
            n->id = to_string(static_cast<long double>(lon)).append(",").append(to_string(static_cast<long double>(lat)));
            n->longitude = lon;
            n->latitude = lat;
            n->elevation = (contourLineInterpolate(vec2(n->longitude, n->latitude)));
//            n->elevation = 0;

            way->addRef(n);
            latCounter++;
        }
        way->id = string("LON").append(to_string(static_cast<long double>((way->nodes[0]->longitude))));
        longitudeLines.push_back(way);
        lonCounter++;
    }
    PRINTINT(unsuccessfulInterpolations);
    // Then, create as many vertical lines as there are nodes in the previous horizontal lines
    for (int i = 0; i < latCounter; i++){
        Way* way = new Way();
        way->eType = OSMElement::GRID_LINE;

        // For ith vertical line grab the ith nodes in each horizontal line previously created
        for (int j = 0; j < lonCounter; j++) way->addRef(longitudeLines[j]->nodes[i]);

        way->id = string("LAT").append(to_string(static_cast<long double>((way->nodes[0]->latitude))));
        latitudeLines.push_back(way);

    }

    // Add it all to the way database
    for (int i = 0; i < latitudeLines.size(); i++){
        theWays.emplace(latitudeLines[i]->id, latitudeLines[i]);
    }
    for (int i = 0; i < longitudeLines.size(); i++){
        theWays.emplace(longitudeLines[i]->id, longitudeLines[i]);
    }

}

inline double lerp(double a, double b, double t){ return a * t + b * (1 - t); }

double Game::contourLineInterpolate(vec2 xy){
    // Get closest contour lines to xy
    vector<double> distances;
    vector<vec2> directions;
    vector<std::pair<Node*, Node*>> nodePairs;
    vector<Way*> closests = findNClosestWays(5, xy, OSMElement::CONTOUR, distances, directions,nodePairs);

//    int lerpedWay = 1;
    int lerpedWay = getLerpedContourLines(xy, closests, directions, nodePairs);
    if (lerpedWay != -1) closests[1] = closests[lerpedWay];
    else{
        lerpedWay = 1;
//        return stod(closests[0]->getValue("ele"));
        unsuccessfulInterpolations++;
    }

    // prepare for lerping
    double sum = distances[0] + distances[lerpedWay];
    double t = 1 - distances[0] / sum;

    // get found contour lines elevations
    std::vector<double> heights;
    heights.push_back(stod(closests[0]->getValue("ele")));
    heights.push_back(stod(closests[lerpedWay]->getValue("ele")));

    return lerp(heights[0], heights[1], distances[0]) / 25000;
}


int Game::getLerpedContourLines(vec2 xy, vector<Way*> ways, vector<vec2> directions, vector<std::pair<Node*, Node*>> nodePairs){
//    Node* n0;
//    Node* n1;
//    bool first = true;
//    bool inside = true;
//    int counter = 1;
//    double initEle = stod(ways[0]->getValue("ele"));
//    for (auto nodeIt = ways[0]->nodes.begin() ; nodeIt != ways[0]->nodes.end(); ++nodeIt){
//        if (first){
//            n0 = *nodeIt;
//            first = false;
//            continue;
//        }
//        if ((counter % 2) == 0) n0 = n1;
//        else{
//            n1 = *nodeIt;
//            vec3 v0 = vec3(n1->latitude - n0->latitude, n1->longitude - n0 -> longitude,0);
//            vec3 v1 = vec3(n1->latitude - xy.x, n1->longitude - xy.y,0);
//            vec3 v2 = cross(v0, v1);

//            if (v2.z > 0){
//                inside = false;
//                break;
//            }
//        }

//        counter = (counter + 1) % 2;
//        }


//    for (int i = 1; i < ways.size(); i++){
//        double runningEle = stod(ways[i]->getValue("ele"));
//        if (inside && (runningEle > initEle)){ return i; }
//        else if(!inside && (runningEle < initEle)) return i;
//    }

    //Linesegment between our search point and the closest point on the candidate contour line.
    LineSegment L0;
    L0.p0 = xy;

//    PRINTVEC2VECTOR(directions);
    //Iterate on all candidate contours
    for (int i = 1; i < ways.size(); i++){

        L0.p1 = xy + directions[i];

        //running linesegment on the absolute known nearest contour
        LineSegment L1;

        // The running nodes forming segments on the known nearest contour
        Node* n0;
        Node* n1;

        // needed to start actually counting at 1
        int counter = 1;

        int numOfIntersections = 0;
        bool intersects = false;
        bool first = true;
        // Loop on all nodes on the best contour
        for (auto nodeIt = ways[0]->nodes.begin() ; nodeIt != ways[0]->nodes.end(); ++nodeIt){
            if (first){
                n0 = *nodeIt;
                first = false;
                continue;
            }

            if ((counter % 2) == 0) n0 = n1; //start a new linesegment
            else{
                n1 = *nodeIt;


                //lat long still flipped :(
                L1.p0 = vec2(n0->longitude, n0->latitude);
                L1.p1 = vec2(n1->longitude, n1->latitude);
//                PRINTELEMENT(L0);
//                PRINTELEMENT(L1);
                if (L0.intersects(L1)){
//                    PRINTSTRING("INTERSECTS!");
                    intersects = true;
                    numOfIntersections++;
//                    break; // we now know this way isnt viable as it intersected the nearest. we can stop looping around
                }
            }

            counter = (counter + 1) % 2;
        }
//        if (!intersects) return i;
        // an even number of intersections means we were inside the polygon. maybe?
        if ((numOfIntersections % 2) == 0) return i;
    }

    return -1; // method didnt really work .revert to default.
}

// Convert the address interpolation ways into building polygons
void Game::extrudeAddrInterps(){
    int counter = 0;

    // Iterate on all ways
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;

        // Keep only ADDR_INTERPOLATIONs
        if (way->eType != OSMElement::ADDR_INTERPOLATION) continue;

        vector<Node*> nodes;
        // Store this way's 2 nodes
        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt) nodes.push_back(*nodeIt);

        // We only want to look at roads
        int filter = OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_PRIMARY | OSMElement::HIGHWAY_TRUNK | OSMElement::HIGHWAY_SERVICE;

        // Preparing road query...
        vector<vector<vec2> > directions;
        directions.push_back(vector<vec2>());
        directions.push_back(vector<vec2>());

        vector<std::pair<Node*, Node*>> nodePairs;
        vector<vector<double> > distances;
        distances.push_back(vector<double>());
        distances.push_back(vector<double>());

        // Find the closest roads to this ADDR_INTERP
        vector<Way*> firstNodeWays = findNClosestWays(5, vec2(nodes[0]->longitude, nodes[0]->latitude), filter, distances[0], directions[0], nodePairs);
        vector<Way*> secondNodeWays = findNClosestWays(5, vec2(nodes[1]->longitude, nodes[1]->latitude), filter, distances[1], directions[1], nodePairs);

        // Find the closest way that both nodes share (should generally be the one parallel to the addr_interp)
        // Maybe better to look at dot products? This works ok for now
        pair<int, int> commonIndices = findCommonWay(firstNodeWays, secondNodeWays);

        int i = commonIndices.first;
        int j = commonIndices.second;

        // Multiplicative factor related to the polygon size
        double factor = 1;

        //If finding the common way didnt work just move on
        if (i == -1) continue;

        // Move on to the next if the common road is too far from one of the nodes
        if (distances[0][i] > 0.0002 * factor) continue;
        if (distances[1][j] > 0.0002 * factor) continue;

        // Move on to next if the nodes are on different sides of the common road
        if (dot(directions[0][i], directions[1][j]) < 0) continue;


        // Create the new nodes by moving from each edge of the ADDR_INTERP, perpendicularly to the road
        Node* node3 = new Node();
        node3->id = string("ADDR_INTERP_NODE3_").append(std::to_string(counter));
        node3->longitude = nodes[0]->longitude - directions[0][i].x * factor;
        node3->latitude = nodes[0] -> latitude - directions[0][i].y * factor;

        Node* node4 = new Node();
        node4->id = string("ADDR_INTERP_NODE4_").append(std::to_string(counter));
        node4->longitude = nodes[1]->longitude - directions[0][i].x * factor;
        node4->latitude = nodes[1]->latitude - directions[0][i].y * factor;

        // Create the new building
        Way* buildingWay  = new Way();
        buildingWay->addRef(nodes[1]);
        buildingWay->addRef(node4);
        buildingWay->addRef(node3);
        buildingWay->addRef(nodes[0]);
        buildingWay->addRef(nodes[1]);
        buildingWay->eType = OSMElement::BUILDING_ADDRINTERP;
        buildingWay->id = string("ADDR_INTERP_BUILDING").append(std::to_string(counter));

        // Store the new building
        theWays.emplace(buildingWay->id, buildingWay);
        counter++;
    }
}

pair<int, int> Game::findCommonWay(vector<Way*> firstNodeWays, vector<Way*> secondNodeWays){
    for (int i = 0; i < firstNodeWays.size(); i++){
        for (int j = 0; j < secondNodeWays.size(); j++){
            if (firstNodeWays[i]->id.compare(secondNodeWays[j]->id) == 0) return pair<int, int>(i,j);
        }
    }
    PRINTSTRING("Couldnt find suitable similar ways!");
    return pair<int, int>(-1,-1);
}
