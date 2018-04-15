#include "stdafx.h"

#include "Building.h"
#include "Way.h"
#include "Model.h"
#include "OSMElement.h"
#include "Node.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Material.h"
#include "LigumX.h"
#include "LXError.h"

using namespace glm;
using namespace std;

Building::Building(Way* way) 
	: m_MinCoords(glm::vec3(std::numeric_limits<float>::max()))
	, m_MaxCoords(glm::vec3(-std::numeric_limits<float>::max()))
{
	m_Way = way;
}


bool Building::GenerateModel()
{

	Way* way = m_Way;
	auto first = m_Way->GetNodes().front();
	auto last = m_Way->GetNodes().back();
	int LiftableWalls = OSMElement::BUILDING_UNMARKED;
	int nbFailedLoops = 0;
	int nbSuccessLoops = 0;

	std::vector<glm::vec3> buildingLines;
	std::vector<float> buildingLinesTexCoords;
	std::vector<float> groundTriangleTextureIDs;
	std::vector<glm::vec3> buildingTrianglePositions;


	float height = 0;
	if (way->eType == OSMElement::BUILDING_UNMARKED) 
	{
		height = 10;//game.buildingHeight /* + (rand() / RAND_MAX) * 4.f*/;
	}
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
	        
	    for(auto n = way->GetNodes().begin(); n != way->GetNodes().end()-1; ++n)
	    {
	        loopNodes.push_back(*n);
	        
	        if (way->eType == LiftableWalls)
	        {
	            buildingLines.push_back((*n)->GetWorldPosition());
	            buildingLines.push_back((*n+1)->GetWorldPosition());
	        }

	        if (way->eType == LiftableWalls)
	        {
	            buildingLinesTexCoords.push_back(float(distance));    
	        } 

	        distance += glm::distance(vec2((*n)->GetWorldPosition()),
	                                  vec2((*n + 1)->GetWorldPosition()));

	        if (way->eType == LiftableWalls)
	        {
	        	buildingLinesTexCoords.push_back(float(distance));	
	        } 
	    }

	    unsigned int nbLoops = 0;

	    // recursively transform loop into triangles until fully consumed.
	    auto nodeIt1 = loopNodes.begin();
	    auto nodeIt2 = loopNodes.begin() + 1;
	    auto nodeIt3 = loopNodes.begin() + 2;

	    failedLoop = false;
	    do 
	    {
	        if(nbLoops++ > 2 * loopNodes.size()) 
	        {
	//                    cout << "missed triangle." << endl;
	            failedLoop = true;
	            break;
	        }
	        Node* n1 = *(nodeIt1);
	        Node* n2 = *(nodeIt2);
	        Node* n3 = *(nodeIt3);

	        vec2 p1 = vec2(n1->GetWorldPosition());
	        vec2 p2 = vec2(n2->GetWorldPosition());
	        vec2 p3 = vec2(n3->GetWorldPosition());
			if (p1 == p2 || p1 == p3)
			{
				cout << "bad precision" << std::endl;
				
				return false;
			}
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
	            for(Node* node : loopNodes) 
	            {
	                if(node == n1 || node == n2 || node == n3) continue;
	                //vec3 n_3D = vec3(node->latitude, node->longitude, 0);
					vec3 n_3D = node->GetWorldPosition();
	                if(clockwiseness * glm::cross(v12_3D, n_3D - vec3(p1.x, p1.y, 0)).z > 0.f &&
	                   clockwiseness * glm::cross(v23_3D, n_3D - vec3(p2.x, p2.y, 0)).z > 0.f &&
	                   clockwiseness * glm::cross(v31_3D, n_3D - vec3(p3.x, p3.y, 0)).z > 0.f ) 
	                {
	                    isGoodTriangle = false;
	                    break;
	                }
	            }
	    	}
	    	if(isGoodTriangle) 
	    	{
	        	// create triangle
	            vec3 p1 = n1->GetWorldPosition();
	            vec3 p2 = n2->GetWorldPosition();
	            vec3 p3 = n3->GetWorldPosition();

	            vec3 point;
	            point = p1;
	            tempTriangleVertices.push_back(point);
	            point = p2;
	            tempTriangleVertices.push_back(point);
	            point = p3;
	            tempTriangleVertices.push_back(point);

	            // delete node from loop. std::vector reassigns positions to all elements after the one deleted, so we must reassign the iterators acordingly.
	            if(nodeIt2 == loopNodes.begin()) 
	            {
	                loopNodes.erase(nodeIt2);
	                nodeIt3 = loopNodes.begin();
	                nodeIt2 = loopNodes.end() - 1;
	                nodeIt1 = nodeIt2 - 1;
	            } 
	            else 
	            {
	                loopNodes.erase(nodeIt2);
	                nodeIt2 = nodeIt1 + 1;
	                if(nodeIt2 == loopNodes.end()) nodeIt2 = loopNodes.begin();
	                nodeIt3 = nodeIt2 + 1;
	                if(nodeIt3 == loopNodes.end()) nodeIt3 = loopNodes.begin();
	            }
	        	nbLoops = 0;
	    	} 
	        else 
	        {
	            // no triangle, check next
	            nodeIt1 = nodeIt2;
	            nodeIt2 = nodeIt3;
	            ++nodeIt3;
	            if(nodeIt3 == loopNodes.end()) nodeIt3 = loopNodes.begin();
	        }

		} 
	    while(loopNodes.size() >= 3);

	    if(!failedLoop) 
	    {
	        break; // do not check other clockwiseness, the first guess worked.
	    } 
		else 
	    {
	        tempTriangleVertices.clear();
	    }
	}

	if (failedLoop)
	{
		return false;
	}

	// keep and copy the triangles we created
	for (vec3& v : tempTriangleVertices)
	{
		v.z = height;
		buildingTrianglePositions.push_back(v);

		m_MaxCoords = glm::max(v, m_MaxCoords);
		m_MinCoords = glm::min(v, m_MinCoords);
	}

	lxAssert((tempTriangleVertices.size() % 3) == 0);
	for (int i = 0; i < (tempTriangleVertices.size() / 3); ++i)
	{
		m_Triangles.push_back(Triangle({ tempTriangleVertices[3 * i + 0], 
							             tempTriangleVertices[3 * i + 1],
							             tempTriangleVertices[3 * i + 2] }));
	}

	Renderer& renderer = Renderer::GetInstance();

	m_Model = new Model();
	m_Model->SetName("Building");

	Mesh* mesh = new Mesh(buildingTrianglePositions, GL::PrimitiveMode::Triangles);

	glm::vec3 color = renderer.typeColorMap[m_Way->eType];

	Material* material = new Material(renderer.pPipelineBasic, color);
	m_Model->addMesh(mesh, material);
    
    return true;
}


