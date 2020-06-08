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

#pragma region  FORWARD_DECLARATIONS RenderDataManager
#include "LXSystem.h"
class Serializer2;

class RenderDataManager;
class CullingOptions;
class RenderingStats;


#pragma endregion  FORWARD_DECLARATIONS RenderDataManager
extern RenderDataManager* g_RenderDataManager;

class Camera;
class World;
class LigumX;
class Renderer;
class Sector;
class Mesh;

const int m_MaxWays = 512;

class GfxWayVAO
{
public:
	bool m_Initialized = false;

	GLuint m_TranslationVBO;
	GLuint m_NodePositionsVBO;
	GLuint m_GLid;
};

struct LXWayDesc
{
	lx3F32 m_Translation;
	std::vector<lx3F32> m_NodeModelPositions;
};

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

struct S2DMessage
{
	glm::vec3 m_Color;
	std::string m_Message;
	glm::ivec2 m_ScreenPosition;
};

struct TimedMessage
{
	TimedMessage(const std::string& message, const glm::vec2& position, float scale, float timer)
	{
		m_Message = message;
		m_Timer = timer;
		m_Position = position;
		m_Scale = scale;
	}

	std::string m_Message;
	glm::vec2 m_Position;
	float m_Scale;
	float m_Timer;
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

class ColoredPointsMesh : public Mesh
{
public:
	ColoredPointsMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& colorBuffer, GL::PrimitiveMode primitiveMode, bool usePointRendering);

	std::vector<glm::vec3> m_ColorBuffer;

	GLuint glidTypeBuffer;
	void CreateBuffers();

	ColoredPointsGPUBuffers& GetGPUBuffers() { return m_FlatWaysGPUBuffers; }
private:
	ColoredPointsGPUBuffers m_FlatWaysGPUBuffers;

};

#pragma region  HEADER RenderDataManager
class RenderDataManager : public LXObject
{
public:
static const int ClassID = -1412519292;
static const LXType Type = LXType_RenderDataManager;
static constexpr const char* ClassName = "RenderDataManager";
virtual LXType GetLXType() { return LXType_RenderDataManager; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

CullingOptions*& GetCullingOptions() { return m_CullingOptions; }; 
void SetCullingOptions(CullingOptions* value) { m_CullingOptions = value; }; 
RenderingStats*& GetRenderingStats() { return m_RenderingStats; }; 
void SetRenderingStats(RenderingStats* value) { m_RenderingStats = value; }; 
private:
CullingOptions* m_CullingOptions = nullptr;
RenderingStats* m_RenderingStats = nullptr;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_RenderDataManagerPIDX
{
PIDX_CullingOptions,
PIDX_RenderingStats,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER RenderDataManager

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

	void AddEntityAABB(Entity* entity);
	void AddAABBJob(const AABB& aabb, const glm::vec3& color);
	void AddAABBJob(const glm::vec3& worldPosition, int brushWidth, const glm::vec3& color);
	void AddAABBJobCentered(const glm::vec3& worldPosition, int brushWidth, const glm::vec3& color);

	void AddTimedMessage(const std::string& message, const glm::vec2& position, float scale, float time);
	void AddTimedMessage(const std::string& message);

	void Add2DMessage(const std::string& message, const glm::ivec2& screenPosition, const glm::vec3& color);
	void Add2DMessage(const std::string& message, const glm::ivec2& screenPosition);
	void AddMouseMessage(const std::string& message);

	std::vector<TimedMessage>& GetTimedMessages() { return m_TimedMessages; }
	std::vector<S2DMessage>& Get2DMessages() { return m_2DMessages; }

	void Update();


	bool IsAABBVisible(Entity* e, const std::vector<glm::vec3>& vertices, Camera* camera);
	bool IsAABBVisible(Entity* e, const AABB& aabb, const glm::mat4& toWorldMatrix, Camera* camera);
	bool IsSectorVisible(Sector* sector, Camera* camera);


	void GatherVisibleEntities(const std::vector<Entity*>& entities, Camera* camera);
	void GatherVisibleEntities(World* world, Camera* camera);
	std::vector<Entity*>& GetEntityAABBJobs();
	std::vector<AABBJob>& GetAABBJobs();
	void ClearAABBJobs();

	const std::vector<Entity*>& GetVisibleEntities() { return m_VisibleEntities; }
	const std::vector<Sector*>& GetVisibleSectors() { return m_VisibleSectors; }


	std::vector<LXWayDesc> m_LXWayDescs;
	GfxWayVAO m_GfxWayVAO;
	bool m_WaysDirty = false;

private:
	std::vector<S2DMessage> m_2DMessages;
	std::vector<TimedMessage> m_TimedMessages;
	std::vector<AABBJob> m_AABBJobs;
	std::vector<Entity*> m_EntityAABBJobs;
	int m_NumMouseMessages = 0;


	std::vector<Entity*> m_VisibleEntities;
	const int m_MaxVisibleEntities = 32768;

	std::vector<Sector*> m_VisibleSectors;
	const int m_MaxVisibleSectors = 4096;

};
