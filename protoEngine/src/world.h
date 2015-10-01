#ifndef WORLD
#define WORLD

#include <iostream>
#include <unordered_map>
// #include <pair>
#include <sstream>
#include <string>
#include "sector.h"
#include "glm/glm.hpp"
#include "tinyxml2.h"
#include <libspatialindex/capi/sidx_api.h>
#include <libspatialindex/capi/sidx_impl.h>
#include <libspatialindex/capi/sidx_config.h>

// #include "linesegment.h"
// 

/**
 *  Template for iterating over ways and nodes
 */
// for (auto it = waysTypeMap[OSMElement::HIGHWAY_RESIDENTIAL].begin(); it != waysTypeMap[OSMElement::HIGHWAY_RESIDENTIAL].end(); ++it){
//     Way* way = it->second;
//     int first = true;
//     int counter = 0;
//     Node *node0, *node1;
//     for (auto nodeIt = way->nodes.begin(); nodeIt != way->nodes.end();++it){
//         if (first){
//             node0 = *nodeIt;
//             first = false;
//         }

//         if ((counter % 2) == 0) node0 = node1;
//         else{
//             node1 = *nodeIt;
//         }


//         counter++;
//     }

// }

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

    World();
    
    /** sectors */
    Sector* createSector(glm::vec2 pos);
    void loadSectorData(std::string path);
    
    /** Query Methods (to be cleaned up) */
    std::pair<int, int> findCommonWay(std::vector<Way*> firstNodeWays, std::vector<Way*> secondNodeWays);
    std::vector<Way*> findNClosestWays(int n, glm::vec2 xy, int filter, std::vector<double> &distances, std::vector<glm::vec2> &_directions, std::vector<std::pair<Node*, Node*>> &_nodePairs);
    static double pointLineSegmentDistance(glm::vec2 p, glm::vec2 v, glm::vec2 w, glm::vec2 &direction);
    Node* findClosestNode(glm::vec2 xy);
    std::vector<Way*> getNearestWays(OSMElement::ElementType type,double lon,double lat,double maxResults);

    // World Data
    static OSMElement::ElementType typeFromStrings(std::string key, std::string value);    
    void extrudeAddrInterps();
    void addSidewalks();


    /** Spatial Index */
    Index* createIndex();
    void createSpatialIndexes();
    void addPoint(Index* idx, double lat, double lon, int64_t id);
    void addLineSegment(Index* idx, glm::vec2 p0, glm::vec2 p1, int64_t id);
    std::vector<SpatialIndex::IData*>* getNearest(Index* idx,double lat,double lon,double maxResults);
    std::vector<SpatialIndex::IData*>* boxQuery(Index* idx, glm::vec2 bottomLeft,glm::vec2 topRight, double maxResults);



    double sectorSize;
    std::unordered_map<glm::vec2, Sector*> sectors;
    double coordinateInflationFactor;
    std::unordered_map<std::string, Node*>theNodes;
    std::unordered_map<std::string, Way*> theWays;
    std::unordered_map<std::string, Relation*> theRelations;
    Index* spatialIndex;
    // Index* waysIndex;
    std::unordered_map<OSMElement::ElementType, Index*> waysIndex;
	std::unordered_map<OSMElement::ElementType, std::unordered_map<std::string, Way*> > waysTypeMap;

};
#endif

// 45.5 -73.65 45.55 -73.6
// 45.55 -73.6 45.6 -73.55
