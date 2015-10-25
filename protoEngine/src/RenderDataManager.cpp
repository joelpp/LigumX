#include "RenderDataManager.h"
#include "LigumX.h"
#include "Sector.h"
#include "Renderer.h"
#include "Way.h"
#include "Node.h"
#include "Triangle.h"
#include "World.h"
#include "SectorData.h"
#include "Heightfield.h"
#include "Logging.h"
#include "Mesh.h"
#include "Model.h"
#include "glm/gtx/transform.hpp"

using namespace std;
using namespace glm;

int findSetBit(int number){
    unsigned i = 1, pos = 1;
 
    // Iterate through bits of n till we find a set bit
    // i&n will be non-zero only when 'i' and 'n' have a set bit
    // at same position
    while (!(i & number))
    {
        // Unset current bit and set the next bit in 'i'
        i = i << 1;
 
        // increment position
        ++pos;
    }
 
    return pos;
}   

RenderDataManager::RenderDataManager(){
    nbRoads = 0;
    Renderer& renderer = Renderer::GetInstance();

    screenQuadPos.push_back(vec2(1,-1));
    screenQuadPos.push_back(vec2(1,1));
    screenQuadPos.push_back(vec2(-1,-1));
    screenQuadPos.push_back(vec2(-1,1));

    screenQuadTexCoords.push_back(vec2(1,0));
    screenQuadTexCoords.push_back(vec2(1,1));
    screenQuadTexCoords.push_back(vec2(0,0));
    screenQuadTexCoords.push_back(vec2(0,1));

    Renderer::createGLBuffer(renderer.glidScreenQuadPositions, screenQuadPos);
    Renderer::createGLBuffer(renderer.glidScreenQuadTexCoords, screenQuadTexCoords);


    Model* model = new Model("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/Models/Sphere/sphere.obj");
    testMesh = model->m_meshes[0];

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-0.6, 0.55, 0));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0),  0.f, glm::vec3(0, 0, 1));
    glm::mat4 scaleMatrix = glm::scale(0.01f, 0.01f, 0.01f);
    model->m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    testMesh->createBuffers();
    testMesh->m_usesIndexBuffer = false;
    testMesh->m_renderingMode = GL_TRIANGLES;
    Material material;
    material.albedo = glm::vec3(1,1,1);
    material.shader = renderer.pPipelineBasic;
    model->m_materialList.push_back( material ) ;
    renderer.m_debugModels.push_back( model );
}

void RenderDataManager::addToTerrainBuffer(Sector* newSector)
{
    LigumX& game = LigumX::GetInstance();
    Renderer& renderer = Renderer::GetInstance();
    int counter = 0;

    Heightfield* heightField = newSector->m_heightfield;
    testMesh = heightField->m_mesh;

    testMesh->createBuffers();
    testMesh->m_usesIndexBuffer = true;
    testMesh->m_renderingMode = GL_TRIANGLES;
    // renderer.m_debugMeshes.push_back( testMesh );

}

void RenderDataManager::fillBuffers(Sector* sector)
{
    LigumX& game = LigumX::GetInstance();
    Renderer& renderer = Renderer::GetInstance();

    bool first;
    bool second;
    vec3 white = vec3(1.0f,1.0f,1.0f);
    unsigned int nbSuccessLoops = 0;
    unsigned int nbFailedLoops = 0;
    int LiftableWalls = OSMElement::BUILDING_UNMARKED;
    unsigned int nbBuildings = 0;

    GLint firstVertexForThisRoad = 0;


    Mesh* waysMesh = new Mesh();
    waysMesh->m_renderingMode = GL_LINES;
    std::vector<glm::vec3>& vertexPositions = waysMesh->m_buffers.vertexPositions;

    for ( auto it = sector->m_data->ways.begin(); it != sector->m_data->ways.end(); ++it )
    {
        first = true;
        second = false;
        GLsizei nbVertexForThisRoad = 0;
        vec3 pt, oldpt, oldoldpt;
        vec3 point, oldPoint, oldOldPoint;
        vec3 color = vec3(glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f),
                          glm::linearRand(0.5f, 1.0f));
        Way* way = it->second;
        try
        {
            color = renderer.typeColorMap[way->eType];
        }
        catch(...)
        {
            continue;
        }

        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt)
        {
            
            Node* node = *nodeIt;
            point = vec3(node->longitude, node->latitude, node->elevation);
            pt = vec3(node->longitude, node->latitude, node->elevation);
            nodesPositions.push_back(pt);
            if (!first)
            {

                if (way->eType == OSMElement::GRID_LINE)
                {
                    continue; 
                }

                else
                {
                    waysNodesPositions.push_back(oldpt);
                    waysNodesColors.push_back(color);
                    waysNodesPositions.push_back(pt);
                    waysNodesColors.push_back(color);

                    waysNodesPositionsMap[way->eType].push_back(oldpt);
                    waysNodesPositionsMap[way->eType].push_back(pt);

                }

                // create roads subgroup
                if (way->hasTag("highway")) 
                {
                    if(second) 
                    {
                        second = false;
                        // first point for line adjacency, fake straight line
                        roadsPositions.push_back(oldPoint - (point-oldPoint));
                        roadsPositions.push_back(oldPoint);
                        nbVertexForThisRoad += 2;
                    }
                    roadsPositions.push_back(point);
                    ++nbVertexForThisRoad;
                }
            }
            else
            {
                first = false;
                //if (way->hasTagAndValue("highway", "residential")) {
                if (way->hasTag("highway")) 
                {
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
        if (way->hasTag("highway")) 
        {
            roadsPositions.push_back(point + (point-oldOldPoint));
            ++nbVertexForThisRoad;
            ++nbRoads;

            renderer.firstVertexForEachRoad.push_back(firstVertexForThisRoad);
            renderer.nbVerticesForEachRoad.push_back(nbVertexForThisRoad);

            // set first vertex index for next road
            firstVertexForThisRoad += nbVertexForThisRoad;
        }

        // triangulate building loops
        // PRINTELEMENTPTR(way);
        Node first(*(way->nodes.front()));
        Node last(*(way->nodes.back()));
        if((way->eType==OSMElement::BUILDING_UNMARKED||way->eType == OSMElement::LEISURE_PARK)&&way->nodes.size() >= 3 && first == last) 
        {

            float height = 0;
            if (way->eType == OSMElement::BUILDING_UNMARKED) height = game.buildingHeight /* + (rand() / RAND_MAX) * 4.f*/;
//            cout << "building loop" << endl;

            // Note: supposed all loops are given in counter-clockwise order.
            unsigned int nbTriangles = 0;

            // first try clockwise loops, then try counterclockwise if it failed.
            std::vector<vec3> tempTriangleVertices;
            bool failedLoop;
            for(int clockwiseness=-1; clockwiseness<=1; clockwiseness += 2) 
            {

                // copy node loop (not repeating the last node) and
                // add lines to buildingLines
                std::vector<Node*> loopNodes;
                float distance = 0;
                
                for(auto n = way->nodes.begin(); n != way->nodes.end()-1; ++n) 
                {
                    loopNodes.push_back(*n);
                    
                    if (way->eType == LiftableWalls)
                    {
                        buildingLines.push_back(vec3((*n)->longitude, (*n)->latitude, (*n)->elevation));
                        buildingLines.push_back(vec3((*(n+1))->longitude, (*(n+1))->latitude, (*(n+1))->elevation));
                    }

                    if (way->eType == LiftableWalls)
                    {
                        buildingLinesTexCoords.push_back(float(distance));    
                    } 

                    distance += glm::distance(vec2((*n)->longitude, (*n)->latitude),
                                              vec2((*(n+1))->longitude, (*(n+1))->latitude));

                    if (way->eType == LiftableWalls) buildingLinesTexCoords.push_back(float(distance));
                }

                unsigned int nbLoops = 0;

                // recursively transform loop into triangles until fully consumed.
                auto nodeIt1 = loopNodes.begin();
                auto nodeIt2 = loopNodes.begin() + 1;
                auto nodeIt3 = loopNodes.begin() + 2;
                failedLoop = false;
                do 
                {
                    if(nbLoops++ > 2*loopNodes.size()) 
                    {
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
                    if(isGoodTriangle) 
                    {
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
                    if(isGoodTriangle) 
                    {
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
                        if(nodeIt2 == loopNodes.begin()) 
                        {
                            loopNodes.erase(nodeIt2);
                            nodeIt3 = loopNodes.begin();
                            nodeIt2 = loopNodes.end() - 1;
                            nodeIt1 = nodeIt2 - 1;
                        } else 
                        {
                            loopNodes.erase(nodeIt2);
                            nodeIt2 = nodeIt1 + 1;
                            if(nodeIt2 == loopNodes.end()) nodeIt2 = loopNodes.begin();
                            nodeIt3 = nodeIt2 + 1;
                            if(nodeIt3 == loopNodes.end()) nodeIt3 = loopNodes.begin();
                        }
                        nbLoops = 0;
                    } else 
                    {
                        // no triangle, check next
                        nodeIt1 = nodeIt2;
                        nodeIt2 = nodeIt3;
                        ++nodeIt3;
                        if(nodeIt3 == loopNodes.end()) nodeIt3 = loopNodes.begin();
                    }

                } while(loopNodes.size() >= 3);
                if(!failedLoop) 
                {
                    break; // do not check other clockwiseness, the first guess worked.
                } else 
                {
                    tempTriangleVertices.clear();
                }
            }
            if(failedLoop) 
            {
                nbFailedLoops++;
            } 
            else 
            {
                nbSuccessLoops++;
                int ID = 0;

                if (way->eType == OSMElement::LEISURE_PARK){ ID = 1.f; }
                else if (way->eType == OSMElement::BUILDING_UNMARKED || 
                    way->eType == OSMElement::BUILDING_SCHOOL ){ ID = 2.f; }
                // keep and copy the triangles we created
                for(vec3 v : tempTriangleVertices) {
                    v.z = height;
                    buildingTrianglePositions.push_back(v);
                    groundTriangleTextureIDs.push_back(ID);
                }



            }
        }

    

    }
    renderer.nbWaysVertices = waysNodesPositions.size();
    renderer.nbBuildingTriangles = buildingTrianglePositions.size();
    renderer.nbBuildingLines = buildingSides.size();
    renderer.nbNodes = nodesPositions.size();
    renderer.nbRoads = nbRoads;

    Renderer::createGLBuffer(renderer.glidNodesPositions, nodesPositions);
    Renderer::createGLBuffer(renderer.glidWaysPositions, waysNodesPositions);
    Renderer::createGLBuffer(renderer.glidWaysColors, waysNodesColors);
    Renderer::createGLBuffer(renderer.glidBufferRoadsPositions, roadsPositions);
    Renderer::createGLBuffer(renderer.glidBufferBuildingTriangleVertices, buildingTrianglePositions);
    Renderer::createGLBuffer(renderer.glidBufferBuildingLines, buildingSides);
    Renderer::createGLBuffer(renderer.glidBufferBuildingLoopLengths, buildingLoopLengths);

    Model* waysModel = new Model();

    for (auto it = waysNodesPositionsMap.begin(); it != waysNodesPositionsMap.end(); ++it)
    {
        Mesh* mesh = new Mesh();

        OSMElement::ElementType index = it->first;

        mesh->m_buffers.vertexPositions = it->second;

        mesh->padBuffer(VERTEX_UVS);
        mesh->createBuffers();
        mesh->m_renderingMode = GL_LINES;

        Material material;
        material.shader = renderer.pPipelineLines;
        material.albedo = renderer.typeColorMap[index];
        waysModel->m_materialList.push_back( material ) ;

        waysModel->m_meshes.push_back(mesh);
    }

    renderer.m_debugModels.push_back(waysModel);
}


