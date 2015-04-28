#include "game.h"
#include "heightfield.h"

using namespace std;

Heightfield::Heightfield(){

}
Heightfield::Heightfield(double sideLength, vec2 startPoint){
    this->sideLength = sideLength;
    this->startPoint = startPoint;
}

void Heightfield::addTriangle(Triangle* tri){
    triangles.push_back(tri);
}


void Game::generateGridLines( vector<vec3> *groundTrianglesPositions, vector<vec2> *groundTrianglesUV){

    double step = 0.001;

    int lonCounter = 0;
    int latCounter = 0;
    std::vector<Way*> latitudeLines;
    std::vector<Way*> longitudeLines;

    // Start by making horizontal lines
    for( double lon = viewRectLeft; lon <= viewRectRight; lon += step){
        Way* way = new Way();

        way->eType = OSMElement::GRID_LINE;
        latCounter = 0;

//        For each line generate a bunch of nodes
        for (double lat = viewRectBottom; lat <= viewRectTop; lat += step){

            Node* n = new Node();
            n->id = to_string(static_cast<long double>(lon)).append(",").append(to_string(static_cast<long double>(lat)));
            n->longitude = lon;
            n->latitude = lat;
           // n->elevation = (contourLineInterpolate(vec2(n->longitude, n->latitude)));
            n->elevation = 0;
//            for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
//                Way* myway = it->second;

//                if (myway->eType != OSMElement::LEISURE_PARK) continue;
//                if (myway->hasPointInside(n->getLatLong())){
////                    PRINTSTRING("FOUND A POINT");
//                    n->elevation = 0.001;
//                    break;
//                }
//            }
            way->addRef(n);
            latCounter++;
        }
        way->id = string("LON").append(to_string(static_cast<long double>((way->nodes[0]->longitude))));
        longitudeLines.push_back(way);
        lonCounter++;
    }
    PRINTINT(unsuccessfulInterpolations);
    // Then, create as many vertical lines as there are nodes in the previous horizontal lines
    for (int i = 0; i < latCounter; i++){
        Way* way = new Way();
        way->eType = OSMElement::GRID_LINE;

        // For ith vertical line grab the ith nodes in each horizontal line previously created
        for (int j = 0; j < lonCounter; j++) way->addRef(longitudeLines[j]->nodes[i]);

        way->id = string("LAT").append(to_string(static_cast<long double>((way->nodes[0]->latitude))));
        latitudeLines.push_back(way);

    }
    heightfield = Heightfield(step, viewRectBottomLeft);
    for (int i = 0; i < latCounter-1; i++){
        for (int j = 0; j < lonCounter-1; j++){

            Triangle* tri = new Triangle(latitudeLines[i]->nodes[j]->getLatLongEle(),
                                        latitudeLines[i]->nodes[j+1]->getLatLongEle(),
                                        latitudeLines[i+1]->nodes[j]->getLatLongEle());
            heightfield.addTriangle(tri);

            tri = new Triangle(latitudeLines[i]->nodes[j+1]->getLatLongEle(),
                           latitudeLines[i+1]->nodes[j+1]->getLatLongEle(),
                           latitudeLines[i+1]->nodes[j]->getLatLongEle());
            heightfield.addTriangle(tri);

        }
    }
    heightfield.triangles[50] = NULL;
    // heightfield.triangles[100] = NULL;
    PRINT(heightfield.triangles.size());

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
        for (auto nodeIt = ways[0]->nodes.begin() ; nodeIt != ways[0]->nodes.end(); ++nodeIt){
            if (first){
                n0 = *nodeIt;
                first = false;
                continue;
            }

            if ((counter % 2) == 0) n0 = n1; //start a new linesegment
            else{
                n1 = *nodeIt;

                L1.p0 = vec2(n0->longitude, n0->latitude);
                L1.p1 = vec2(n1->longitude, n1->latitude);
                if (L0.intersects(L1)){
                    intersects = true;
                    break; // we now know this way isnt viable as it intersected the nearest. we can stop looping around
                }
            }
            counter = (counter + 1) % 2;
        }
        if (!intersects){ return i; }
    }
    L0.p1 = xy + directions[1];
    return -1; // method didnt really work .revert to default.
}
