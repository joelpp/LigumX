#include "world.h"

World::World(){
	chunkSize = 0.05;
}

void World::addChunk(glm::vec2 pos){
    chunks.emplace(pos, new Chunk(pos));
}
