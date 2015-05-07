#ifndef CHUNK
#define CHUNK

#include "glm/glm.hpp"
#include "heightfield.h"
#include "osm_element.h"
#include "node.h"
#include "way.h"
#include "relation.h"

class Chunk{
public:
    glm::vec2 position;
    Heightfield* heightfield;
    std::unordered_map<std::string, Way*> ways;
  	std::unordered_map<std::string, Node*> nodes;
  	std::unordered_map<int, std::unordered_map<std::string, Way*> > waysMap;
  	
    Chunk();
    Chunk(glm::vec2 pos);
    void heightfieldTesting();

    
};

#endif // CHUNK

