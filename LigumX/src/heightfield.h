#ifndef HEIGHTFIELD
#define HEIGHTFIELD

#include <vector>
#include <glm/glm.hpp>
#include "Logging.h"

class Triangle;
class Mesh;
class Way;
class Node;
class Heightfield{
private:
  	REGISTERCLASS(Heightfield);

public:
    std::vector<Triangle* > triangles;
    double sideLength;
    glm::vec2 startPoint;

    Heightfield();
    Heightfield(glm::vec2 startPoint,double sideLength);

    bool generateTriangles();
	static double contourLineInterpolate(glm::vec2 xy, std::vector<Way*>& contourLines);
	static int getLerpedContourLines(glm::vec2 xy, std::vector<Way*> ways, std::vector<glm::vec2> directions, 
                                       			  std::vector<std::pair<Node*, Node*>> nodePairs);
	double getHeight(glm::vec2 xy);
	double step;
    Mesh* m_mesh;
    int m_numberOfPointsPerSide;
};

#endif // HEIGHTFIELD
