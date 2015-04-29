#ifndef WORLD
#define WORLD


#include <unordered_map>
// #include <pair>

#include "chunk.h"
#include "glm/glm.hpp"

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

	World();
    void addChunk(glm::vec2 pos);
};
#endif

// 45.5 -73.65 45.55 -73.6
// 45.55 -73.6 45.6 -73.55
