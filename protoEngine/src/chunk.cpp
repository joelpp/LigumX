#include "chunk.h"

Chunk::Chunk(){

//    this->heightfield = new Heightfield();
}
Chunk::Chunk(glm::vec2 pos){
    this->position = pos;

    this->heightfield = new Heightfield(position, 0.05);
}
