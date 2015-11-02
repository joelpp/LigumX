#ifndef _SECTOR_DATA_
#define _SECTOR_DATA_

#include <sstream>
#include <unordered_map>

#include "glm/glm.hpp"
#include "tinyxml2.h"

#include "Math.h"
#include "Logging.h"
#include <string>
#include "OSM_Element.h"
// class OSMElement;
class Way;
class Node;
class Relation;
class Heightfield;

typedef std::unordered_map<OSMElement::ElementType, std::unordered_map<std::string, Way*> > WayTypeMap;

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
// 
// 
class SectorData{
public:

    enum EOSMDataType
    {
      CONTOUR,
      MAP
    };

	  glm::vec2 m_pos;

    std::unordered_map<std::string, Way*> ways;
  	std::unordered_map<std::string, Node*> nodes;
  	std::unordered_map<int, std::unordered_map<std::string, Way*> > waysMap;
	  WayTypeMap m_wayTypeMap;

  	/**
  	 *  Constructor / Destructor
  	 */
  	SectorData();
  	SectorData(glm::vec2 pos);

  	void loadData(EOSMDataType dataType);

  	/**
  	 * [BuildXMLPath Build the OSM data path.]
  	 * @param  pos [The position of the sector (lon-lat)]
  	 * @return     [The string representing the path to the xml file]
  	 */
  	std::string BuildXMLPath(EOSMDataType dataType, glm::vec2 pos);

  	/**
  	 * [BuildXMLPath Build the OSM data path.]
  	 * @param  pos [The position of the sector (lon-lat)]
  	 * @return     [The string representing the path to the xml file]
  	 */
  	std::vector<Way*> findNClosestWays(int n, 
  											  glm::vec2 xy, 
  											  int filter, 
  											  std::vector<double> &distances, 
  											  std::vector<glm::vec2> &_directions, 
  											  std::vector<std::pair<Node*, Node*>> &_nodePairs);
    
    Node* findClosestNode(glm::vec2 xy);
    void elevateNodes(Heightfield* heightfield);

    void downloadData(std::string path);
    //TODO: Implement this (in the right place)
    // std::vector<Way*> getNearestWays(OSMElement::ElementType type,double lon,double lat,double maxResults);
private:

  REGISTERCLASS(SectorData);

};


#endif