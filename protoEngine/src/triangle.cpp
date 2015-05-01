#include "triangle.h"

using namespace glm;

Triangle::Triangle(){

}

Triangle::Triangle(vec3 *p0, vec3 *p1, vec3 *p2){
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
}
