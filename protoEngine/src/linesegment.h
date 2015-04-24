#ifndef LINESEGMENT
#define LINESEGMENT
#include "glm/ext.hpp"
#include "glm/glm.hpp"

#include "node.h"
#include "way.h"

class Way;

class LineSegment{
public:
    glm::vec2 p0;
    glm::vec2 p1;

    bool intersects(LineSegment other);
    LineSegment();
    LineSegment(glm::vec2 p0, glm::vec2 p1);
    LineSegment(Node* n0, Node* n1);
    LineSegment(Way* way);

    std::string toString();
};

#endif // LINESEGMENT

