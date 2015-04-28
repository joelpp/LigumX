#ifndef WORLD
#define WORLD


#include <unordered_map>
// #include <pair>

#include "heightfield.h"
#include "glm/glm.hpp"
// namespace std
// {
// template<>
// struct hash<glm::vec2> {
//     size_t operator()(const glm::vec2 &hf) const {
//         return std::hash<int>()(hf);
//     }
// };
// }

class World{
public:

	double chunkSize;
	std::unordered_map<int, Heightfield> heightfields;

	World();
};
#endif

// 45.5 -73.65 45.55 -73.6
// 45.55 -73.6 45.6 -73.55