#include "heightfield.h"
#include "triangle.h"
#include "Settings.h"
#include "Mesh.h"
#include "Node.h"
#include "Way.h"
#include "World.h"
#include "LigumX.h"
#include "LineSegment.h"
#include "FreeImage.h"
#include "Sector.h"
#include <sstream>
using namespace std;
using namespace glm;

inline double lerp(double a, double b, double t){ return a * t + b * (1 - t); }

Heightfield::Heightfield(){

}
Heightfield::Heightfield(vec2 startPoint, double sideLength){
    this->sideLength = sideLength;
    this->startPoint = startPoint;
    this->step = Settings::GetInstance().f("HeightfieldResolution");
    this->m_numberOfPointsPerSide = 2 + (sideLength + 0.0000001) / step;
    m_mesh = 0;
}

bool Heightfield::generateTriangles(){
    m_mesh = new Mesh();

    World* world = LigumX::GetInstance().world;
    std::stringstream ss;

    std::vector<Sector*>* relevantSectors = world->sectorsAroundPoint(startPoint,2);

    for (Sector* sector : *relevantSectors)
    {
        if (sector->m_initializationLevel < Sector::ContourLoaded)
        {
            return false;
        }
    }
    std::vector<Way*> contoursToQuery = World::getAllContourLines(relevantSectors);

    delete(relevantSectors);
    
    float lon = startPoint.x;
    float lat = startPoint.y;

    std::vector<glm::vec3>& points = m_mesh->m_buffers.vertexPositions;
    std::vector<glm::vec2>& UVs = m_mesh->m_buffers.m_vertexUVs;
    std::vector<int>& indexBuffer = m_mesh->m_buffers.indexBuffer;

    // Start by making horizontal lines
    float u = 0;
    float v = 0;
    float uvStep = step / sideLength;
    FreeImage_Initialise();
    FIBITMAP *bitmap = FreeImage_Allocate(m_numberOfPointsPerSide, m_numberOfPointsPerSide, 24);
    for(int i = 0; i < m_numberOfPointsPerSide; i++)
    {
        lon = startPoint.x;
        u = 0;
//        For each line generate a bunch of points
        for (int j = 0; j < m_numberOfPointsPerSide; j++)
        {
            float z = contourLineInterpolate(glm::vec2(lon, lat), contoursToQuery);
            // float z = 0;
            vec3 point = vec3(lon, lat, z);
            points.push_back(point);
            UVs.push_back(glm::vec2(u, v));
            lon += step;
            u += uvStep;
            RGBQUAD color;
            float val = z * 10000;
            color.rgbRed = val;
            color.rgbBlue = val;
            color.rgbGreen = val;
            // std:: cout << val << std::endl;
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
        lat += step;
        v += uvStep;
    }

    // vertically
    for (int i = 0; i < m_numberOfPointsPerSide - 1; ++i)
    {
        //horizontally
        for (int j = 0; j < m_numberOfPointsPerSide - 1; ++j)
        {
            indexBuffer.push_back( (i * m_numberOfPointsPerSide) + j );
            indexBuffer.push_back( ((i + 1) * m_numberOfPointsPerSide) + (j) );
            indexBuffer.push_back( (i * m_numberOfPointsPerSide) + (j + 1)) ;

            indexBuffer.push_back( ((i + 1) * m_numberOfPointsPerSide) + (j)  );
            indexBuffer.push_back( (i * m_numberOfPointsPerSide) + (j + 1)) ;
            indexBuffer.push_back( ((i + 1) * m_numberOfPointsPerSide) + (j + 1) );
        }

    }

    FreeImage_Save(FIF_PNG, bitmap, "test_heightfield.png", 0);
    FreeImage_DeInitialise();

    m_mesh->createBuffers();
    m_mesh->m_usesIndexBuffer = true;
    m_mesh->m_renderingMode = GL_TRIANGLES;
    // m_mesh->m_wireframeRendering = true;
    return true;
}

double bilerp(glm::vec2 xy, glm::vec3 corners[4])
{
    double f1 = 1. / ( (corners[4].x - corners[0].x) * (corners[4].y - corners[0].y) );
    double f2 = corners[0].z * (corners[4].x - xy.x) * (corners[4].y - xy.y) + 
                corners[1].z * (xy.x - corners[0].x) * (corners[4].y - xy.y) + 
                corners[2].z * (corners[4].x - xy.x) * (xy.y - corners[0].y) + 
                corners[3].z * (xy.x - corners[0].x) * (xy.y - corners[0].y);

    return (f1 * f2); 
}

double Heightfield::getHeight(glm::vec2 xy)
{
    glm::vec2 normalized = (xy - startPoint) / glm::vec2(sideLength);
    // PRINTVEC2(normalized);
    glm::vec2 unNormalized = normalized * glm::vec2(m_numberOfPointsPerSide, m_numberOfPointsPerSide);
    // PRINTVEC2(unNormalized);

    int xIndex = trunc(unNormalized.x);
    int yIndex = trunc(unNormalized.y);
    // PRINTINT(xIndex);
    // PRINTINT(yIndex);

    // PRINT(m_numberOfPointsPerSide);
    glm::vec3 corner[4];
    std::vector<glm::vec3>& points = m_mesh->m_buffers.vertexPositions;

    corner[0] = points[ yIndex * m_numberOfPointsPerSide + xIndex ];
    corner[1] = points[ yIndex * m_numberOfPointsPerSide + xIndex + 1 ];
    corner[2] = points[ (yIndex + 1) * m_numberOfPointsPerSide + xIndex ];
    corner[3] = points[ (yIndex + 1) * m_numberOfPointsPerSide + xIndex + 1];

    // PRINTVEC3(corner[0]);
    // PRINTVEC3(corner[1]);
    // PRINTVEC3(corner[2]);
    // PRINTVEC3(corner[3]);
    return bilerp(unNormalized - glm::vec2(xIndex, yIndex), corner);
}

double Heightfield::contourLineInterpolate(vec2 xy, std::vector<Way*>& contourSet) 
{
    // Get closest contour lines to xy
    vector<double> distances;
    vector<vec2> directions;
    vector<std::pair<Node*, Node*>> nodePairs;
    vector<Way*> closests = World::findNClosestWays(5, xy, 
                                                    contourSet, OSMElement::CONTOUR, 
                                                    distances, directions, nodePairs);

    int lerpedWay = getLerpedContourLines(xy, closests, directions, nodePairs);
    if (lerpedWay != -1)
    {
        closests[1] = closests[lerpedWay];
    }
    else
    { 
        return stod(closests[0]->getValue("ele")) / 15000; 
    }

    // prepare for lerping
    double sum = distances[0] + distances[lerpedWay];
    double t = 1 - distances[0] / sum;

    // get found contour lines elevations
    std::vector<double> heights;
    heights.push_back(stod(closests[0]->getValue("ele")));
    heights.push_back(stod(closests[lerpedWay]->getValue("ele")));

    double result = lerp(heights[0], heights[1], t) / 15000;
    return result;
}

/**
 * @brief Game::getLerpedContourLines
 * @param xy
 * @param ways
 * @param directions
 * @param nodePairs
 * @return
 */
int Heightfield::getLerpedContourLines(vec2 xy, vector<Way*> ways, vector<vec2> directions, 
                                       vector<std::pair<Node*, Node*>> nodePairs)
{


    //Linesegment between our search point and the closest point on the candidate contour line.
    LineSegment L0;
    L0.p0 = xy;

    //Iterate on all candidate contours
    for (int i = 1; i < ways.size(); i++){

        L0.p1 = xy + directions[i];

        //running linesegment on the absolute known nearest contour
        LineSegment L1;

        // The running nodes forming segments on the known nearest contour
        Node* n0;
        Node* n1;

        // needed to start actually counting at 1
        int counter = 1;

        int numOfIntersections = 0;
        bool intersects = false;
        bool first = true;
        // Loop on all nodes on the best contour
        for (auto nodeIt = ways[0]->nodes.begin() ; nodeIt != ways[0]->nodes.end(); ++nodeIt)
        {
            if (first)
            {
                n0 = *nodeIt;
                first = false;
                continue;
            }

            if ((counter % 2) == 0)
            {
                n0 = n1; //start a new linesegment
            }
            else
            {
                n1 = *nodeIt;

                L1.p0 = vec2(n0->longitude, n0->latitude);
                L1.p1 = vec2(n1->longitude, n1->latitude);
                if (L0.intersects(L1, 0, 0))
                {
                    intersects = true;
                    break; // we now know this way isnt viable as it intersected the nearest. we can stop looping around
                }
            }

            counter = (counter + 1) % 2;
        }
        if (!intersects) 
        {
            return i;
        }
    }
    L0.p1 = xy + directions[1];
    return -1; // method didnt really work .revert to default.
}




