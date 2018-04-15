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

class Camera;
class World;
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

struct WayData
{
	int type;
	int IndexInSector;
};

struct TimedMessage
{
	TimedMessage(const std::string& message, int timer)
	{
		m_Message = message;
		m_Timer = timer;
	}

	std::string m_Message;
	int m_Timer;
};


class FlatWaysMesh : public Mesh
{
public:
	FlatWaysMesh(const std::vector<glm::vec3>& vertices, const std::vector<WayData>& typeBuffer, GL::PrimitiveMode primitiveMode, bool usePointRendering);

	std::vector<int> m_WayTypeBuffer;
	std::vector<WayData> m_DataBuffer;

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
	static Model* CreateFlatDebugModel(const std::vector<glm::vec3>& lineData, const std::vector<WayData>& typeBuffer, glm::vec3 color, const char* name);

	void addToTerrainBuffer(Sector* newSector);
    Mesh* terrainMesh();
	void CreateWaysLines(Sector* sector);

    std::vector<TerrainRenderingJob> terrainRenderingJobs;

	void AddAABBJob(const AABB& aabb, const glm::vec3& color);
	void AddAABBJob(const glm::vec3& worldPosition, int brushWidth, const glm::vec3& color);
	void AddAABBJobCentered(const glm::vec3& worldPosition, int brushWidth, const glm::vec3& color);

	void AddTimedMessage(const std::string& message, int numFrames);
	void AddTimedMessage(const std::string& message);
	std::vector<TimedMessage>& GetTimedMessages() { return m_TimedMessages; }

	void Update();


	bool IsAABBVisible(const AABB& aabb, Camera* camera);
	bool IsSectorVisible(Sector* sector, Camera* camera);


	void GatherVisibleEntities(const std::vector<Entity*>& entities, Camera* camera);
	void GatherVisibleEntities(World* world, Camera* camera);
	std::vector<AABBJob>& GetAABBJobs();
	void ClearAABBJobs();

	int GetNumVisibleSectors() { return m_NumVisibleSectors; }
	int GetNumVisibleEntities() { return m_NumVisibleEntities; }
	const std::vector<Entity*>& GetVisibleEntities() { return m_VisibleEntities; }
	const std::vector<Sector*>& GetVisibleSectors() { return m_VisibleSectors; }

private:
	REGISTERCLASS(RenderDataManager);
    
	std::vector<TimedMessage> m_TimedMessages;
	std::vector<AABBJob> m_AABBJobs;

	std::vector<Entity*> m_VisibleEntities;
	const int m_MaxVisibleEntities = 2048;

	std::vector<Sector*> m_VisibleSectors;
	const int m_MaxVisibleSectors = 2048;


	int m_NumVisibleEntities;
	int m_NumVisibleSectors;
};
