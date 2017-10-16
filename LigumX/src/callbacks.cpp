#include "stdafx.h"

#include "LigumX.h"
#include "World.h"
#include "SectorManager.h"
#include "Renderer.h"
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::vector;
using std::string;

using namespace glm;

Renderer* LigumX::m_Renderer;

void LigumX::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    LigumX::GetInstance().Shutdown();
}

void LigumX::glfwMouseScrollCallback(GLFWwindow* /*pWindow*/, double xOffset, double yOffset)
{
        static const float factor = 1.1;
        if(yOffset < 0) {
            m_Renderer->GetDebugCamera()->multViewSizeBy(factor);
        } else {
			m_Renderer->GetDebugCamera()->multViewSizeBy(1.f/factor);
        }
}

void flipBool(bool& value)
{
	value = !value;
}

void LigumX::HandleKeyboardInput(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    // send event to entity Manager (temporary before a playerInput class)
	LigumX& game = LigumX::GetInstance();

    if(action == GLFW_PRESS){
        if (key == GLFW_KEY_SPACE) 
		{ 
			m_Renderer->m_ShowGUI = !m_Renderer->m_ShowGUI;
		}
        else if (key == GLFW_KEY_ESCAPE) 
		{
			if (m_Renderer->GetDebugCamera()->controlType == Camera::ControlType::QWEASDZXC_CONTINUOUS)
			{
				m_Renderer->GetDebugCamera()->controlType = Camera::ControlType::QWEASDZXC_DRAG;
			}
        }
		else if (key == GLFW_KEY_R)
		{ 
			m_Renderer->init_pipelines(); 
		}
		else if (key == GLFW_KEY_M)
		{
            for( auto it = m_Renderer->displayElementType.begin(); it != m_Renderer->displayElementType.end(); ++it)
			{
                it->second = false;
            }
        }
		else if(key == GLFW_KEY_F4)
		{
        }
		else if (key == GLFW_KEY_L)
		{
			game.world->m_sectorManager->setLoadNewSectors(game.world->m_sectorManager->getLoadNewSectors());
		}

    }
	m_Renderer->GetDebugCamera()->handlePresetKey(pWindow, key, scancode, action, mods);
}

void LigumX::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
	bool caughtByImgui = ImGui::IsMouseHoveringAnyWindow();
        // Left-click
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS){
             double x,y;
             glfwGetCursorPos(pWindow, &x, &y);
            // vec2 worldPos = LigumX::GetInstance().windowPosToWorldPos(vec2(x,y));
            // int index = 0;
            // std::vector<Way*> closests;
            // std::vector<double> distances;
            // std::vector<vec2> directions;
            // vector<std::pair<Node*, Node*>> nodePairs;
            // int filter = OSMElement::CONTOUR;
            // TIME(closests = LigumX::GetInstance().findNClosestWays(2, worldPos, filter, distances, directions, nodePairs));

            // if (closests[0] == NULL) return;
            // TIME(LigumX::GetInstance().updateSelectedWay(closests[0]));
            // PRINTELEMENTVECTOR(closests);

			 if (!caughtByImgui)
			 {
				 m_Renderer->SetMouseClickPosition(glm::vec2(x, y));
			 }
        }
        //Right Click
        else if (button == GLFW_MOUSE_BUTTON_2){
            if (action == GLFW_PRESS)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else if (action == GLFW_RELEASE)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

		m_Renderer->GetDebugCamera()->handlePresetMouseButton(pWindow, button, action, mods);
}

void LigumX::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
        if (false)
		{

            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);
            vec2 offset = vec2(x,y) - LigumX::GetInstance().oldMousePosition;
            offset.y *= -1; // reversed controls? this should be an option

			m_Renderer->GetDebugCamera()->translateBy(vec3(offset/1000.f,0));
			//m_Renderer->m_ShadowCamera->translateBy(vec3(offset / 1000.f, 0));

			LigumX::GetInstance().oldMousePosition = vec2(x, y);

        }
		m_Renderer->GetDebugCamera()->handlePresetCursorPos(pWindow, x, y);
}
