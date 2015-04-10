#ifndef LINESEGMENT
#define LINESEGMENT
#include "glm/ext.hpp"
#include "glm/glm.hpp"
using namespace glm;

class LineSegment{
public:
    vec2 p0;
    vec2 p1;

    bool intersects(LineSegment other);
    LineSegment();
    LineSegment(vec2 p0, vec2 p1);

    std::string toString();
};

#endif // LINESEGMENT

