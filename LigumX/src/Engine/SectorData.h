#ifndef _SECTOR_DATA_
#define _SECTOR_DATA_

#include "LXSystem.h"

//#include "Math.h"
#include "OSMElement.h"

// class OSMElement;
class Way;
class Node;
class Relation;
class Heightfield;
class SectorGraphicalData;
class CurlRequest;

typedef std::unordered_map<OSMElement::ElementType, std::unordered_map<std::string, Way*> > WayTypeMap;

class SectorData 
{

public:

    enum EOSMDataType
    {
      CONTOUR,
      MAP
    };

	  glm::vec2 m_pos;

    std::unordered_map<std::string, Way*> ways;
    std::unordered_map<std::string, Way*> contourWays;
  	std::unordered_map<std::string, Node*> nodes;
	std::map<lxInt64, Node*> m_AllNodesPtr;

  	std::unordered_map<int, std::unordered_map<std::string, Way*> > waysMap;
	WayTypeMap m_wayTypeMap;

  	/**
  	 *  Constructor / Destructor
  	 */
  	SectorData();
	SectorData(glm::vec2 pos);

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

	SectorGraphicalData* GetGraphicalData() { return m_GraphicalData; };
	std::map<int, Way*> m_AllWaysPtr;

private:

	SectorGraphicalData* m_GraphicalData;

	CurlRequest* m_CurlRequest;

	REGISTERCLASS(SectorData);

};


#endif