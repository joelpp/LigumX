#include "game.h"
#include <vector>
#include <chrono>

using namespace std;

void Game::init()
{
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    glEnable(GL_POINT_SPRITE);

    //=============================================================================
    // Parameters, camera setup.
    //=============================================================================
    running = true;
//    windowWidth = 1600;
//    windowHeight = 1200;

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
    buildingHeight = 0.0001;
    buildingSideScaleFactor = 1;

    sunOrientation = 0;
    sunTime = 0;
    sunSpeed = 0.1;
    sunMoveAuto = false;

    saveScreenshot = false;

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



    init_tweakBar();





    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);



    //=============================================================================
    // Load world data.
    //=============================================================================

    vector<vec3> waysNodesPositions; // positions of nodes forming ways, possibly contains duplicates.
    vector<vec3> groundTrianglesPositions; // positions of nodes forming ways, possibly contains duplicates.

    vector<vec3> waysNodesColors;
    vector<vec2> roadsPositions;
    vector<vec2> buildingTrianglePositions;
    vector<vec2> buildingSides;
    vector<float> buildingLoopLengths;

    interpolateContours = false;
//    TIME(loadXML("../data/rouyntopo.xml"));
//    TIME(loadXML("../data/rouyn.xml"));

    TIME(loadXML("../data/srtm.xml"));
    TIME(loadXML("../data/result.xml"));


//    TIME(extrudeAddrInterps());

    PRINTINT(theWays.size());

    //TIME(fillBuffers(&waysNodesPositions, &waysNodesColors, &roadsPositions));
//    camera->translateTo(vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0.1));
    camera->translateTo(vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0) + 0.1f*camera->frontVec);
    camera->lookAtTargetPos = vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0);

    TIME(generateGridLines());

    TIME(fillBuffers(&waysNodesPositions, &waysNodesColors, &roadsPositions, &buildingTrianglePositions, &buildingSides, &buildingLoopLengths,&groundTrianglesPositions));


    //=============================================================================
    // Screen quad data.
    //=============================================================================

//    vec2 screenQuadPos[4] = {vec2(1,-1), vec2(1,1), vec2(-1,-1), vec2(-1,1)};
//    vec2 screenQuadTexCoords[4] = {vec2(1,0), vec2(1,1), vec2(0,0), vec2(0,1)};

    vector<vec2> screenQuadPos;
    screenQuadPos.push_back(vec2(1,-1));
    screenQuadPos.push_back(vec2(1,1));
    screenQuadPos.push_back(vec2(-1,-1));
    screenQuadPos.push_back(vec2(-1,1));

    vector<vec2> screenQuadTexCoords;
    screenQuadTexCoords.push_back(vec2(1,0));
    screenQuadTexCoords.push_back(vec2(1,1));
    screenQuadTexCoords.push_back(vec2(0,0));
    screenQuadTexCoords.push_back(vec2(0,1));
    //=============================================================================
    // create and fill VBOs.
    //=============================================================================

    nbWaysVertices = waysNodesPositions.size();
    nbGroundVertices = groundTrianglesPositions.size();
    nbBuildingTriangles = buildingTrianglePositions.size();
    nbBuildingLines = buildingSides.size();

    createGLBuffer(glidWaysPositions, waysNodesPositions);
    createGLBuffer(glidWaysColors, waysNodesColors);
    createGLBuffer(glidBufferRoadsPositions, roadsPositions);
    createGLBuffer(glidBufferBuildingTriangleVertices, buildingTrianglePositions);
    createGLBuffer(glidBufferBuildingLines, buildingSides);
    createGLBuffer(glidBufferBuildingLoopLengths, buildingLoopLengths);
    createGLBuffer(glidScreenQuadPositions, screenQuadPos);
    createGLBuffer(glidScreenQuadTexCoords, screenQuadTexCoords);
    createGLBuffer(glidGroundTrianglePositions, groundTrianglesPositions);

    //=============================================================================
    // Textures, framebuffer, renderbuffer
    //=============================================================================


    pBuildingTex = new Texture("../data/brickles.png");

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



    init_pipelines();

}








