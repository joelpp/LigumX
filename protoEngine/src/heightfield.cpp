#include "heightfield.h"
#include "triangle.h"
#include "Settings.h"

using namespace std;
using namespace glm;

Heightfield::Heightfield(){

}
Heightfield::Heightfield(vec2 startPoint, double sideLength){
    this->sideLength = sideLength;
    this->startPoint = startPoint;

    generateTriangles();
}

void Heightfield::addTriangle(Triangle* tri){
    triangles.push_back(tri);
}

void Heightfield::generateTriangles(){
    double step = Settings::GetInstance().f("HeightfieldResolution");

    int numberOfPointsPerSide = sideLength / step + 1;

    
    float lon = startPoint.x;
    float lat = startPoint.y;

    for (int i = 0; i < numberOfPointsPerSide + 1; i++) points.push_back(std::vector<glm::vec3>());
    // Start by making horizontal lines
    for(int i = 0; i <= numberOfPointsPerSide; i++){
        lat = startPoint.y;
//        For each line generate a bunch of points
        for (int j =0; j <= numberOfPointsPerSide; j++){

            points[j].push_back(vec3(lon, lat, 0));
            lat += step;
        }
        lon += step;
    }

    for (int i = 0; i < numberOfPointsPerSide; i++){
        for (int j = 0; j < numberOfPointsPerSide; j++){

            Triangle* tri = new Triangle(points[i][j], points[i+1][j], points[i][j+1]);
            addTriangle(tri);

            tri = new Triangle(points[i+1][j], points[i+1][j+1], points[i][j+1]);
            addTriangle(tri);

        }
    }
    // triangles[100] = NULL;
}

inline double lerp(double a, double b, double t){ return a * t + b * (1 - t); }

