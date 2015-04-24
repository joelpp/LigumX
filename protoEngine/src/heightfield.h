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
    Heightfield(double sideLength, glm::vec2 startPoint);

    void addTriangle(Triangle *tri);
};

#endif // HEIGHTFIELD

