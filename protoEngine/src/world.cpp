#include "world.h"
using namespace std;
using namespace glm;
World::World(){
	chunkSize = 0.05;
    coordinateInflationFactor = 1;

}

Chunk* World::createChunk(glm::vec2 pos){
	Chunk* c = new Chunk(pos);
    chunks.emplace(pos, c);
    std::stringstream ss;
    ss << "/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/Data_";
    ss << pos.x * 1000 << "x" << pos.y*1000 << ".xml";
    std::cout << ss.str() << "\n";
	loadChunkData(ss.str());
    // c->heightfieldTesting();
    extrudeAddrInterps();

    return c;
}

void World::loadChunkData(string path){
    tinyxml2::XMLDocument doc;
    // PRINT(path);
    doc.LoadFile(path.c_str());
    tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();
    Chunk* chunk = chunks[glm::vec2(-73.65, 45.5)];
    cout << docRoot->Value() << "\n";
    for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        if (string(child->Value()).compare("bound") == 0){
            // string box = child->ToElement()->FindAttribute("box")->Value();
            // std::istringstream ss(box);
            // std::string token;

            // std::getline(ss, token, ','); viewRectBottom = atof(token.c_str()) * coordinateInflationFactor - 45;
            // std::getline(ss, token, ','); viewRectLeft = atof(token.c_str()) * coordinateInflationFactor   + 73;
            // std::getline(ss, token, ','); viewRectTop = atof(token.c_str()) * coordinateInflationFactor    - 45;
            // std::getline(ss, token, ','); viewRectRight = atof(token.c_str()) * coordinateInflationFactor  + 73;

            // PRINT(viewRectBottom);
            // PRINT(viewRectLeft);
            // PRINT(viewRectTop);
            // PRINT(viewRectRight);


            // chunk = world->getOrCreateChunk(viewRectBottomLeft);
        }
        if (string(child->Value()).compare("node") == 0){
//            cout << "Looking at a node \n";
//            cout << child->ToElement()->FindAttribute("id")->Value() << "\n";

            string id = child->ToElement()->FindAttribute("id")->Value();
            float latitude = atof(child->ToElement()->FindAttribute("lat")->Value()) * coordinateInflationFactor  - 45;
            float longitude = atof(child->ToElement()->FindAttribute("lon")->Value()) * coordinateInflationFactor + 73;
            //double latitude = strtod(child->ToElement()->FindAttribute("lat")->Value(), NULL);
            //double longitude = strtod(child->ToElement()->FindAttribute("lon")->Value(), NULL);

            Node* node = new Node(id, longitude, latitude);
            node->elevation = 0;
           // if (path.compare("/Users/joelpp/Documents/Maitrise/LigumX/LigumX/protoEngine/data/result.xml") == 0) node->elevation = contourLineInterpolate(vec2(node->longitude, node->latitude)) * 1.0001;
            for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement()){
                string key = tag->ToElement()->FindAttribute("k")->Value();
                string value = tag->ToElement()->FindAttribute("v")->Value();
                node -> addTag(key, value);
            }
            chunk->nodes.emplace(id, node);
            theNodes.emplace(id, node);
        }

        else if (string(child->Value()).compare("way") == 0){
//            cout << "Looking at a way \n";
            string id = child->ToElement()->FindAttribute("id")->Value();
            Way* way = new Way(id);
            way->eType = OSMElement::NOT_IMPLEMENTED;
            for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement()){
                if (string(way_child->Value()).compare("nd") == 0){
                    string ref = way_child->ToElement()->FindAttribute("ref")->Value();
                    way -> addRef(chunk->nodes[ref]);
                }
                else if (string(way_child->Value()).compare("tag") == 0){
                    string key = way_child->ToElement()->FindAttribute("k")->Value();
                    string value = way_child->ToElement()->FindAttribute("v")->Value();
                    way -> addTag(key, value);

                    OSMElement::ElementType _eType = typeFromStrings(key, value);
                    if (_eType == OSMElement::NOT_IMPLEMENTED) continue;
                    else way->eType = _eType;
                }
            }

            chunk->ways.emplace(id, way);
            theWays.emplace(id, way);
        }
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

    }

}

void World::extrudeAddrInterps(){
    int counter = 0;

    // Iterate on all ways
    for ( auto it = theWays.begin(); it != theWays.end(); ++it ){
        Way* way = it->second;

        // Keep only ADDR_INTERPOLATIONs
        if (way->eType != OSMElement::ADDR_INTERPOLATION) continue;

        vector<Node*> nodes;
        // Store this way's 2 nodes
        for (auto nodeIt = way->nodes.begin() ; nodeIt != way->nodes.end(); ++nodeIt) nodes.push_back(*nodeIt);

        // We only want to look at roads
        int filter = OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_PRIMARY | OSMElement::HIGHWAY_TRUNK | OSMElement::HIGHWAY_SERVICE | OSMElement::HIGHWAY_UNCLASSIFIED;

        // Preparing road query...
        vector<vector<vec2> > directions;
        directions.push_back(vector<vec2>());
        directions.push_back(vector<vec2>());

        vector<std::pair<Node*, Node*>> nodePairs;
        vector<vector<double> > distances;
        distances.push_back(vector<double>());
        distances.push_back(vector<double>());

        // Find the closest roads to this ADDR_INTERP
        vector<Way*> firstNodeWays = findNClosestWays(5, vec2(nodes[0]->longitude, nodes[0]->latitude), filter, distances[0], directions[0], nodePairs);
        vector<Way*> secondNodeWays = findNClosestWays(5, vec2(nodes[1]->longitude, nodes[1]->latitude), filter, distances[1], directions[1], nodePairs);

        LineSegment L0(nodes[0], nodes[1]);

        bool intersectOtherRoad = false;
        //Check if the adress way intersects any of these roads
        for (int i = 0 ; i < firstNodeWays.size(); i++){
            LineSegment L1(firstNodeWays[i]);

            if (!intersectOtherRoad && L0.intersects(L1)) intersectOtherRoad = true;
        }

        for (int i = 0 ; i < secondNodeWays.size(); i++){
            LineSegment L1(secondNodeWays[i]);

            if (!intersectOtherRoad && L0.intersects(L1)) intersectOtherRoad = true;
        }

//        if (intersectOtherRoad) continue;

        // Find the closest way that both nodes share (should generally be the one parallel to the addr_interp)
        // Maybe better to look at dot products? This works ok for now
        pair<int, int> commonIndices = findCommonWay(firstNodeWays, secondNodeWays);

        int i = commonIndices.first;
        int j = commonIndices.second;

        // Multiplicative factor related to the polygon size
        double factor = 1.3;

        //If finding the common way didnt work just move on
        if (i == -1) continue;

        // Move on to the next if the common road is too far from one of the nodes
        if (distances[0][i] > 0.0002 * factor) continue;
        if (distances[1][j] > 0.0002 * factor) continue;

        if (distances[0][i] < 1e-6) continue;
        if (distances[1][j] < 1e-6) continue;

        // Move on to next if the nodes are on different sides of the common road
        if (dot(directions[0][i], directions[1][j]) < 0) continue;


        int totalUnits = 20;
        vec2 wayOrientation = nodes[1]->getLatLong() - nodes[0]->getLatLong();
        vec2 wayOrientationUnit =  wayOrientation / vec2(totalUnits,totalUnits);
        int remaining = totalUnits;
        float unit = length(wayOrientation) / (float)totalUnits;
        int buildingMaxUnits = 10;
        int buildingMinUnits = 5;
        Node *node0, *node1;
        vec2 pos0, pos1;
        pos0 = nodes[0]->getLatLong();
        while (remaining > 0){
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            // std::cout << r;
            if (r < 0.55){
                remaining--;
                pos0 += wayOrientationUnit;
                continue;
            }
            int numberOfUnits = buildingMinUnits + buildingMaxUnits * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (remaining - numberOfUnits < 0) numberOfUnits = remaining;
            node0 = new Node();
            node0->id = string("ADDR_INTERP_NODE0_").append(std::to_string(counter));
            node0->longitude = pos0.x;
            node0->latitude = pos0.y;

            pos1 = pos0 + wayOrientationUnit * vec2(numberOfUnits, numberOfUnits);
            node1 = new Node();
            node1->id = string("ADDR_INTERP_NODE1_").append(std::to_string(counter));
            node1->longitude = pos1.x;
            node1->latitude = pos1.y;

            // Create the new nodes by moving from each edge of the ADDR_INTERP, perpendicularly to the road
            Node* node3 = new Node();
            node3->id = string("ADDR_INTERP_NODE3_").append(std::to_string(counter));
            node3->longitude = node0->longitude - directions[0][i].x * factor;
            node3->latitude = node0 -> latitude - directions[0][i].y * factor;
            // node3->elevation = contourLineInterpolate(vec2(node3->longitude, node3->latitude)) * 1.0001;
            node3->elevation = 0;
            
            Node* node4 = new Node();
            node4->id = string("ADDR_INTERP_NODE4_").append(std::to_string(counter));
            node4->longitude = node1->longitude - directions[0][i].x * factor;
            node4->latitude = node1->latitude - directions[0][i].y * factor;
            // node4->elevation = contourLineInterpolate(vec2(node4->longitude, node4->latitude)) * 1.0001;
            node4->elevation = 0;
            
            // Create the new building
            Way* buildingWay  = new Way();
            buildingWay->addRef(node0);
            buildingWay->addRef(node3);
            buildingWay->addRef(node4);
            buildingWay->addRef(node1);
            buildingWay->addRef(node0);
            buildingWay->eType = OSMElement::BUILDING_UNMARKED;
            buildingWay->id = string("ADDR_INTERP_BUILDING").append(std::to_string(counter));

            // Store the new building
            chunks[vec2(-73.65, 45.5)]->ways.emplace(buildingWay->id, buildingWay); 
            theWays.emplace(buildingWay->id, buildingWay);
            counter++;
            pos0 = pos1;
            remaining -= numberOfUnits;
        }
    }

    // PRINTSTRING("Extruding address interpolation into polygons");
    // PRINTINT(counter);
}



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