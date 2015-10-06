#include "sector.h"
#include "SectorData.h"
#include "heightfield.h"

using namespace glm;
Sector::Sector(){
}

Sector::Sector(vec2 pos, float sideLength){
    this->m_pos = pos;
    this->m_heightfield = new Heightfield(m_pos, sideLength);

    loadData();
}

void Sector::loadData(){
    m_data = new SectorData(m_pos);
}
