#ifndef HEIGHTFIELD
#define HEIGHTFIELD

#include "triangle.h"
#include <vector>


class Heightfield{
public:
    std::vector<Triangle* > triangles;
    double sideLength;
    glm::vec2 startPoint;

    Heightfield();
    Heightfield(glm::vec2 startPoint,double sideLength);
    void generateTriangles();
    void addTriangle(Triangle *tri);
    std::vector<std::vector<glm::vec3> > points;
};

#endif // HEIGHTFIELD

