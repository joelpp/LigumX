#ifndef HEIGHTFIELD
#define HEIGHTFIELD

#include <vector>
#include <glm/glm.hpp>
#include "Logging.h"

class Triangle;
class Mesh;
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


    Mesh* m_mesh;
};

#endif // HEIGHTFIELD
