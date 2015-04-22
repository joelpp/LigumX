#ifndef TRIANGLE
#define TRIANGLE
#include "glm/glm.hpp"
using namespace glm;
class Triangle{
public:
    vec3 p0;
    vec3 p1;
    vec3 p2;
    vec3 normal;

    Triangle();
    Triangle(vec3 p0, vec3 p1, vec3 p2);
};

#endif // TRIANGLE

