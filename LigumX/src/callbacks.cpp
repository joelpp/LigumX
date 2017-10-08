#include "stdafx.h"

#include "LigumX.h"
#include "World.h"
#include "SectorManager.h"
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::vector;
using std::string;

using namespace glm;


void LigumX::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    LigumX::GetInstance().running = false;
}

void LigumX::glfwMouseScrollCallback(GLFWwindow* /*pWindow*/, double xOffset, double yOffset)
{
        static const float factor = 1.1;
        if(yOffset < 0) {
            LigumX::GetInstance().camera->multViewSizeBy(factor);
        } else {
            LigumX::GetInstance().camera->multViewSizeBy(1.f/factor);
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
    game.entityManager.KeyCallback(key, action);

    if(action == GLFW_PRESS){
        if (key == GLFW_KEY_SPACE) 
		{ 
			Renderer::GetInstance().m_ShowGUI = !Renderer::GetInstance().m_ShowGUI;
		}
        else if (key == GLFW_KEY_ESCAPE) 
		{
			if (LigumX::GetInstance().camera->controlType == Camera::ControlType::QWEASDZXC_CONTINUOUS)
			{
				game.camera->controlType = Camera::ControlType::QWEASDZXC_DRAG;
				//Renderer::GetInstance().m_ShadowCamera->controlType = Camera::ControlType::QWEASDZXC_DRAG;
			}
        }
		else if (key == GLFW_KEY_R)
		{ 
			Renderer::GetInstance().init_pipelines(); 
		}
		else if (key == GLFW_KEY_M)
		{
            for( auto it = Renderer::GetInstance().displayElementType.begin(); it != Renderer::GetInstance().displayElementType.end(); ++it)
			{
                it->second = false;
            }
        }
		else if(key == GLFW_KEY_F4)
		{
			game.toggleEntityLand();
        }
		else if (key == GLFW_KEY_L)
		{
			game.world->m_sectorManager->setLoadNewSectors(game.world->m_sectorManager->getLoadNewSectors());
		}

    }
    LigumX::GetInstance().camera->handlePresetKey(pWindow, key, scancode, action, mods);
	//Renderer::GetInstance().m_ShadowCamera->handlePresetKey(pWindow, key, scancode, action, mods);
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
				 Renderer::GetInstance().SetMouseClickPosition(glm::vec2(x, y));
			 }
        }
        //Right Click
        else if (button == GLFW_MOUSE_BUTTON_2){
            if (action == GLFW_PRESS)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

//                LigumX::GetInstance().draggingCamera = true;
//                double x; double y;
//                glfwGetCursorPos(pWindow, &x, &y);
//                LigumX::GetInstance().oldMousePosition = vec2(x,y);
            }
            else if (action == GLFW_RELEASE)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//                LigumX::GetInstance().draggingCamera = false;
            }
        }

		LigumX::GetInstance().camera->handlePresetMouseButton(pWindow, button, action, mods);
		//Renderer::GetInstance().m_ShadowCamera->handlePresetMouseButton(pWindow, button, action, mods);
//        TODO
        //        LigumX::GetInstance().renderer->updateMVPMatrix();
}

void LigumX::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
        if (LigumX::GetInstance().draggingCamera){
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);
            vec2 offset = vec2(x,y) - LigumX::GetInstance().oldMousePosition;
            offset.y *= -1; // reversed controls? this should be an option

            LigumX::GetInstance().camera->translateBy(vec3(offset/1000.f,0));
			//Renderer::GetInstance().m_ShadowCamera->translateBy(vec3(offset / 1000.f, 0));

			LigumX::GetInstance().oldMousePosition = vec2(x, y);

        }
		LigumX::GetInstance().camera->handlePresetCursorPos(pWindow, x, y);
		//Renderer::GetInstance().m_ShadowCamera->handlePresetCursorPos(pWindow, x, y);
}
