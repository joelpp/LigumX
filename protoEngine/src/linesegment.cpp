
#include "linesegment.h"
#include <sstream>
using namespace glm;

LineSegment::LineSegment(){

}
LineSegment::LineSegment(vec2 p0, vec2 p1){
    this->p0 = p0;
    this->p1 = p1;
}

LineSegment::LineSegment(Node* n0, Node* n1){
    this->p0 = vec2(n0->longitude, n0->latitude);
    this->p1 = p1;
}

LineSegment::LineSegment(Way* way){
    this->p0 = way->nodes[0]->getLatLong();
    this->p1 = way->nodes[1]->getLatLong();
}


//bool LineSegment::intersects(LineSegment other){

//    double x00 = this->p0.x; double y00 = this->p0.y;
//    double x10 = other.p0.x; double y10 = other.p0.y;
//    double x01 = this->p1.x; double y01 = this->p1.y;
//    double x11 = other.p1.x; double y11 = other.p1.y;

//    double d = x11*y01 - x01*y11;

//    double s = (1/d) * ((x00 - x10) * y01 - (y00 - y10)*x01);
//    double t = (1/d) * -(-(x00 - x10) * y11 + (y00 - y10)*x11);

//    return ((s >= 0) && (s <= 1) && (t >= 0) && (t <= 1));
//}

bool LineSegment::intersects(LineSegment other/*, float *i_x, float *i_y*/)
{
    float p0_x = this->p0.x;
    float p0_y = this->p0.y;
    float p1_x = this->p1.x;
    float p1_y = this->p1.y;
    float p2_x = other.p0.x;
    float p2_y = other.p0.y;
    float p3_x = other.p1.x;
    float p3_y = other.p1.y;
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
//        // Collision detected
//        if (i_x != NULL)
//            *i_x = p0_x + (t * s1_x);
//        if (i_y != NULL)
//            *i_y = p0_y + (t * s1_y);
        return true;
    }

    return false; // No collision
}

std::string LineSegment::toString(){
    std::stringstream s;
    s << "LineSegment - p0: " << glm::to_string(p0) << " p1: " << glm::to_string(p1) << "\n";
    return s.str();
}
