#pragma once

#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "OSM_element.h"

#include "Logging.h"

class LigumX;
class Renderer;
class Sector;
class Mesh;
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
    void initializeSector(Sector* sector);

	void addToTerrainBuffer(Sector* newSector);

private:
	REGISTERCLASS(RenderDataManager);

};
