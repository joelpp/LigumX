#include "game.h"
#include "heightfield.h"

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
    PRINT("GENERATIRNG TRINGALES");
    double step = 0.001;

    int numberOfPointsPerSide = sideLength / step;

    
    float lon = startPoint.x + 73;
    float lat = startPoint.y - 45;

    for (int i = 0; i < numberOfPointsPerSide + 1; i++) points.push_back(std::vector<glm::vec3>());
    // Start by making horizontal lines
    for(int i = 0; i <= numberOfPointsPerSide; i++){
        lat = startPoint.y - 45;
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
    PRINT(triangles.size());
}

inline double lerp(double a, double b, double t){ return a * t + b * (1 - t); }

double Game::contourLineInterpolate(vec2 xy){
    // Get closest contour lines to xy
    vector<double> distances;
    vector<vec2> directions;
    vector<std::pair<Node*, Node*>> nodePairs;
    vector<Way*> closests = findNClosestWays(5, xy, OSMElement::CONTOUR, distances, directions,nodePairs);

//    int lerpedWay = 1;
    int lerpedWay = getLerpedContourLines(xy, closests, directions, nodePairs);
    if (lerpedWay != -1) closests[1] = closests[lerpedWay];
    else{ return stod(closests[0]->getValue("ele")) / 15000; }

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
int Game::getLerpedContourLines(vec2 xy, vector<Way*> ways, vector<vec2> directions, vector<std::pair<Node*, Node*>> nodePairs){


    // //Linesegment between our search point and the closest point on the candidate contour line.
    // LineSegment L0;
    // L0.p0 = xy;

    // //Iterate on all candidate contours
    // for (int i = 1; i < ways.size(); i++){

    //     L0.p1 = xy + directions[i];

    //     //running linesegment on the absolute known nearest contour
    //     LineSegment L1;

    //     // The running nodes forming segments on the known nearest contour
    //     Node* n0;
    //     Node* n1;

    //     // needed to start actually counting at 1
    //     int counter = 1;

    //     int numOfIntersections = 0;
    //     bool intersects = false;
    //     bool first = true;
    //     // Loop on all nodes on the best contour
    //     for (auto nodeIt = ways[0]->nodes.begin() ; nodeIt != ways[0]->nodes.end(); ++nodeIt){
    //         if (first){
    //             n0 = *nodeIt;
    //             first = false;
    //             continue;
    //         }

    //         if ((counter % 2) == 0) n0 = n1; //start a new linesegment
    //         else{
    //             n1 = *nodeIt;

    //             L1.p0 = vec2(n0->longitude, n0->latitude);
    //             L1.p1 = vec2(n1->longitude, n1->latitude);
    //             if (L0.intersects(L1, NULL, NULL)){
    //                 intersects = true;
    //                 break; // we now know this way isnt viable as it intersected the nearest. we can stop looping around
    //             }
    //         }
    //         counter = (counter + 1) % 2;
    //     }
    //     if (!intersects){ return i; }
    // }
    // L0.p1 = xy + directions[1];
    // return -1; // method didnt really work .revert to default.
}
