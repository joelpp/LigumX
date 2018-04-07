#include "TerrainTool.h"

#include <algorithm>

#include "EngineSettings.h"
#include "Editor.h"
#include "Sector.h"
#include "Heightfield.h"
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
{ "Mode", PIDX_Mode, offsetof(TerrainTool, m_Mode), 0, LXType_TerrainEditionMode, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
{ "TerrainErasureMode", PIDX_TerrainErasureMode, offsetof(TerrainTool, m_TerrainErasureMode), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "TerrainBrushSize", PIDX_TerrainBrushSize, offsetof(TerrainTool, m_TerrainBrushSize), 0, LXType_float, false, LXType_None, PropertyFlags_Adder, 0, 0, }, 
{ "XYZMask", PIDX_XYZMask, offsetof(TerrainTool, m_XYZMask), 0, LXType_glmvec4, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "ClickedTexel", PIDX_ClickedTexel, offsetof(TerrainTool, m_ClickedTexel), 0, LXType_glmivec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "StartTexel", PIDX_StartTexel, offsetof(TerrainTool, m_StartTexel), 0, LXType_glmivec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "SplatMapIncrement", PIDX_SplatMapIncrement, offsetof(TerrainTool, m_SplatMapIncrement), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "ClickedSector", PIDX_ClickedSector, offsetof(TerrainTool, m_ClickedSector), 0, LXType_Sector, true, LXType_None, PropertyFlags_Transient, 0, 0, }, 
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
}

glm::ivec2 WorldToTexture(glm::vec2 worldPos, float scale, glm::ivec2 texSize)
{
	glm::vec2 normalized = glm::fract(worldPos / scale);

	return glm::ivec2(normalized * glm::vec2(texSize));
}

float TextureToWorldScale(float scale, glm::ivec2 texSize)
{
	return scale / texSize.x;
}

bool TerrainTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	PickingTool* pickingTool = g_Editor->GetPickingTool();

	if (pickingTool->GetAimingID() == 0)
	{
		return false;
	}

	glm::vec3 worldPosition = pickingTool->GetAimingWorldPosition();

	World* world = LigumX::GetInstance().GetWorld();
	m_ClickedSector = world->GetSectorByWorldPosition(worldPosition);

	Texture* tex;
	
	if (m_Mode == TerrainEditionMode_Color)
	{
		tex = m_ClickedSector->GetSplatMapTexture();
	}
	else
	{
		tex = m_ClickedSector->GetHeightfield()->GetHeightDataTexture();
	}

	float worldScale = g_EngineSettings->GetWorldScale();

	glm::vec3 scale = glm::vec3(worldScale); // /*pickingTool->GetPickedEntity()->GetScale();*/

	glm::vec3 aabbCenterPoint = worldPosition;
	int aabbScale = (int)(m_TerrainBrushSize * TextureToWorldScale(worldScale, tex->GetSize()));
	
	// move aabb basis to 0
	aabbCenterPoint.z = aabbScale / 2.f;
	g_RenderDataManager->AddAABBJobCentered(worldPosition, aabbScale, glm::vec3(1,0,0));

	if (mouseButton1Down)
	{
		glm::vec2 dragDistance = g_InputHandler->GetDragDistance();;

		glm::ivec2 texSize = tex->GetSize();
		int numTexels = texSize.x * texSize.y;


		int brushWidth = (int)m_TerrainBrushSize;
		int brushWidthSq = brushWidth * brushWidth;

		glm::vec2 screenDistance = dragDistance;
		screenDistance.y *= -1;

		glm::vec3 normalized = glm::fract(worldPosition / scale);

		glm::vec2 xyCoords = glm::vec2(normalized[0], normalized[1]);

		m_ClickedTexel = WorldToTexture(glm::vec2(worldPosition), worldScale, tex->GetSize());

		m_StartTexel = m_ClickedTexel - glm::ivec2(brushWidth) / 2;
		m_StartTexel = glm::max(m_StartTexel, glm::ivec2(0, 0));
		m_StartTexel = glm::min(m_StartTexel, texSize - glm::ivec2(brushWidth));
		
		unsigned char* val = nullptr;
		
		int stride = 4;

		if (m_Mode == TerrainEditionMode_Color)
		{
			val = tex->GetTextureData();
		}
		else
		{
			val = (unsigned char*) m_ClickedSector->GetHeightfield()->GetHeightData().data();
		}

		int numBytes = stride * numTexels;

		int dataOffset = stride * (m_StartTexel.y * tex->GetSize().x + m_StartTexel.x);

		unsigned char* offsetVal = val + dataOffset;

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

				if (m_Mode == TerrainEditionMode_Color)
				{
					for (int c = 0; c < 4; ++c)
					{
						unsigned char& value = offsetVal[index + c];
						int toAdd = (int)m_XYZMask[c] * m_SplatMapIncrement * (m_XYZMask.w == 0 ? 1 : -1);

						if (GetTerrainErasureMode() && toAdd != 0)
						{
							value = 0;
						}
						
						value = std::min(255, std::max(value + toAdd, 0));
					}
				}
				else
				{
					float* value = (float*) (offsetVal + index);
					float toAdd = (float)m_XYZMask[0] * (m_XYZMask.w == 0 ? 1 : -1);

					if (GetTerrainErasureMode() && toAdd != 0)
					{
						value = 0;
					}
					else
					{
						*value = std::min(FLT_MAX, std::max(*value + toAdd, FLT_MIN));
					}
				}
			}
		}	

		Renderer* renderer = LigumX::GetRenderer();
		renderer->Bind2DTexture(0, tex->GetHWObject());
		GLuint format = tex->GetFormat();
		GLuint type = tex->GetPixelType();

		int startPoint = 0;
		glTexSubImage2D(GL_TEXTURE_2D, 0, startPoint, startPoint, texSize.x, texSize.y, format, type, val);

		renderer->Bind2DTexture(0, 0);
	}

	return false;
}

void TerrainTool::HandleKeyboardInput(int button, int action, int mods)
{
	bool isXYZ[3] = { button == GLFW_KEY_X, button == GLFW_KEY_Y, button == GLFW_KEY_Z };
	bool isPress = action == GLFW_PRESS;
	bool isRelease = action == GLFW_RELEASE;
	bool isShift = mods & GLFW_MOD_SHIFT;

	bool maskModified = isXYZ[0] || isXYZ[1] || isXYZ[2] || isShift;
	maskModified &= (isPress || isRelease);

	if (maskModified)
	{
		float add = isPress ? 1.f : -1.f;

		glm::vec3 newMask = glm::vec3(m_XYZMask);

		newMask = add * glm::vec3(isXYZ[0], isXYZ[1], isXYZ[2]) + newMask;
		newMask = glm::max(newMask, glm::vec3(-1, -1, -1));
		newMask = glm::min(newMask, glm::vec3(1, 1, 1));

		m_XYZMask = glm::vec4(newMask, isShift && isPress);
	}

	if (mods & GLFW_MOD_CONTROL)
	{
		int max = EnumLength_TerrainEditionMode;
		constexpr TerrainEditionMode ModeKeyboardToggles[EnumLength_TerrainEditionMode] =
		{
			TerrainEditionMode_None,
			TerrainEditionMode_Color,
			TerrainEditionMode_Height,
		};

		int base = GLFW_KEY_0;

		if (button > base && button < base + max)
		{
			int offset = button - base;
			TerrainEditionMode mode = ModeKeyboardToggles[offset];

			const std::string& toolName = EnumValues_TerrainEditionMode[mode];
			std::string message = "Selected terrain editin mode : " + toolName + "(" + std::to_string(offset) + ")";

			g_RenderDataManager->AddTimedMessage(message);

			m_Mode = mode;
		}
	}

}

Texture* TerrainTool::GetTextureToDisplay()
{
	if (m_ClickedSector)
	{
		if (m_Mode == TerrainEditionMode_Color)
		{
			return m_ClickedSector->GetSplatMapTexture();
		}
		else
		{
			return m_ClickedSector->GetHeightfield()->GetHeightDataTexture();
		}
	}
	return nullptr;
}

