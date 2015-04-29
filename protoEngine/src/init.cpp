#include "game.h"
#include <vector>
#include <chrono>

using namespace std;
using namespace glm;

static void test_error_cb (int error, const char *description)
    {
        fprintf(stderr, "%d: %s\n", error, description);
    }
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
    drawGround = false;
    saveScreenshot = false;
    populateTypeColorArray();
    //=============================================================================
    // create window and GLcontext, register callbacks.
    //=============================================================================

    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    glfwSetErrorCallback(test_error_cb);


    // Initialise GLFW
    if( !glfwInit() )
    {
        cerr << "Failed to initialize GLFW.\n";
        return;
    } else {
        clog << "Initialized GLFW." << endl;
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

    world = new World();
//    world->addChunk(vec2(45.5, -73.65));
    world->addChunk(vec2(-0.65000,0.50000));
    world->addChunk(vec2(-0.60000,0.50000));
    world->addChunk(vec2(-0.65000,0.55000));
    world->addChunk(vec2(-0.60000,0.55000));

    vector<vec3> waysNodesPositions; // positions of nodes forming ways, possibly contains duplicates.
    vector<vec3> waysNodesColors;
    vector<vec3> roadsPositions;
    vector<vec3> buildingTrianglePositions;
    vector<vec3> buildingSides;
    vector<float> buildingLoopLengths;

    interpolateContours = false;
//    TIME(loadXML("../data/rouyntopo.xml"));
//    TIME(loadXML("../data/rouyn.xml"));

#ifdef __APPLE__
    TIME(loadXML("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/srtm.xml"));
    TIME(loadXML("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/result.xml"));
#else
//    loadXML("../data/srtm.xml");
//    TIME(loadXML("../data/result.xml"));
    loadXML("../data/0.xml");
//    loadXML("../data/1.xml");
//    loadXML("../data/2.xml");
//    loadXML("../data/3.xml");
#endif

   // TIME(extrudeAddrInterps());

    PRINTINT(theWays.size());

    //TIME(fillBuffers(&waysNodesPositions, &waysNodesColors, &roadsPositions));
//    camera->translateTo(vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0.1));
    camera->translateTo(vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0) + 0.1f*camera->frontVec);
    camera->lookAtTargetPos = vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0);

//    TIME(generateGridLines(&groundTrianglesPositions, &groundTrianglesUV));


    TIME(fillBuffers(&waysNodesPositions, &waysNodesColors, &roadsPositions, &buildingTrianglePositions, &buildingSides, &buildingLoopLengths,&groundTrianglesPositions, &groundTrianglesUV));


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

//    for (int i = 0; i < buildingSides.size(); i++){
//        PRINTVEC3(buildingSides[i]);
//    }

    createGLBuffer(glidWaysPositions, waysNodesPositions);
    createGLBuffer(glidWaysColors, waysNodesColors);
    createGLBuffer(glidBufferRoadsPositions, roadsPositions);
    createGLBuffer(glidBufferBuildingTriangleVertices, buildingTrianglePositions);
    createGLBuffer(glidBufferBuildingLines, buildingSides);
    createGLBuffer(glidBufferBuildingLoopLengths, buildingLoopLengths);
    createGLBuffer(glidScreenQuadPositions, screenQuadPos);
    createGLBuffer(glidScreenQuadTexCoords, screenQuadTexCoords);
    createGLBuffer(glidGroundTrianglePositions, groundTrianglesPositions);
    createGLBuffer(glidGroundTriangleUVs, groundTrianglesUV);
    // PRINTVEC3VECTOR(groundTrianglesPositions);

    for ( auto it = waysNodesPositionsMap.begin(); it != waysNodesPositionsMap.end(); ++it ){
        createGLBuffer(glidWaysNodesPositions[it->first], it->second);
    }
    //=============================================================================
    // Textures, framebuffer, renderbuffer
    //=============================================================================

#ifdef __APPLE__
    textureMap.emplace("bricks", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/brickles.png"));
    textureMap.emplace("grass", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/grass.png"));
    textureMap.emplace("rock", new Texture("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/rock.png"));
#else
    textureMap.emplace("bricks", new Texture("../data/brickles.png"));
    textureMap.emplace("grass", new Texture("../data/grass.png"));
    textureMap.emplace("rock", new Texture("../data/rock.png"));
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



    init_pipelines();

    //Freetype stuff (shouldnt stay here)
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    else
        cout << "Loaded Freetype font! yayy" << "\n";
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
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

#ifndef __APPLE__
    entityManager.Init();
#endif  
    inEntityLand = false;
}


void Game::fillBuffers(vector<vec3> *waysNodesPositions,
                       vector<vec3> *waysNodesColors,
                       vector<vec3> *roadsPositions,
                       vector<vec3> *buildingTrianglePositions,
                       vector<vec3> *buildingLines,
                       vector<float> *buildingLinesTexCoords,
                       vector<vec3> *groundTrianglesPositions,
                       vector<vec2> *groundTrianglesUV){
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
        vec3 point, oldPoint, oldOldPoint;
        vec3 color = vec3(glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f));
        Way* way = it->second;
        try{
            color = typeColorMap[way->eType];
        }
        catch(...){
            continue;
        }

        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt){
            Node* node = *nodeIt;
            point = vec3(node->longitude, node->latitude, node->elevation);
            pt = vec3(node->longitude, node->latitude, node->elevation);
            if (!first){

                if (way->eType == OSMElement::GRID_LINE){ continue; }

                else{
                    waysNodesPositions->push_back(oldpt);
                    waysNodesColors->push_back(color);
                    waysNodesPositions->push_back(pt);
                    waysNodesColors->push_back(color);

                    waysNodesPositionsMap[way->eType].push_back(oldpt);
                    waysNodesPositionsMap[way->eType].push_back(pt);
                }
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
            vector<vec3> tempTriangleVertices;
            bool failedLoop;
            for(int clockwiseness=-1; clockwiseness<=1; clockwiseness += 2) {

                // copy node loop (not repeating the last node) and
                // add lines to buildingLines
                vector<Node*> loopNodes;
                float distance = 0;
                for(auto n = way->nodes.begin(); n != way->nodes.end()-1; ++n) {
                    loopNodes.push_back(*n);
                    buildingLines->push_back(vec3((*n)->longitude, (*n)->latitude, (*n)->elevation));
                    buildingLines->push_back(vec3((*(n+1))->longitude, (*(n+1))->latitude, (*(n+1))->elevation));

//                    buildingLines->push_back(vec3((*n)->longitude, (*n)->latitude, 0));
//                    buildingLines->push_back(vec3((*(n+1))->longitude, (*(n+1))->latitude, 0));


                    buildingLinesTexCoords->push_back(float(distance));
                    distance += glm::distance(vec2((*n)->longitude, (*n)->latitude),
                                              vec2((*(n+1))->longitude, (*(n+1))->latitude));
                    buildingLinesTexCoords->push_back(float(distance));
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
                        vec3 p1 = vec3(n1->longitude, n1->latitude,0);
                        vec3 p2 = vec3(n2->longitude, n2->latitude,0);
                        vec3 p3 = vec3(n3->longitude, n3->latitude,0);

                        vec3 point;
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
                for(vec3 v : tempTriangleVertices) {
                    buildingTrianglePositions->push_back(v);
                }
            }
        }

    }

    cout << "succeeded loops: " << nbSuccessLoops << endl;
    cout << "failed loops: " << nbFailedLoops << endl;
    int counter = 0;
    for (auto it = world->chunks.begin(); it != world->chunks.end(); ++it){
        Heightfield* heightField = it->second->heightfield;
        vector<Triangle* >& tris = heightField->triangles;
        for (int i = 0; i < tris.size()-1; i += 2){
            Triangle* tri = tris[i];
            if (tri != NULL){
                groundTrianglesPositions->push_back(tri->p0);
                groundTrianglesPositions->push_back(tri->p1);
                groundTrianglesPositions->push_back(tri->p2);
                groundTrianglesUV->push_back(vec2(0,0));
                groundTrianglesUV->push_back(vec2(1,0));
                groundTrianglesUV->push_back(vec2(0,1));
            }

            tri = tris[i+1];
            if (tri != NULL){
                groundTrianglesPositions->push_back(tri->p0);
                groundTrianglesPositions->push_back(tri->p1);
                groundTrianglesPositions->push_back(tri->p2);
                groundTrianglesUV->push_back(vec2(1,0));
                groundTrianglesUV->push_back(vec2(1,1));
                groundTrianglesUV->push_back(vec2(0,1));
            }
        }
    }
}







