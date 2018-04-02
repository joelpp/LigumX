#include "TerrainTool.h"

#include <algorithm>

#include "EngineSettings.h"
#include "Editor.h"
#include "PickingTool.h"
#include "Logging.h"
#include "World.h"
#include "LigumX.h"
#include "InputHandler.h"
#include "Texture.h"
#include "RenderDataManager.h"

#include "RaycastingHelpers.h"

#pragma region  CLASS_SOURCE TerrainTool

#include "TerrainTool.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData TerrainTool::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(TerrainTool, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(TerrainTool, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Sector", PIDX_Sector, offsetof(TerrainTool, m_Sector), 0, LXType_Sector, true, LXType_None, 0, 0, 0, }, 
{ "Mode", PIDX_Mode, offsetof(TerrainTool, m_Mode), 0, LXType_TerrainEditionMode, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
{ "SplatMapTexture", PIDX_SplatMapTexture, offsetof(TerrainTool, m_SplatMapTexture), 0, LXType_Texture, true, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "TerrainErasureMode", PIDX_TerrainErasureMode, offsetof(TerrainTool, m_TerrainErasureMode), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "TerrainBrushSize", PIDX_TerrainBrushSize, offsetof(TerrainTool, m_TerrainBrushSize), 0, LXType_float, false, LXType_None, PropertyFlags_Adder, 0, 0, }, 
{ "XYZMask", PIDX_XYZMask, offsetof(TerrainTool, m_XYZMask), 0, LXType_glmvec4, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "ClickedTexel", PIDX_ClickedTexel, offsetof(TerrainTool, m_ClickedTexel), 0, LXType_glmivec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
};
bool TerrainTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
const std::string EnumValues_TerrainEditionMode[] = 
{
"None",
"Height",
"Color",
};

const TerrainEditionMode Indirection_TerrainEditionMode[] =
{
	TerrainEditionMode_None,
	TerrainEditionMode_Height,
	TerrainEditionMode_Color,
};

#pragma endregion  CLASS_SOURCE TerrainTool

TerrainTool::TerrainTool()
{
	m_SplatMapTexture = new Texture(48463);

}



bool TerrainTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	PickingTool* pickingTool = g_Editor->GetPickingTool();

	glm::vec3 worldPosition = pickingTool->GetAimingWorldPosition();

	g_RenderDataManager->AddAABBJobCentered(worldPosition, (int)m_TerrainBrushSize, glm::vec3(1, 0, 0));

	if (mouseButton1Down)
	{
		glm::vec2 dragDistance = g_InputHandler->GetDragDistance();;

		World* world = LigumX::GetInstance().GetWorld();
		Sector* sector = world->GetSectorByWorldPosition(worldPosition);

		Texture* tex = m_SplatMapTexture;
		glm::ivec2 texSize = tex->GetSize();
		int numTexels = texSize.x * texSize.y;
		int stride = 4;
		int numBytes = stride * numTexels;

		int brushWidth = (int)m_TerrainBrushSize;
		int brushWidthSq = brushWidth * brushWidth;
		if (m_SplatMapData.size() != numBytes)
		{
			m_SplatMapData.resize(numBytes);
		}


		glm::vec2 screenDistance = dragDistance;
		screenDistance.y *= -1;

		glm::vec3 scale = glm::vec3(g_EngineSettings->GetWorldScale()); // /*pickingTool->GetPickedEntity()->GetScale();*/

		glm::vec3 normalized = glm::fract(worldPosition / scale);

		glm::vec2 xyCoords = glm::vec2(normalized[0], normalized[1]);

		m_ClickedTexel = glm::ivec2(xyCoords * glm::vec2(tex->GetSize()));
		glm::ivec2 startTexel = m_ClickedTexel - glm::ivec2(brushWidth) / 2;
		startTexel = glm::max(startTexel, glm::ivec2(0, 0));
		startTexel = glm::min(startTexel, texSize - glm::ivec2(brushWidth));

		unsigned char* val = m_SplatMapData.data();

		int dataOffset = stride * (startTexel.y * tex->GetSize().x + startTexel.x);
		val += dataOffset;

		double maxVal = std::max(-screenDistance.y / 100, 0.f);

		glm::vec2 center = glm::vec2(0.5f, 0.5f);

		double maxHeight = maxVal * glm::length(center);
		double radius = 0.5f;

		for (int i = 0; i < brushWidth; ++i)
		{
			for (int j = 0; j < brushWidth; ++j)
			{
				int index = (int)(stride * (j * texSize.y + i));

				glm::vec2 localUV = glm::vec2(i, j) / glm::vec2(brushWidth);

				glm::vec2 centeredUV = localUV - center;
				double horizDist = glm::length(centeredUV);;

				float height = 0;

				if (horizDist < radius)
				{
					height = (~(0));
				}

				if (index < 0 || index > numBytes)
				{
					continue;
				}

				for (int c = 0; c < 4; ++c)
				{
					unsigned char& value = val[index + c];
					int toAdd = (int)m_XYZMask[c];

					if (GetTerrainErasureMode() && toAdd != 0)
					{
						value = 0;
					}
					else if (m_XYZMask.w == 0) // adding 
					{
						value += (value == 255) ? 0 : (char)toAdd;
					}
					else if (m_XYZMask.w == 1) // subtracting
					{
						value -= (value == 0) ? 0 : (char)toAdd;
					}

				}
			}
		}

		Renderer* renderer = LigumX::GetRenderer();
		renderer->Bind2DTexture(0, tex->GetHWObject());
		GLuint format = tex->GetFormat();
		GLuint type = tex->GetPixelType();

		//glTexSubImage2D(GL_TEXTURE_2D, 0, startTexel.x, startTexel.y, brushWidth, brushWidth, format, type, val);
		int startPoint = 0;
		glTexSubImage2D(GL_TEXTURE_2D, 0, startPoint, startPoint, texSize.x, texSize.y, format, type, m_SplatMapData.data());
		//tex->SaveToFile("C:\\temp\\output.png");

		renderer->Bind2DTexture(0, 0);
	}

	return false;
}
