#ifndef CHUNK
#define CHUNK

#include "glm/glm.hpp"
#include "heightfield.h"
class Chunk{
public:
    glm::vec2 position;
    Heightfield* heightfield;

    Chunk();
    Chunk(glm::vec2 pos);
};

#endif // CHUNK

