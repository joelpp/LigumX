#ifndef LINESEGMENT
#define LINESEGMENT
#include "glm/ext.hpp"
#include "glm/glm.hpp"

#include "node.h"
#include "way.h"

class Way;

using namespace glm;

class LineSegment{
public:
    vec2 p0;
    vec2 p1;

    bool intersects(LineSegment other);
    LineSegment();
    LineSegment(vec2 p0, vec2 p1);
    LineSegment::LineSegment(Node* n0, Node* n1);
    LineSegment::LineSegment(Way* way);

    std::string toString();
};

#endif // LINESEGMENT

