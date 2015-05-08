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
    //=============================================================================
    // Parameters, camera setup.
    //=============================================================================
    running = true;

    camera = new Camera();
    camera->setViewSize(0.03);
    draggingCamera = false;

    selectedWay.way = NULL;
    unsuccessfulInterpolations = 0;
    buildingHeight = 3;
    buildingSideScaleFactor = 1;

    sunOrientation = 0;
    sunTime = 0;
    sunSpeed = 0.1;
    sunMoveAuto = false;
    drawGround = false;
    saveScreenshot = false;
    renderText = false;

    populateTypeColorArray();

    showTweakBar = false;

    srand(987654321);

    init_tweakBar();

    // register GLFW and GLdebug callbacks
    glfwSetMouseButtonCallback( renderer.pWindow, glfwMouseButtonCallback );
    glfwSetKeyCallback( renderer.pWindow, glfwKeyCallback );
    glfwSetCharCallback( renderer.pWindow, glfwCharCallback );
    glfwSetCursorPosCallback( renderer.pWindow, glfwMousePositionCallback );
    glfwSetCursorEnterCallback( renderer.pWindow, glfwMouseEntersCallback );
    glfwSetScrollCallback( renderer.pWindow, glfwMouseScrollCallback );
    glfwSetWindowPosCallback( renderer.pWindow, glfwWindowPositionCallback );
    glfwSetWindowSizeCallback( renderer.pWindow, glfwWindowSizeCallback );
    glfwSetWindowCloseCallback( renderer.pWindow, glfwWindowClosedCallback );
    glfwSetWindowRefreshCallback( renderer.pWindow, glfwWindowRefreshCallback );
    glfwSetWindowFocusCallback( renderer.pWindow, glfwWindowFocusCallback );
    glfwSetWindowIconifyCallback( renderer.pWindow, glfwWindowIconifyCallback );
    glfwSetFramebufferSizeCallback( renderer.pWindow, glfwWindowFramebufferSizeCallback );

//    glDebugMessageCallback(Game::debugCallback, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    //=============================================================================
    // Load world data.
    //=============================================================================
    viewRectBottom =  0.5;
    viewRectLeft =  -0.65;
    viewRectTop = 0.55;
    viewRectRight = -0.6;

    viewRectBottomLeft = vec2(viewRectLeft,viewRectBottom);
    viewRectTopRight = vec2(viewRectRight,viewRectTop);
    viewRectVecDiago = viewRectBottomLeft - viewRectTopRight;

    camera->translateTo(vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0) + 0.1f*camera->frontVec);
    camera->lookAtTargetPos = vec3(viewRectBottomLeft + (viewRectTopRight - viewRectBottomLeft)/2.f,0);

    world = new World();
    world->createChunk(vec2(-73.650, 45.500));
    // world->createChunk(vec2(-73.650, 45.510));
    // world->createChunk(vec2(-73.650, 45.520));
    // world->createChunk(vec2(-73.64, 45.500));
    // world->createChunk(vec2(-73.64, 45.510));
    // world->createChunk(vec2(-73.64, 45.520));
    // world->createChunk(vec2(-73.63, 45.500));
    // world->createChunk(vec2(-73.63, 45.510));
    // world->createChunk(vec2(-73.63, 45.520));

    vector<vec3> waysNodesPositions; // positions of nodes forming ways, possibly contains duplicates.
    vector<vec3> waysNodesColors;
    vector<vec3> roadsPositions;
    vector<vec3> buildingTrianglePositions;
    vector<vec3> buildingSides;
    vector<float> buildingLoopLengths;
    vector<float> groundTriangleTextureIDs;

    TIME(fillBuffers(&waysNodesPositions, &waysNodesColors, &roadsPositions, &buildingTrianglePositions, &buildingSides, &buildingLoopLengths,&groundTrianglesPositions, &groundTrianglesUV, &groundTriangleTextureIDs));


    //=============================================================================
    // Screen quad data.
    //=============================================================================

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

    renderer.nbWaysVertices = waysNodesPositions.size();
    renderer.nbGroundVertices = groundTrianglesPositions.size();
    renderer.nbBuildingTriangles = buildingTrianglePositions.size();
    renderer.nbBuildingLines = buildingSides.size();


    renderer.createGLBuffer(renderer.glidWaysPositions, waysNodesPositions);
    renderer.createGLBuffer(renderer.glidWaysColors, waysNodesColors);
    renderer.createGLBuffer(renderer.glidBufferRoadsPositions, roadsPositions);
    renderer.createGLBuffer(renderer.glidBufferBuildingTriangleVertices, buildingTrianglePositions);
    renderer.createGLBuffer(renderer.glidBufferBuildingLines, buildingSides);
    renderer.createGLBuffer(renderer.glidBufferBuildingLoopLengths, buildingLoopLengths);
    renderer.createGLBuffer(renderer.glidScreenQuadPositions, screenQuadPos);
    renderer.createGLBuffer(renderer.glidScreenQuadTexCoords, screenQuadTexCoords);
    renderer.createGLBuffer(renderer.glidGroundTrianglePositions, groundTrianglesPositions);
    renderer.createGLBuffer(renderer.glidGroundTriangleUVs, groundTrianglesUV);
    renderer.createGLBuffer(renderer.glidGroundTriangleTextureIDs, groundTriangleTextureIDs);
//     PRINTVEC3VECTOR(groundTrianglesPositions);

    for ( auto it = waysNodesPositionsMap.begin(); it != waysNodesPositionsMap.end(); ++it ){
        renderer.createGLBuffer(renderer.glidWaysNodesPositions[it->first], it->second);
        renderer.numberOfVerticesToDrawPerElement[it->first] = it->second.size();
    }


    //=============================================================================
    // Textures, framebuffer, renderbuffer
    //=============================================================================
    renderer.init_pipelines();

//    FT_Library ft;
//    if (FT_Init_FreeType(&ft))
//        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

//    FT_Face face;
//#ifdef __APPLE__
//    if (FT_New_Face(ft, "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/fonts/arial.ttf",0,&face))
//#else
//    if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face))
//#endif
//        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//    else
//        cout << "Loaded Freetype font! yayy" << "\n";
//    FT_Set_Pixel_Sizes(face, 0, 48);
//    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
//        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

//    for (GLubyte c = 0; c < 128; c++)
//    {
//        // Load character glyph
//        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//        {
//            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//            continue;
//        }
//        // Generate texture
//        GLuint texture;
//        glGenTextures(1, &texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(
//            GL_TEXTURE_2D,
//            0,
//            GL_RED,
//            face->glyph->bitmap.width,
//            face->glyph->bitmap.rows,
//            0,
//            GL_RED,
//            GL_UNSIGNED_BYTE,
//            face->glyph->bitmap.buffer
//        );
//        // Set texture options
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        // Now store character for later use
//        Character character = {
//            texture,
//            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//#ifdef __APPLE__
//            static_cast<GLuint>(face->glyph->advance.x)
//#else
//            face->glyph->advance.x
//#endif
//        };
//        Characters.insert(std::pair<GLchar, Character>(c, character));
//    }
//    FT_Done_Face(face);
//    FT_Done_FreeType(ft);


//#ifndef __APPLE__
//    entityManager.Init();
//#endif
//    inEntityLand = false;
//    int TOTAL = 0;

    // This was experimental to up performance when rendering text. It sucks actually.
//    int filter = OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_UNCLASSIFIED;
//    for (auto it = theWays.begin(); it != theWays.end(); ++it){
//        Way* way = it->second;

//        if ((way->eType & filter) == 0) continue;
////        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

//        // Store the info we want
//        std::string text;
//        try{text = way->tags.at("name");}
//        catch(...){continue;}

//        float scale = 0.000001f;
//        //Get the text position
//        vec3 xyz = (way->nodes[0]->getLatLongEle() + way->nodes[1]->getLatLongEle()) / vec3(2,2,2);

//        // Store each character's quad vertices
//        std::vector<characterQuadVertices> quads;
//        std::string::const_iterator c;
//        float x = xyz.x;
//        float y = xyz.y;
//        for (c = text.begin(); c != text.end(); c++)
//        {
//            Character ch = Characters[*c];

//            GLfloat xpos = x + ch.Bearing.x * scale;
//            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

//            GLfloat w = ch.Size.x * scale;
//            GLfloat h = ch.Size.y * scale;
//            GLfloat vertices[6][3] = {
//                { xpos,     ypos + h,  0.0001 },
//                { xpos,     ypos,      0.0001 },
//                { xpos + w, ypos,      0.0001 },

//                { xpos,     ypos + h,  0.0001 },
//                { xpos + w, ypos,      0.0001 },
//                { xpos + w, ypos + h,  0.0001 }
//            };
//            characterQuadVertices v(vertices);
//            quads.push_back(v);
//            TOTAL++;
//            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
//        }

//        // Store the info
//        Text t = {
//            text,
//            xyz,
//            true,
//            scale,
//            quads
//        };
//        texts.push_back(t);
//    }
//    PRINT(TOTAL);
}

void Game::init_freetype(){

}

void Game::fillBuffers(vector<vec3> *waysNodesPositions,
                       vector<vec3> *waysNodesColors,
                       vector<vec3> *roadsPositions,
                       vector<vec3> *buildingTrianglePositions,
                       vector<vec3> *buildingLines,
                       vector<float> *buildingLinesTexCoords,
                       vector<vec3> *groundTrianglesPositions,
                       vector<vec2> *groundTrianglesUV,
                       vector<float> *groundTriangleTextureIDs){
    unsigned int nbRoads;
    bool first;
    bool second;
    vec3 white = vec3(1.0f,1.0f,1.0f);
    unsigned int nbSuccessLoops = 0;
    unsigned int nbFailedLoops = 0;

    int LiftableWalls = OSMElement::BUILDING_UNMARKED;
    
    static unsigned int nbBuildings = 0;

    nbRoads = 0;
    GLint firstVertexForThisRoad = 0;

    for (auto chunkIterator = world->chunks.begin();
         chunkIterator != world->chunks.end();
         ++chunkIterator){

        Chunk *chunk = chunkIterator->second;

        for ( auto it = chunk->ways.begin(); it != chunk->ways.end(); ++it ){
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
                color = renderer.typeColorMap[way->eType];
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

                renderer.firstVertexForEachRoad.push_back(firstVertexForThisRoad);
                renderer.nbVerticesForEachRoad.push_back(nbVertexForThisRoad);

                // set first vertex index for next road
                firstVertexForThisRoad += nbVertexForThisRoad;
            }

            // triangulate building loops
            Node first(*(way->nodes.front()));
            Node last(*(way->nodes.back()));
            if((way->eType==OSMElement::BUILDING_UNMARKED||way->eType == OSMElement::LEISURE_PARK)&&way->nodes.size() >= 3 && first == last) {

                float height = 0;
                if (way->eType == OSMElement::BUILDING_UNMARKED) height = buildingHeight /* + (rand() / RAND_MAX) * 4.f*/;
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
                        
                        if (way->eType == LiftableWalls){
                            buildingLines->push_back(vec3((*n)->longitude, (*n)->latitude, (*n)->elevation));
                            buildingLines->push_back(vec3((*(n+1))->longitude, (*(n+1))->latitude, (*(n+1))->elevation));
                        }
    //                    buildingLines->push_back(vec3((*n)->longitude, (*n)->latitude, 0));
    //                    buildingLines->push_back(vec3((*(n+1))->longitude, (*(n+1))->latitude, 0));


                        if (way->eType == LiftableWalls) buildingLinesTexCoords->push_back(float(distance));
                        distance += glm::distance(vec2((*n)->longitude, (*n)->latitude),
                                                  vec2((*(n+1))->longitude, (*(n+1))->latitude));
                        if (way->eType == LiftableWalls) buildingLinesTexCoords->push_back(float(distance));
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
                    int ID = 0;

                    if (way->eType == OSMElement::LEISURE_PARK){ ID = 1.f; }
                    else if (way->eType == OSMElement::BUILDING_UNMARKED || 
                        way->eType == OSMElement::BUILDING_SCHOOL ){ ID = 2.f; }
                    // keep and copy the triangles we created
                    for(vec3 v : tempTriangleVertices) {
                        v.z = height;
                        buildingTrianglePositions->push_back(v);
                        groundTriangleTextureIDs->push_back(ID);
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
                    groundTrianglesPositions->push_back(tri->vertices[0]);
                    groundTrianglesPositions->push_back(tri->vertices[1]);
                    groundTrianglesPositions->push_back(tri->vertices[2]);
                    groundTrianglesUV->push_back(vec2(0,0));
                    groundTrianglesUV->push_back(vec2(1,0));
                    groundTrianglesUV->push_back(vec2(0,1));
                    // groundTrianglesUV->push_back(vec2(0.3, 0.76));
                    // groundTrianglesUV->push_back(vec2(0.53, 0.76));
                    // groundTrianglesUV->push_back(vec2(0.3,0.99));
                }

                tri = tris[i+1];
                if (tri != NULL){
                    groundTrianglesPositions->push_back(tri->vertices[0]);
                    groundTrianglesPositions->push_back(tri->vertices[1]);
                    groundTrianglesPositions->push_back(tri->vertices[2]);
                    groundTrianglesUV->push_back(vec2(1,0));
                    groundTrianglesUV->push_back(vec2(1,1));
                    groundTrianglesUV->push_back(vec2(0,1));
                    // PRINT(tri->vertexIDs[0]);
                    // groundTrianglesUV->push_back(vec2(0.53,0.76));
                    // groundTrianglesUV->push_back(vec2(0.53, 0.99));
                    // groundTrianglesUV->push_back(vec2(0.3,0.99));
                }
            }
        }
    }
    renderer.nbRoads = nbRoads;
}







