#include "sector.h"
#include "heightfield.h"

using namespace glm;
Sector::Sector(){
}

Sector::Sector(vec2 pos,  float size){
    this->m_pos = pos;
    this->m_size = glm::vec2(size);
    m_data = 0;
   	m_heightfield = 0;
   	m_initialized = false;
	// loadData();
}

void Sector::loadData(SectorData::EOSMDataType type){
    if (!m_data)
    {
    	m_data = new SectorData(m_pos);
    } 
    m_data->loadData(type);
}

void Sector::createHeightfield()
{
    m_heightfield = new Heightfield(m_pos, m_size.x);
    m_heightfield->generateTriangles();
}