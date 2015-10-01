#include "world.h"
using namespace std;
using namespace glm;
using namespace SpatialIndex;
World::World(){
	sectorSize = 0.05;
    coordinateInflationFactor = 1;
    // spatialIndex = createIndex();
    // waysIndex = createIndex();
    createSpatialIndexes();
}

void World::createSpatialIndexes(){
    waysIndex[OSMElement::HIGHWAY_TRUNK] = createIndex();
    waysIndex[OSMElement::HIGHWAY_PRIMARY] = createIndex();
    waysIndex[OSMElement::HIGHWAY_SECONDARY] = createIndex();
    waysIndex[OSMElement::HIGHWAY_TERTIARY] = createIndex();
    waysIndex[OSMElement::HIGHWAY_RESIDENTIAL] = createIndex();
    waysIndex[OSMElement::HIGHWAY_SERVICE] = createIndex();
    waysIndex[OSMElement::HIGHWAY_UNCLASSIFIED] = createIndex();
    waysIndex[OSMElement::CONTOUR] = createIndex();
    waysIndex[OSMElement::BUILDING_UNMARKED] = createIndex();
    waysIndex[OSMElement::BUILDING_SCHOOL] = createIndex();
    waysIndex[OSMElement::BUILDING_ADDRINTERP] = createIndex();
    waysIndex[OSMElement::BOUNDARY] = createIndex();
    waysIndex[OSMElement::LEISURE_PARK] = createIndex();
    waysIndex[OSMElement::NATURAL_WOOD] = createIndex();
    waysIndex[OSMElement::NATURAL_WATER] = createIndex();
    waysIndex[OSMElement::LANDUSE] = createIndex();
    waysIndex[OSMElement::RAILWAY_SUBWAY] = createIndex();
    waysIndex[OSMElement::aDEBUG] = createIndex();
    waysIndex[OSMElement::ADDR_INTERPOLATION] = createIndex();
    waysIndex[OSMElement::NOT_IMPLEMENTED] = createIndex();
    waysIndex[OSMElement::ANY_TYPE] = createIndex();
    waysIndex[OSMElement::GRID_LINE] = createIndex();
}

Sector* World::createSector(glm::vec2 pos){
	Sector* c = new Sector(pos);
    sectors.emplace(pos, c);

    // c->heightfieldTesting();
    // extrudeAddrInterps();
    PRINT(theWays.size());
    // for (auto waysIt = theWays.begin(); waysIt != theWays.end(); ++waysIt){
    //     Way* way = waysIt->second;
    //     bool first = true;
    //     Node *n0, *n1;
    //     int counter = 1;
    //     OSMElement::ElementType type = way->eType;
    //     if (type == OSMElement::NOT_IMPLEMENTED) continue;
    //     for (auto it = way->nodes.begin(); it != way->nodes.end(); ++it){
    //         if (first){
    //             first = false;
    //             n0 = *it;
    //             continue;
    //         }

    //         if (counter % 2 == 0) n0 = *it;
    //         else{
    //             n1 = *it;
    //             std::cout << n0->toString() << "\n";
    //             std::cout << n1->toString() << "\n";
    //             addLineSegment(waysIndex[type], n0->getLatLong(), n1->getLatLong(), atoi(way->id.c_str()));
    //         }

    //         counter++;
    //     } 
    // }

    return c;
}

void World::loadSectorData(string path){
//     tinyxml2::XMLDocument doc;
//     // PRINT(path);
//     doc.LoadFile(path.c_str());
//     tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();
//     Sector* sector = sectors[glm::vec2(-73.65, 45.5)];
//     cout << docRoot->Value() << "\n";
//     int i = 0;
//     for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
//     {
//         if (string(child->Value()).compare("bound") == 0){
//             // string box = child->ToElement()->FindAttribute("box")->Value();
//             // std::istringstream ss(box);
//             // std::string token;

//             // std::getline(ss, token, ','); viewRectBottom = atof(token.c_str()) * coordinateInflationFactor - 45;
//             // std::getline(ss, token, ','); viewRectLeft = atof(token.c_str()) * coordinateInflationFactor   + 73;
//             // std::getline(ss, token, ','); viewRectTop = atof(token.c_str()) * coordinateInflationFactor    - 45;
//             // std::getline(ss, token, ','); viewRectRight = atof(token.c_str()) * coordinateInflationFactor  + 73;

//             // PRINT(viewRectBottom);
//             // PRINT(viewRectLeft);
//             // PRINT(viewRectTop);
//             // PRINT(viewRectRight);


//             // sector = world->getOrCreateSector(viewRectBottomLeft);
//         }
//         if (string(child->Value()).compare("node") == 0){
// //            cout << "Looking at a node \n";
// //            cout << child->ToElement()->FindAttribute("id")->Value() << "\n";

//             string id = child->ToElement()->FindAttribute("id")->Value();
//             float latitude = atof(child->ToElement()->FindAttribute("lat")->Value()) * coordinateInflationFactor  - 45;
//             float longitude = atof(child->ToElement()->FindAttribute("lon")->Value()) * coordinateInflationFactor + 73;
//             //double latitude = strtod(child->ToElement()->FindAttribute("lat")->Value(), NULL);
//             //double longitude = strtod(child->ToElement()->FindAttribute("lon")->Value(), NULL);

//             Node* node = new Node(id, longitude, latitude);
//             node->elevation = 0;
//            // if (path.compare("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/result.xml") == 0) node->elevation = contourLineInterpolate(vec2(node->longitude, node->latitude)) * 1.0001;
//             for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement()){
//                 string key = tag->ToElement()->FindAttribute("k")->Value();
//                 string value = tag->ToElement()->FindAttribute("v")->Value();
//                 node -> addTag(key, value);
//             }
//             sector->nodes.emplace(id, node);
//             theNodes.emplace(id, node);
//             addPoint(spatialIndex, longitude, latitude, atoi(id.c_str()));
//             // i++;
//         }

//         else if (string(child->Value()).compare("way") == 0){
// //            cout << "Looking at a way \n";
//             string id = child->ToElement()->FindAttribute("id")->Value();
//             Way* way = new Way(id);
//             way->eType = OSMElement::NOT_IMPLEMENTED;
//             for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement()){
//                 if (string(way_child->Value()).compare("nd") == 0){
//                     string ref = way_child->ToElement()->FindAttribute("ref")->Value();
//                     way -> addRef(sector->nodes[ref]);
//                 }
//                 else if (string(way_child->Value()).compare("tag") == 0){
//                     string key = way_child->ToElement()->FindAttribute("k")->Value();
//                     string value = way_child->ToElement()->FindAttribute("v")->Value();
//                     way -> addTag(key, value);

//                     OSMElement::ElementType _eType = typeFromStrings(key, value);
//                     if (_eType == OSMElement::NOT_IMPLEMENTED) continue;
//                     else way->eType = _eType;
//                 }
//             }

//             sector->ways.emplace(id, way);
//             theWays.emplace(id, way);
//             waysTypeMap[way->eType].emplace(id,way);
//         }
//         else if (string(child->Value()).compare("relation") == 0){
//             string id = child->ToElement()->FindAttribute("id")->Value();
//             Relation *relation = new Relation(id);

//             for (tinyxml2::XMLNode* relation_child = child->FirstChildElement(); relation_child != NULL; relation_child = relation_child->NextSiblingElement()){
//                 if (string(relation_child->Value()).compare("member") == 0){
//                     string type = relation_child->ToElement()->FindAttribute("type")->Value();
//                     string elementId = relation_child->ToElement()->FindAttribute("ref")->Value();
// //                    PRINT(type);
// //                    PRINT(elementId);
//                     //Odd. Ucommenting this crashes the program when trying to fill the buffers.
//                     try{
//                         if (type.compare("node") == 0) relation->addMember(theNodes.at(elementId));
//                         else if (type.compare("way") == 0) relation->addMember(theWays.at(elementId));
//                         else if (type.compare("relation") == 0) relation->addMember(theRelations.at(elementId));
//                     }
//                     catch(...){

//                     }
//                 }
//                 else if (string(relation_child->Value()).compare("tag") == 0){
//                     string key = relation_child->ToElement()->FindAttribute("k")->Value();
//                     string value = relation_child->ToElement()->FindAttribute("v")->Value();
//                     relation->addTag(key, value);
//                 }
//             }
//             theRelations.emplace(id, relation);
//         }

    // }

}

void World::addSidewalks(){
    for (auto it = waysTypeMap[OSMElement::HIGHWAY_RESIDENTIAL].begin(); it != waysTypeMap[OSMElement::HIGHWAY_RESIDENTIAL].end(); ++it){
        Way* way = it->second;
        int first = true;
        int counter = 0;
        Node *node0, *node1;
        for (auto nodeIt = way->nodes.begin(); nodeIt != way->nodes.end();++it){
            if (first){
                node0 = *nodeIt;
                first = false;
            }

            if ((counter % 2) == 0) node0 = node1;
            else{
                node1 = *nodeIt;

                vec2 coords0 = node0->getLatLong();
                double coords[2];
                coords[0] = coords0.x; coords[1] = coords0.y;
                const Point p0 = Point(coords, 2);
                vec2 coords1 = node1->getLatLong();
                coords[0] = coords1.x; coords[1] = coords1.y;
                const Point p1 = Point(coords, 2);
                SpatialIndex::LineSegment ls = SpatialIndex::LineSegment(p0, p1);

                
            }


            counter++;
        }

    }
}

void World::extrudeAddrInterps(){
    // int counter = 0;

    // // Iterate on all ways
    // for ( auto it = waysTypeMap[OSMElement::ADDR_INTERPOLATION].begin(); it != waysTypeMap[OSMElement::ADDR_INTERPOLATION].end(); ++it ){
    //     Way* way = it->second;

    //     vector<Node*> nodes;
    //     // Store this way's 2 nodes
    //     for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt) nodes.push_back(*nodeIt);
    //     vec2 center = (nodes[0]->getLatLong() + nodes[1]->getLatLong) / vec2(2,2);
    //     // We only want to look at roads
        // int filter = OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_PRIMARY | OSMElement::HIGHWAY_TRUNK | OSMElement::HIGHWAY_SERVICE | OSMElement::HIGHWAY_UNCLASSIFIED;



        // Find the closest roads to this ADDR_INTERP


        // LineSegment L0(nodes[0], nodes[1]);

        // bool intersectOtherRoad = false;
        // //Check if the adress way intersects any of these roads
        // for (int i = 0 ; i < firstNodeWays.size(); i++){
        //     LineSegment L1(firstNodeWays[i]);

        //     if (!intersectOtherRoad && L0.intersects(L1,NULL,NULL)) intersectOtherRoad = true;
        // }

        // for (int i = 0 ; i < secondNodeWays.size(); i++){
        //     LineSegment L1(secondNodeWays[i]);

        //     if (!intersectOtherRoad && L0.intersects(L1, NULL, NULL)) intersectOtherRoad = true;
        // }

//        if (intersectOtherRoad) continue;

        // Find the closest way that both nodes share (should generally be the one parallel to the addr_interp)
        // Maybe better to look at dot products? This works ok for now
        // pair<int, int> commonIndices = findCommonWay(firstNodeWays, secondNodeWays);

        // int i = commonIndices.first;
        // int j = commonIndices.second;
        // 
        // Way* way = getNearestWays(OSMElement::HIGHWAY_RESIDENTIAL, center.x, center.y, 1);
        // // vec2 direction = 

        // // Multiplicative factor related to the polygon size
        // double factor = 1.3;

        // //If finding the common way didnt work just move on
        // if (i == -1) continue;

        // // Move on to the next if the common road is too far from one of the nodes
        // if (distances[0][i] > 0.0002 * factor) continue;
        // if (distances[1][j] > 0.0002 * factor) continue;

        // if (distances[0][i] < 1e-6) continue;
        // if (distances[1][j] < 1e-6) continue;

        // // Move on to next if the nodes are on different sides of the common road
        // if (dot(directions[0][i], directions[1][j]) < 0) continue;


        // int totalUnits = 20;
        // vec2 wayOrientation = nodes[1]->getLatLong() - nodes[0]->getLatLong();
        // vec2 wayOrientationUnit =  wayOrientation / vec2(totalUnits,totalUnits);
        // int remaining = totalUnits;
        // float unit = length(wayOrientation) / (float)totalUnits;
        // int buildingMaxUnits = 10;
        // int buildingMinUnits = 5;
        // Node *node0, *node1;
        // vec2 pos0, pos1;
        // pos0 = nodes[0]->getLatLong();
        // while (remaining > 0){
        //     float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        //     // std::cout << r;
        //     if (r < 0.55){
        //         remaining--;
        //         pos0 += wayOrientationUnit;
        //         continue;
        //     }
        //     int numberOfUnits = buildingMinUnits + buildingMaxUnits * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        //     if (remaining - numberOfUnits < 0) numberOfUnits = remaining;
        //     node0 = new Node();
        //     node0->id = string("ADDR_INTERP_NODE0_").append(std::to_string(counter));
        //     node0->longitude = pos0.x;
        //     node0->latitude = pos0.y;

        //     pos1 = pos0 + wayOrientationUnit * vec2(numberOfUnits, numberOfUnits);
        //     node1 = new Node();
        //     node1->id = string("ADDR_INTERP_NODE1_").append(std::to_string(counter));
        //     node1->longitude = pos1.x;
        //     node1->latitude = pos1.y;

        //     // Create the new nodes by moving from each edge of the ADDR_INTERP, perpendicularly to the road
        //     Node* node3 = new Node();
        //     node3->id = string("ADDR_INTERP_NODE3_").append(std::to_string(counter));
        //     node3->longitude = node0->longitude - directions[0][i].x * factor;
        //     node3->latitude = node0 -> latitude - directions[0][i].y * factor;
        //     // node3->elevation = contourLineInterpolate(vec2(node3->longitude, node3->latitude)) * 1.0001;
        //     node3->elevation = 0;
            
        //     Node* node4 = new Node();
        //     node4->id = string("ADDR_INTERP_NODE4_").append(std::to_string(counter));
        //     node4->longitude = node1->longitude - directions[0][i].x * factor;
        //     node4->latitude = node1->latitude - directions[0][i].y * factor;
        //     // node4->elevation = contourLineInterpolate(vec2(node4->longitude, node4->latitude)) * 1.0001;
        //     node4->elevation = 0;
            
        //     // Create the new building
        //     Way* buildingWay  = new Way();
        //     buildingWay->addRef(node0);
        //     buildingWay->addRef(node3);
        //     buildingWay->addRef(node4);
        //     buildingWay->addRef(node1);
        //     buildingWay->addRef(node0);
        //     buildingWay->eType = OSMElement::BUILDING_UNMARKED;
        //     buildingWay->id = string("ADDR_INTERP_BUILDING").append(std::to_string(counter));

        //     // Store the new building
        //     sectors[vec2(-73.65, 45.5)]->ways.emplace(buildingWay->id, buildingWay); 
        //     theWays.emplace(buildingWay->id, buildingWay);
        //     counter++;
        //     pos0 = pos1;
        //     remaining -= numberOfUnits;
        // }
    // }

    // PRINTSTRING("Extruding address interpolation into polygons");
    // PRINTINT(counter);
}

// std::



std::vector<Way*> World::findNClosestWays(int n, vec2 xy, int filter, vector<double> &distances, vector<vec2> &_directions, vector<std::pair<Node*, Node*>> &_nodePairs){
    Way* closest = NULL;
    std::vector<Way*> closests;
    std::vector<double> bestDists;
    std::vector<vec2> directions;
    std::pair<Node*, Node*> a;
    vector<std::pair<Node*, Node*>> nodePairs;
    for (int i = 0; i < n; i++){
        bestDists.push_back(99999);
        closests.push_back(NULL);
        directions.push_back(vec2(0,0));
        nodePairs.push_back(a);
    }

    //Look at all the ways
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;
//        if (!way->selectable) continue;

        // This way's type not yet implemented :( get to it!
        if (way->eType == OSMElement::NOT_IMPLEMENTED) continue;

        // A specific filter has been chosen. Check if this way passes it!
        if (filter != OSMElement::ANY_TYPE)
            if ((way->eType & filter) == 0) continue;

        Node* firstNode;
        Node* secondNode;
        int counter = 1;
        bool first = true;
        vec2 p1, p2;
        double bestDist = 99999;
        vec2 direction;
        std::pair<Node*, Node*> aPair;

        //Look at all this way's nodes
        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt){

            // Keep first node and continue
            if (first){
                firstNode = *nodeIt;
                first = false;
                continue;
            }

            // Prepare next pair by putting the old second in the new first
            if (counter == 0) firstNode = secondNode;
            // Look at next pair by looking at new second node
            else{
                secondNode = *nodeIt;
                p1 = vec2(firstNode->longitude, firstNode->latitude);
                p2 = vec2(secondNode->longitude, secondNode->latitude);
                vec2 dir;
                double dist = pointLineSegmentDistance(xy, p2, p1, dir);

                if (dist < bestDist){
                    closest = way;
                    bestDist = dist;
                    direction = dir;
                    aPair = std::pair<Node*, Node*>(firstNode, secondNode);
                }
            }

            //flip-flop
            counter = (counter + 1) % 2;
        }
        //Now you're done looking at all the nodes.
        //Iterate over all current closest list, from closest to farthest
        for (int i = 0; i < n; i++){

            // Otherwise, we found a smaller distance (the way may have been seen or not)
            if (bestDist <= bestDists[i]){


                // Move everything behind this element farther by 1
                for (int j = n-1; j > i; j--){
                    bestDists[j] = bestDists[j-1];
                    closests[j] = closests[j-1];
                    directions[j] = directions[j-1];
                    nodePairs[j] = nodePairs[j-1];
                }

                // Keep the way and the distance.
                bestDists[i] = bestDist;
                closests[i] = way;
                directions[i] = direction;
                nodePairs[i] = aPair;
                break; //get out! you don't want to keep going in the array.

            }

        }

    }
    _directions = directions;
    distances = bestDists;
    _nodePairs = nodePairs;
    return closests;
}


pair<int, int> World::findCommonWay(vector<Way*> firstNodeWays, vector<Way*> secondNodeWays){
    for (int i = 0; i < firstNodeWays.size(); i++){
        for (int j = 0; j < secondNodeWays.size(); j++){
            if (firstNodeWays[i]->id.compare(secondNodeWays[j]->id) == 0) return pair<int, int>(i,j);
        }
    }
    return pair<int, int>(-1,-1);
}

double World::pointLineSegmentDistance(vec2 p, vec2 v, vec2 w, vec2 &direction) {
  // Return minimum distance between line segment vw and point p
  const double l2 = glm::length(w-v) * glm::length(w-v);  // i.e. |w-v|^2 -  avoid a sqrt
  if (l2 == 0.0){
      direction = v - p;
      return glm::distance(p, v);   // v == w case
  }
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line.
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  const double t = glm::dot(p - v, w - v) / l2;
  if (t < 0.0){
      direction = v - p;
      return glm::distance(p, v); // Beyond the 'v' end of the segment
  }
  else if (t > 1.0){
      direction = w - p;
      return glm::distance(p, w);  // Beyond the 'w' end of the segment
  }
  const vec2 projection = v + (vec2(t,t) * (w - v));  // Projection falls on the segment
  direction = projection - p;
  return glm::distance(p, projection);
}

Index* World::createIndex()
{
    // create a property set with default values.
    // see utility.cc for all defaults  http://libspatialindex.github.io/doxygen/Utility_8cc_source.html#l00031
    Tools::PropertySet* ps = GetDefaults();
    Tools::Variant var;

    // set index type to R*-Tree
    var.m_varType = Tools::VT_ULONG;
    var.m_val.ulVal = RT_RTree;
    ps->setProperty("IndexType", var);

    // Set index to store in memory (default is disk)
    var.m_varType = Tools::VT_ULONG;
    var.m_val.ulVal = RT_Memory;
    ps->setProperty("IndexStorageType", var);

    // initalise index
    Index* idx = new Index(*ps);
    delete ps;

    // check index is ok
    if (!idx->index().isIndexValid())
        throw "Failed to create valid index";
    else
        cout << "created index" << endl;

    return idx;
}

// add a Point to index.
void World::addPoint(Index* idx,double lon,double lat, int64_t id)
{
    // create array with lat/lon points
    double coords[] = {lon, lat};

    // shapes can also have anobject associated with them but we'll leave that for the moment.
    uint8_t* pData = 0;
    uint32_t nDataLength = 0;

    // create shape
    SpatialIndex::IShape* shape = 0;
    shape = new SpatialIndex::Point(coords, 2);

    // insert into index along with the an object and an ID
    idx->index().insertData(nDataLength,pData,*shape,id);

    // cout << "Point " << id << " inserted into index." << endl;

    delete shape;

}

void World::addLineSegment(Index* idx, vec2 _p0,vec2 _p1, int64_t id)
{
    // create array with lat/lon points

    // shapes can also have anobject associated with them but we'll leave that for the moment.
    uint8_t* pData = 0;
    uint32_t nDataLength = 0;

    // create shape
    SpatialIndex::IShape* shape = 0;
    double coords[] = {_p0.x, _p0.y};
    const SpatialIndex::Point* p0 = new SpatialIndex::Point(coords, 2);
    coords[0] = _p1.x; coords[1] = _p1.y;
    const SpatialIndex::Point* p1 = new SpatialIndex::Point(coords, 2);
    shape = new SpatialIndex::LineSegment(*p0, *p1);
    // insert into index along with the an object and an ID
    idx->index().insertData(nDataLength,pData,*shape,id);

    // cout << "LineSegment " << id << " inserted into index." << endl;

    delete shape;

}

std::vector<Way*> World::getNearestWays(OSMElement::ElementType type,double lon,double lat,double maxResults){
    std::vector<Way*> toReturn;

    std::vector<SpatialIndex::IData*>* data = getNearest(waysIndex[type], lon, lat, maxResults);

    for (SpatialIndex::IData* &item : (*data))
        {
            // get the generic shape object which we can cast to other types
            SpatialIndex::IShape* shape;
            item->getShape(&shape);

            // cast the shape to a Point
            SpatialIndex::Point center;
            shape->getCenter(center);

            //get ID of shape
            id_type id = item->getIdentifier();
            std::stringstream ssid;
            ssid << id;
            Way* way = waysTypeMap[type][ssid.str()];

            toReturn.push_back(way);
        }
        return toReturn;
}


std::vector<SpatialIndex::IData*>* World::getNearest(Index* idx,double lon,double lat,double maxResults)
{
    double coords[] = {lon,lat};

    // get a visitor object and a point from which to search
    ObjVisitor* visitor = new ObjVisitor;
    // make point from lat/lon with two dimentions
    SpatialIndex::Point* r = new SpatialIndex::Point(coords, 2);

    // get nearesr maxResults shapes form index
    idx->index().nearestNeighborQuery(maxResults,*r,*visitor);

    // get count of results
    int64_t nResultCount;
    nResultCount = visitor->GetResultCount();

    // get actual results
    std::vector<SpatialIndex::IData*>& results = visitor->GetResults();
    // an empty vector that wewill copt the results to
    vector<SpatialIndex::IData*>* resultsCopy = new vector<SpatialIndex::IData*>();

    // copy the Items into the newly allocated vector array
    // we need to make sure to clone the actual Item instead
    // of just the pointers, as the visitor will nuke them
    // upon destroy
    for (int64_t i = 0; i < nResultCount; ++i)
    {
        resultsCopy->push_back(dynamic_cast<SpatialIndex::IData*>(results[i]->clone()));
    }

    delete r;
    delete visitor;

    cout << "found " << nResultCount << " results." << endl;

    return resultsCopy;
}

std::vector<SpatialIndex::IData*>* World::boxQuery(Index* idx, glm::vec2 bottomLeft,glm::vec2 topRight, double maxResults)
{
    double coords[2];

    // get a visitor object and a point from which to search
    ObjVisitor* visitor = new ObjVisitor;

    // make point from lat/lon with two dimentions
    coords[0] = bottomLeft.x; coords[1] = bottomLeft.y;
    const SpatialIndex::Point *a = new SpatialIndex::Point(coords,2);
    coords[0] = topRight.x; coords[1] = topRight.y;
    const SpatialIndex::Point *b = new SpatialIndex::Point(coords,2);
    SpatialIndex::Region* r = new SpatialIndex::Region(*a,*b);

    // get nearesr maxResults shapes form index
    idx->index().containsWhatQuery(*r,*visitor);

    // get count of results
    int64_t nResultCount;
    nResultCount = visitor->GetResultCount();

    // get actual results
    std::vector<SpatialIndex::IData*>& results = visitor->GetResults();
    // an empty vector that wewill copt the results to
    vector<SpatialIndex::IData*>* resultsCopy = new vector<SpatialIndex::IData*>();

    // copy the Items into the newly allocated vector array
    // we need to make sure to clone the actual Item instead
    // of just the pointers, as the visitor will nuke them
    // upon destroy
    for (int64_t i = 0; i < nResultCount; ++i)
    {
        resultsCopy->push_back(dynamic_cast<SpatialIndex::IData*>(results[i]->clone()));
    }

    delete r;
    delete visitor;
    delete a;
    delete b;
    // cout << "found " << nResultCount << " results." << endl;

    return resultsCopy;
}

Node* World::findClosestNode(vec2 xy){
    Node* closest;
    double bestDist = 99999;
    for ( auto it = theNodes.begin(); it != theNodes.end(); ++it ){
        Node* n = it->second;

        vec2 point = vec2(n->longitude, n->latitude);

        double dist = glm::distance(point, xy);

        if (dist < bestDist){
            closest = n;
            bestDist = dist;
        }
    }
    return closest;
}

OSMElement::ElementType World::typeFromStrings(string key, string value){
    if (key.compare("highway") == 0){
        if (value.compare("trunk") == 0) return OSMElement::HIGHWAY_TRUNK;
        else if (value.compare("primary") == 0) return OSMElement::HIGHWAY_PRIMARY;
        else if (value.compare("secondary") == 0) return OSMElement::HIGHWAY_SECONDARY;
        else if (value.compare("tertiary") == 0) return OSMElement::HIGHWAY_TERTIARY;
        else if (value.compare("residential") == 0) return OSMElement::HIGHWAY_RESIDENTIAL;
        else if (value.compare("service") == 0) return OSMElement::HIGHWAY_SERVICE;
        else if (value.compare("unclassified") == 0) return OSMElement::HIGHWAY_UNCLASSIFIED;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("natural") == 0){
        if (value.compare("wood") == 0) return OSMElement::NATURAL_WOOD;
        if (value.compare("water") == 0) return OSMElement::NATURAL_WATER;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("building") == 0){
        if (value.compare("yes") == 0) return OSMElement::BUILDING_UNMARKED;
        else if (value.compare("school") == 0) return OSMElement::BUILDING_SCHOOL;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("contour") == 0) return OSMElement::CONTOUR;
    else if (key.compare("leisure") == 0){
        if (value.compare("park") == 0) return OSMElement::LEISURE_PARK;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("addr:interpolation") == 0){ return OSMElement::ADDR_INTERPOLATION; }
    else if (key.compare("landuse") == 0){ return OSMElement::LANDUSE; }
    else if (key.compare("boundary") == 0){ return OSMElement::BOUNDARY; }

    else return OSMElement::NOT_IMPLEMENTED;
}
