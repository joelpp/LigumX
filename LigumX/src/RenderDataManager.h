#pragma once

#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "OSMElement.h"

#include "Logging.h"

#include "GL.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"

extern RenderDataManager* g_RenderDataManager;

class LigumX;
class Renderer;
class Sector;
class Mesh;

struct TerrainRenderingJob
{
    glm::vec2 start;
    float scale;
    GLuint buffer;
};

struct AABBJob
{
	AABB m_AABB;
	glm::vec3 m_Color;
};

class FlatWaysMesh : public Mesh
{
public:
	FlatWaysMesh(const std::vector<glm::vec3>& vertices, const std::vector<int>& typeBuffer, GLenum renderingMode, bool usePointRendering);

	std::vector<int> m_WayTypeBuffer;
	GLuint glidTypeBuffer;
	void CreateBuffers();

	FlatWaysGPUBuffers& GetGPUBuffers() { return m_FlatWaysGPUBuffers; }
private:
	FlatWaysGPUBuffers m_FlatWaysGPUBuffers;

};

class RenderDataManager{

public:
    std::unordered_map<OSMElement::ElementType, std::vector<glm::vec3> > waysNodesPositionsMap;

    std::vector<glm::vec2> screenQuadPos;
    std::vector<glm::vec2> screenQuadTexCoords;

    std::vector<glm::vec3> waysNodesPositions; // positions of nodes forming ways, possibly contains duplicates.
    std::vector<glm::vec3> waysNodesColors;
    std::vector<glm::vec3> roadsPositions;
    std::vector<glm::vec3> buildingTrianglePositions;
    std::vector<glm::vec3> buildingSides;
    std::vector<float> buildingLoopLengths;
    std::vector<glm::vec3> nodesPositions;
    std::vector<float> groundTriangleTextureIDs;
    std::vector<glm::vec3> groundTrianglesPositions;
    std::vector<glm::vec2> groundTrianglesUV;
    std::vector<glm::vec3> buildingLines;
    std::vector<float> buildingLinesTexCoords;
    Mesh* testMesh;
    
    float m_renderingScale;
    int nbRoads;
    RenderDataManager();

    void fillBuffers(Sector* newSector);
    static void InitializeSector(Sector* sector);
	static void AddDebugModel(const std::vector<glm::vec3>& line, glm::vec3 color);
	static Model* CreateDebugModel(const std::vector<glm::vec3>& line, glm::vec3 color, const char* name);
	static Model* CreateDebugModel(const std::vector<glm::vec3>& lineData, const std::vector<int>& indexBuffer, glm::vec3 color, const char* name);
	static Model* CreateFlatDebugModel(const std::vector<glm::vec3>& lineData, const std::vector<int>& typeBuffer, glm::vec3 color, const char* name);

	void addToTerrainBuffer(Sector* newSector);
    Mesh* terrainMesh();
	static void CreateWaysLines(Sector* sector);

    std::vector<TerrainRenderingJob> terrainRenderingJobs;

	void AddAABBJob(const AABB& aabb, const glm::vec3& color);

	std::vector<AABBJob>& GetAABBJobs();
	void ClearAABBJobs();

	std::vector<AABBJob> m_AABBJobs;
private:
	REGISTERCLASS(RenderDataManager);
    

};
