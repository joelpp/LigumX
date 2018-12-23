#include "stdafx.h"

#include "heightfield.h"
#include "Settings.h"
#include "Mesh.h"
#include "Node.h"
#include "Way.h"
#include "World.h"
#include "LigumX.h"
#include "LineSegment.h"
#include "Sector.h"
#include "PerlinNoise.h"
#include "EngineSettings.h"
#include <sstream>
using namespace std;
#pragma region  CLASS_SOURCE Heightfield

#include "Heightfield.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
#include "Texture.h"
const ClassPropertyData Heightfield::g_Properties[] = 
{
{ "HeightDataTexture", PIDX_HeightDataTexture, offsetof(Heightfield, m_HeightDataTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "MaxHeight", PIDX_MaxHeight, offsetof(Heightfield, m_MaxHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MinHeight", PIDX_MinHeight, offsetof(Heightfield, m_MinHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Width", PIDX_Width, offsetof(Heightfield, m_Width), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
};
bool Heightfield::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Heightfield::ShowPropertyGrid()
{
	LXIMGUI_SHOW_OBJECTREF("HeightDataTexture", m_HeightDataTexture, Texture);
	LXIMGUI_SHOW_FLOAT("MaxHeight", m_MaxHeight, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_FLOAT("MinHeight", m_MinHeight, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_INT("Width", m_Width, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	return true;
}

#pragma endregion  CLASS_SOURCE Heightfield
using namespace glm;

inline double lerp(double a, double b, double t){ return a * t + b * (1 - t); }

Mesh* Heightfield::hfBaseMesh;
PerlinNoise* Heightfield::pNoise;

Heightfield::Heightfield()
{
	m_Width = 2048;

}

Heightfield::Heightfield(glm::vec2 offsetIndex)
{
	m_MaxHeight = -1000;
	m_MinHeight = 1000;
	
	m_HeightData.resize(m_Width * m_Width);



	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			float last = m_Width - 1.f;
			float x = 1.f - (float)i / last;
			float y = (float)j / last;

			glm::vec2 wsPos = (glm::vec2(offsetIndex) + glm::vec2(x, y));

			if (!pNoise)
			{
				pNoise = new PerlinNoise(1, 10, 1, 1, 5847);
			}

			float z = 0;

			if (!(g_EngineSettings->GetGenerateFlatTerrain()))
			{
				z = pNoise->GetHeight(wsPos.x, wsPos.y);
			}

			z *= 500.f;

			m_MaxHeight = glm::max(m_MaxHeight, z);
			m_MinHeight = glm::min(m_MinHeight, z);

			m_HeightData[j * m_Width + i] = z;
		}
	}

	m_HeightDataTexture = g_ObjectManager->CreateObject<Texture>();

	m_HeightDataTexture->SetSize(glm::ivec2(m_Width));
	m_HeightDataTexture->SetBitsPerPixel(32);
	m_HeightDataTexture->SetFormat(GLPixelFormat_RED);
	m_HeightDataTexture->SetInternalFormat(GLPixelFormat_R32F);
	m_HeightDataTexture->SetPixelType(GLPixelType_Float);

	m_HeightDataTexture->SetWrapS(GL::TextureWrapMode::ClampToEdge);
	m_HeightDataTexture->SetWrapR(GL::TextureWrapMode::ClampToEdge);
	m_HeightDataTexture->SetWrapT(GL::TextureWrapMode::ClampToEdge);

	m_HeightDataTexture->GenerateFromData(m_HeightData);
}

Heightfield::Heightfield(vec2 startPoint, float sideLength)
{
    //this->sideLength = sideLength;
    //this->startPoint = startPoint;
    //this->step = Settings::GetInstance().f("HeightfieldResolution");
    //this->iWidth = 1 + (int) (sideLength / step);
    //m_mesh = 0;
    //
    //if (!hfBaseMesh)
    //{
    //    generateBaseMesh();
    //    pNoise = new PerlinNoise(100, 0.1, 2, 3, 5847);

    //}
}

void Heightfield::generateBaseMesh()
{
    hfBaseMesh = new Mesh();
    std::vector<glm::vec3>& points = hfBaseMesh->m_buffers.m_VertexPositions;
    std::vector<glm::vec2>& UVs = hfBaseMesh->m_buffers.m_vertexUVs;
    std::vector<int>& indexBuffer = hfBaseMesh->m_buffers.indexBuffer;
    
    int c = 0;

    for(int i = 0; i < m_Width; i++)
    {
        for (int j = 0; j < m_Width; j++)
        {
            float last = (float) (m_Width - 1);
            float x = (float)i / (float)last;
            float y = (float)j / (float)last;
            vec3 point = vec3(x, y, 0);
            points.push_back(point);
            UVs.push_back(glm::vec2(x, y));
            std::cout << c++ << " " << x << " " << y << " " << std::endl;

        }
    }

    // vertically
    for (int i = 0; i < m_Width - 1; ++i)
    {
        //horizontally
        for (int j = 0; j < m_Width - 1; ++j)
        {
            int width = m_Width;
            indexBuffer.push_back( (i * width) + j );
            indexBuffer.push_back( ((i + 1) * width) + (j) );
            indexBuffer.push_back( (i * width) + (j + 1)) ;
            
            indexBuffer.push_back( ((i + 1) * width) + (j)  );
            indexBuffer.push_back( (i * width) + (j + 1)) ;
            indexBuffer.push_back( ((i + 1) * width) + (j + 1) );
            
//            for (int )
            
        }
    }

    
    hfBaseMesh->CreateBuffers();
    hfBaseMesh->SetUsesIndexBuffer(true);
    hfBaseMesh->m_PrimitiveMode = GL::PrimitiveMode::Triangles;
}

bool Heightfield::generate(){
    // for a lon, fill all the lat
    float lon;
    float lat = startPoint.y;
    for(int i = 0; i < m_Width; i++)
    {
        lon = startPoint.x;
        for (int j = 0; j < m_Width; j++)
        {
			//float z = pNoise->GetHeight(lon, lat);
			float z = 0.1f * (sin(lon) + sin(lat));

            data.push_back(z);
            lon += step;
        }
        lat += step;
    }



    glGenTextures(1, &buffer);

    glBindTexture(GL_TEXTURE_2D, buffer);
    // Set our texture parametersgl
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_Width, m_Width, 0, GL_RED, GL_FLOAT, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);


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
    glm::vec2 unNormalized = normalized * glm::vec2(m_Width, m_Width);
    // PRINTVEC2(unNormalized);

    int xIndex = (int) trunc(unNormalized.x);
    int yIndex = (int) trunc(unNormalized.y);

    glm::vec3 corners[4];
    
    corners[0].x = startPoint.x + unNormalized.x;
    corners[0].y = startPoint.y + unNormalized.y;
    
    corners[1].x = startPoint.x + unNormalized.x + step;
    corners[1].y = startPoint.y + unNormalized.y;
    
    corners[2].x = startPoint.x + unNormalized.x;
    corners[2].y = startPoint.y + unNormalized.y + step;
    
    corners[3].x = startPoint.x + unNormalized.x + step;
    corners[3].y = startPoint.y + unNormalized.y + step;
    
    corners[0].z = data[ yIndex * m_Width + xIndex ];
    corners[1].z = data[ yIndex * m_Width + xIndex + 1 ];
    corners[2].z = data[ (yIndex + 1) * m_Width + xIndex ];
    corners[3].z = data[ (yIndex + 1) * m_Width + xIndex + 1];

    // PRINTVEC3(corner[0]);
    // PRINTVEC3(corner[1]);
    // PRINTVEC3(corner[2]);
    // PRINTVEC3(corner[3]);
    return bilerp(unNormalized - glm::vec2(xIndex, yIndex), corners);
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
        Node* n0 = NULL;
        Node* n1 = NULL;

        // needed to start actually counting at 1
        int counter = 1;

        int numOfIntersections = 0;
        bool intersects = false;
        bool first = true;
        // Loop on all nodes on the best contour
        for (auto nodeIt = ways[0]->GetNodes().begin() ; nodeIt != ways[0]->GetNodes().end(); ++nodeIt)
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

float Heightfield::SampleHeight(const glm::vec2& normalizedPos)
{
	glm::vec2 correctedPos = glm::vec2(1.f - normalizedPos.x, normalizedPos.y);

	glm::vec2 samplingIndices = (correctedPos * (float) m_Width);

	glm::ivec2 indices = glm::ivec2(samplingIndices);
	glm::vec2 weights = glm::fract(samplingIndices);

	int index = indices.y * m_Width + indices.x;

	int indexAround[4];
	indexAround[0] = index - 1;
	indexAround[1] = index + 1;
	indexAround[2] = index - m_Width;
	indexAround[3] = index + m_Width;

	return m_HeightData[index];
}




