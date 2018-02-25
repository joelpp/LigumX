#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Logging.h"
#include "Renderer.h"

#pragma region  FORWARD_DECLARATIONS Heightfield
#include "property.h"

class Heightfield;
class Texture;


#pragma endregion  FORWARD_DECLARATIONS Heightfield
class Triangle;
class Mesh;
class Way;
class Node;
class PerlinNoise;


class Heightfield
{
#pragma region  HEADER Heightfield
public:
static const int ClassID = 3816013350;
static const LXType Type = LXType_Heightfield;
static constexpr const char* ClassName = "Heightfield";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
Texture*& GetHeightDataTexture() { return m_HeightDataTexture; }; 
void SetHeightDataTexture(Texture* value) { m_HeightDataTexture = value; }; 
private:
int m_ObjectID;
std::string m_Name;
Texture* m_HeightDataTexture;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_HeightfieldPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_HeightDataTexture,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Heightfield

public:
    Heightfield(glm::vec2 offsetIndex);
    Heightfield(glm::vec2 startPoint, float sideLength);

    void generateBaseMesh();
    
    bool generate();
	static double contourLineInterpolate(glm::vec2 xy, std::vector<Way*>& contourLines);
	static int getLerpedContourLines(glm::vec2 xy, std::vector<Way*> ways, std::vector<glm::vec2> directions, 
                                       			  std::vector<std::pair<Node*, Node*>> nodePairs);
	double getHeight(glm::vec2 xy);

	float SampleHeight(const glm::vec2& normalizedPos);


private:
    Mesh* m_mesh;
    
    int m_Width;
    float step;
    float sideLength;
    std::vector<Triangle* > triangles;
    std::vector<float> data;
    GLuint buffer;
    glm::vec2 startPoint;
    
    static Mesh* hfBaseMesh;
    static PerlinNoise* pNoise;

	std::vector<float> m_HeightData;

};

