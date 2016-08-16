#include "stdafx.h"

#include "triangle.h"

using namespace glm;

Triangle::Triangle(){

}

Triangle::Triangle(vec3 p0, vec3 p1, vec3 p2){
    this->vertices[0] = p0;
    this->vertices[1] = p1;
    this->vertices[2] = p2;
}
