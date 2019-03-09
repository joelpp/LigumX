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
#include "EngineSettings.h"

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
	OSMElementType LiftableWalls = OSMElementType::OSMElementType_Building_Unmarked;
	int nbFailedLoops = 0;
	int nbSuccessLoops = 0;

	std::vector<glm::vec3> buildingLines;
	std::vector<float> buildingLinesTexCoords;
	std::vector<float> groundTriangleTextureIDs;
	std::vector<glm::vec3> buildingTrianglePositions;

	// Note: supposed all loops are given in counter-clockwise order.
	unsigned int nbTriangles = 0;

	// first try clockwise loops, then try counterclockwise if it failed.
	std::vector<vec3> tempTriangleVertices;
	m_FlatVertices.clear();

	bool failedLoop;
	for(int clockwiseness=-1; clockwiseness<=1; clockwiseness += 2) 
	{

	    // copy node loop (not repeating the last node) and
	    // add lines to buildingLines
	    std::vector<Node*> loopNodes;
	    float distance = 0;
	        
	    for(auto n = way->GetNodes().begin(); n != way->GetNodes().end()-1; ++n)
	    {
			if (way->GetNodes().size() <= 3)
			{
				continue;
			}
	        loopNodes.push_back(*n);
	        
	        if (way->GetOSMElementType() == LiftableWalls)
	        {
	            buildingLines.push_back((*n)->GetWorldPosition());
	            buildingLines.push_back((*n+1)->GetWorldPosition());
	        }

	        if (way->GetOSMElementType() == LiftableWalls)
	        {
	            buildingLinesTexCoords.push_back(float(distance));    
	        } 

	        distance += glm::distance(vec2((*n)->GetWorldPosition()),
	                                  vec2((*n + 1)->GetWorldPosition()));

	        if (way->GetOSMElementType() == LiftableWalls)
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

	        const vec3& p1 = n1->GetWorldPosition();
	        const vec3& p2 = n2->GetWorldPosition();
	        const vec3& p3 = n3->GetWorldPosition();
			if (p1 == p2 || p1 == p3)
			{
				cout << "bad precision" << std::endl;
				
				return false;
			}
	        vec3 v12 = p2 - p1;
	        vec3 v31 = p3 - p1;

	        vec3 v23 = p3 - p2;
	        vec3 v13 = p1 - p3;

	        bool isGoodTriangle;
	        // check clockwiseness
	        isGoodTriangle = clockwiseness * glm::cross(v12, v31).z > 0.f;
	        if(isGoodTriangle) 
	        {
	            // make sure it doesn't include another active node
	            for(Node* node : loopNodes) 
	            {
	                if(node == n1 || node == n2 || node == n3) continue;

					vec3 n_3D = node->GetWorldPosition();

	                if(clockwiseness * glm::cross(v12, n_3D - p1).z > 0.f &&
	                   clockwiseness * glm::cross(v23, n_3D - p2).z > 0.f &&
	                   clockwiseness * glm::cross(v13, n_3D - p3).z > 0.f )
	                {
	                    isGoodTriangle = false;
	                    break;
	                }
	            }
	    	}
	    	if(isGoodTriangle) 
	    	{
	        	// create triangle
	            const vec3& p1 = n1->GetWorldPosition();
	            const vec3& p2 = n2->GetWorldPosition();
	            const vec3& p3 = n3->GetWorldPosition();

				vec3 point = p1;
	            tempTriangleVertices.push_back(point);
	            point = p2;
	            tempTriangleVertices.push_back(point);
	            point = p3;
	            tempTriangleVertices.push_back(point);
				m_FlatVertices.push_back(p1);
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
			m_FlatVertices.clear();
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

	CPUBuffers cpuBuffers;
	cpuBuffers.m_VertexPositions = buildingTrianglePositions;

	glm::vec3 scale = m_MaxCoords - m_MinCoords;
	for (int k = 0; k < cpuBuffers.m_VertexPositions.size(); k++)
	{
		const glm::vec3& point = cpuBuffers.m_VertexPositions[k];
		glm::vec2 uv = glm::vec2((point - m_MinCoords) / scale);

		uv /= g_EngineSettings->GetExtent();

		cpuBuffers.m_vertexUVs.push_back(uv);
	}

	Mesh* mesh = new Mesh(cpuBuffers, GL::PrimitiveMode::Triangles, false);

	glm::vec3 color = glm::vec3(39, 181, 51) / 255.f;

	Material* material = new Material(renderer.pPipelineBasic, color);
	m_Model->addMesh(mesh, material);
    
    return true;
}


