#include "chunk.h"
using namespace glm;
Chunk::Chunk(){

//    this->heightfield = new Heightfield();
}
Chunk::Chunk(vec2 pos){
    this->position = pos;
    this->heightfield = new Heightfield(position, 0.01);
}

//Obviously this will move, hopefully after the chunks are more self contained.
void Chunk::heightfieldTesting(){
	int total = heightfield->triangles.size() * 3;
	int percent = total / 100;
	int filter = OSMElement::LEISURE_PARK;
    for (int i = 0 ; i < heightfield->triangles.size() ; i++){
    	// std::cout << i << " " << percent << i % percent << "\n";
        if ((i%percent)==0) std::cout << "\rBuilding heightfield vertex types: " << ((float)i/(float)total * 100) <<" percent complete";
        Triangle* tri = heightfield->triangles[i];
        if (tri == NULL) continue;

        for (int j = 0; j < 3; j++){
        	tri->vertexIDs[j] = 0.f;


	        for (auto it = ways.begin(); it != ways.end(); ++it){
	            Way *way = it->second;
                if (way->eType != OSMElement::LEISURE_PARK) continue;

	            if (way->hasPointInside(vec2(tri->vertices[j].x, tri->vertices[j].y))){
	            	tri->vertexIDs[j] = 1.f;
	                break;
	            }
	        }
    	}
    }
    std::cout << "\n";
}

