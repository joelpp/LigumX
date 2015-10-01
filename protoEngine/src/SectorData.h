#ifndef _SECTOR_DATA_
#define _SECTOR_DATA_

#include "glm/glm.hpp"
#include "osm_element.h"
#include "node.h"
#include "way.h"
#include "relation.h"
#include "tinyxml2.h"
#include <sstream>

class SectorData{
public:
    std::unordered_map<std::string, Way*> ways;
  	std::unordered_map<std::string, Node*> nodes;
  	std::unordered_map<int, std::unordered_map<std::string, Way*> > waysMap;

  	SectorData();
  	SectorData(glm::float2 pos);
  	std::string BuildXMLPath(glm::float2 pos);
};


#endif