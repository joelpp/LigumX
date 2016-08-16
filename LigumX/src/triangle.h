#ifndef TRIANGLE
#define TRIANGLE
#include "glm/glm.hpp"

//using namespace glm;

class Triangle{
public:
    glm::vec3 vertices[3];
    glm::vec3 normal;

    float vertexIDs[3];

    Triangle();
    Triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);


};

#endif // TRIANGLE

