#ifndef WORLD
#define WORLD

#include <iostream>
#include <unordered_map>
// #include <pair>
#include <sstream>
#include <string>
#include "chunk.h"
#include "glm/glm.hpp"
#include "tinyxml2.h"
#include "linesegment.h"


//Specialize std::hash for vec2s
namespace std {
    template <>
        class hash<glm::vec2>{
        public :
            size_t operator()(const glm::vec2 &vec ) const
            {
                return hash<int>()(vec.x) ^ hash<int>()(vec.y);
            }
    };
};

class World{
public:

    double chunkSize;
    std::unordered_map<glm::vec2, Chunk*> chunks;
    double coordinateInflationFactor;
    std::unordered_map<std::string, Node*>theNodes;
    std::unordered_map<std::string, Way*> theWays;
    std::unordered_map<std::string, Relation*> theRelations;
	World();
    Chunk* createChunk(glm::vec2 pos);
    void loadChunkData(std::string path);
    static OSMElement::ElementType typeFromStrings(std::string key, std::string value);
    std::pair<int, int> findCommonWay(std::vector<Way*> firstNodeWays, std::vector<Way*> secondNodeWays);
    std::vector<Way*> findNClosestWays(int n, glm::vec2 xy, int filter, std::vector<double> &distances, std::vector<glm::vec2> &_directions, std::vector<std::pair<Node*, Node*>> &_nodePairs);
    static double pointLineSegmentDistance(glm::vec2 p, glm::vec2 v, glm::vec2 w, glm::vec2 &direction);
    void extrudeAddrInterps();
};
#endif

// 45.5 -73.65 45.55 -73.6
// 45.55 -73.6 45.6 -73.55
