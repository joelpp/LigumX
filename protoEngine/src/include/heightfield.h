#ifndef HEIGHTFIELD
#define HEIGHTFIELD

#include <vector>
#include <glm/glm.hpp>
#include "Logging.h"

class Triangle;

class Heightfield{
private:
  	REGISTERCLASS(Heightfield);

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

