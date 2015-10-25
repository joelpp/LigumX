#include "heightfield.h"
#include "triangle.h"
#include "Settings.h"
#include "Mesh.h"
using namespace std;
using namespace glm;

Heightfield::Heightfield(){

}
Heightfield::Heightfield(vec2 startPoint, double sideLength){
    this->sideLength = sideLength;
    this->startPoint = startPoint;
    m_mesh = new Mesh;
    generateTriangles();
}

void Heightfield::generateTriangles(){
    double step = Settings::GetInstance().f("HeightfieldResolution");

    int numberOfPointsPerSide = 1 + (sideLength) / step;

    float lon = startPoint.x;
    float lat = startPoint.y;

    std::vector<glm::vec3>& points = m_mesh->m_buffers.vertexPositions;
    std::vector<glm::vec2>& UVs = m_mesh->m_buffers.m_vertexUVs;
    std::vector<int>& indexBuffer = m_mesh->m_buffers.indexBuffer;

    // Start by making horizontal lines
    float u = 0;
    float v = 0;
    float uvStep = step / sideLength;
    for(int i = 0; i < numberOfPointsPerSide; i++)
    {
        lon = startPoint.x;
        u = 0;
//        For each line generate a bunch of points
        for (int j =0; j < numberOfPointsPerSide; j++)
        {
            vec3 point = vec3(lon, lat, 0);
            points.push_back(point);
            UVs.push_back(glm::vec2(u, v));
            lon += step;
            u += uvStep;
        }
        lat += step;
        v += uvStep;
    }

    // vertically
    for (int i = 0; i < numberOfPointsPerSide - 1; ++i)
    {
        //horizontally
        for (int j = 0; j < numberOfPointsPerSide - 1; ++j)
        {
            indexBuffer.push_back( (i * numberOfPointsPerSide) + j );
            indexBuffer.push_back( ((i + 1) * numberOfPointsPerSide) + (j) );
            indexBuffer.push_back( (i * numberOfPointsPerSide) + (j + 1)) ;

            indexBuffer.push_back( ((i + 1) * numberOfPointsPerSide) + (j)  );
            indexBuffer.push_back( (i * numberOfPointsPerSide) + (j + 1)) ;
            indexBuffer.push_back( ((i + 1) * numberOfPointsPerSide) + (j + 1) );
        }

    }

}

inline double lerp(double a, double b, double t){ return a * t + b * (1 - t); }

