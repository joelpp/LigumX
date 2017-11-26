#pragma once

#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "OSM_element.h"

#include "Logging.h"

//include only for gluint.
#include "Renderer.h"

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
	static void AddDebugModel(std::vector<glm::vec3>& line, glm::vec3 color);

	void addToTerrainBuffer(Sector* newSector);
    Mesh* terrainMesh();
	static void CreateWaysLines(Sector* sector);

    std::vector<TerrainRenderingJob> terrainRenderingJobs;
private:
	REGISTERCLASS(RenderDataManager);
    

};
