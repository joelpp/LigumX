#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Logging.h"
#include "Renderer.h"

#pragma region  FORWARD_DECLARATIONS Heightfield
#include "LXSystem.h"

class Heightfield;
class Texture;


#pragma endregion  FORWARD_DECLARATIONS Heightfield
class Mesh;
class Way;
class Node;
class PerlinNoise;


#pragma region  HEADER Heightfield
class Heightfield : public LXObject
{
public:
static const int ClassID = 3816013350;
static const LXType Type = LXType_Heightfield;
static constexpr const char* ClassName = "Heightfield";

Texture*& GetHeightDataTexture() { return m_HeightDataTexture; }; 
void SetHeightDataTexture(Texture* value) { m_HeightDataTexture = value; }; 
float GetMaxHeight() { return m_MaxHeight; }; 
void SetMaxHeight(float value) { m_MaxHeight = value; }; 
float GetMinHeight() { return m_MinHeight; }; 
void SetMinHeight(float value) { m_MinHeight = value; }; 
int GetWidth() { return m_Width; }; 
void SetWidth(int value) { m_Width = value; }; 
private:
Texture* m_HeightDataTexture = nullptr;
float m_MaxHeight = 0.f;
float m_MinHeight = 0.f;
int m_Width = 64;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_HeightfieldPIDX
{
PIDX_HeightDataTexture,
PIDX_MaxHeight,
PIDX_MinHeight,
PIDX_Width,
};
bool Serialize(bool writing);
void ShowPropertyGrid();

#pragma endregion  HEADER Heightfield

public:
	Heightfield();
	Heightfield(glm::vec2 offsetIndex);
    Heightfield(glm::vec2 startPoint, float sideLength);

    void generateBaseMesh();
    
    bool generate();
	static double contourLineInterpolate(glm::vec2 xy, std::vector<Way*>& contourLines);
	static int getLerpedContourLines(glm::vec2 xy, std::vector<Way*> ways, std::vector<glm::vec2> directions, 
                                       			  std::vector<std::pair<Node*, Node*>> nodePairs);
	double getHeight(glm::vec2 xy);

	float SampleHeight(const glm::vec2& normalizedPos);

	std::vector<float>& GetHeightData() { return m_HeightData; }

private:
    Mesh* m_mesh;
    
    float step;
    float sideLength;
    std::vector<float> data;
    GLuint buffer;
    glm::vec2 startPoint;
    
    static Mesh* hfBaseMesh;
    static PerlinNoise* pNoise;

	std::vector<float> m_HeightData;

};

