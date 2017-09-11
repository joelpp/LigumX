#include "stdafx.h"

#include "LigumX.h"
#include "Renderer.h"
#include "World.h"
void undisplayAllWays(void *clientData){
    for( auto it = Renderer::GetInstance().displayElementType.begin(); it != Renderer::GetInstance().displayElementType.end(); ++it){
        it->second = false;
    }
}

void toggle_entityland(void *clientData) {
    LigumX::GetInstance().toggleEntityLand();
}

void LigumX::init_tweakBar()
{
    //Renderer& renderer = Renderer::GetInstance();
    //TwInit(TW_OPENGL_CORE, NULL);
    //TwWindowSize(renderer.windowWidth, renderer.windowHeight);

    //myBar = TwNewBar("NameOfMyTweakBar");
    //TwAddVarRW(myBar, "Draw Building Sides", TW_TYPE_BOOLCPP, &drawBuildingSides, NULL);
    //TwAddButton(myBar, "To EntityLand", toggle_entityland, NULL, NULL);
    //TwAddVarRW(myBar, "Draw Ground", TW_TYPE_BOOLCPP, &(renderer.drawGround), NULL);

    //TwEnumVal CameraTypeEV[] = { {Camera::CameraType::AIRPLANE, "Airplane"}, {Camera::CameraType::AROUND_TARGET, "Around Target"}, {Camera::CameraType::CYLINDRICAL, "Cynlindrical"}, {Camera::CameraType::TOP_2D, "Top 2D"}, {Camera::CameraType::TOP_3D, "Top 3D"} };
    //TwType CameraTwType;
    //CameraTwType = TwDefineEnum("CameraType", CameraTypeEV, 5);
    //TwAddVarRW(myBar, "Camera Type", CameraTwType, &(camera->cameraType), NULL);

    //TwEnumVal ControlTypeEV[] = { {Camera::ControlType::QWEASDZXC_DRAG, "QWEASDZXC Drag"}, {Camera::ControlType::QWEASDZXC_CONTINUOUS, "QWEASDZXC Continuous"}, {Camera::ControlType::QWEASDZXC_ARROWS, "QWEASDZXC Arrows"} };
    //TwType ControlTwType;
    //ControlTwType = TwDefineEnum("ControlType", ControlTypeEV, 3);
    //TwAddVarRW(myBar, "Control Type", ControlTwType, &(camera->controlType), NULL);

    //TwAddVarRW(myBar, "Camera Speed", TW_TYPE_FLOAT, &camera->keyMovementSpeed, NULL);
    //TwAddVarRW(myBar, "Camera X", TW_TYPE_FLOAT, &camera->position.x, NULL);
    //TwAddVarRW(myBar, "Camera Y", TW_TYPE_FLOAT, &camera->position.y, NULL);
    //TwAddVarRW(myBar, "Camera Z", TW_TYPE_FLOAT, &camera->position.z, NULL);
    //TwAddVarRW(myBar, "Camera FOV", TW_TYPE_FLOAT, &camera->totalViewAngleY, NULL);
    //// TwAddVarRW(myBar, "Building Height", TW_TYPE_FLOAT, &buildingHeight, NULL);
    //// TwAddVarRW(myBar, "Building Side Scale", TW_TYPE_FLOAT, &buildingSideScaleFactor, NULL);
    //TwAddVarRW(myBar, "Sun Direction", TW_TYPE_FLOAT, &world->sun.orientation, "step=0.001");
    //TwAddVarRW(myBar, "Sun Time", TW_TYPE_FLOAT, &world->m_globalTime, "step=0.001");
    //TwAddVarRW(myBar, "Sun Speed", TW_TYPE_FLOAT, &world->sun.speed, "step=0.001");
    //TwAddVarRW(myBar, "Move Sun Automatically", TW_TYPE_BOOL8, &world->sun.moveAuto, NULL);
    //// // TwAddVarRW(myBar, "Save Screenshot", TW_TYPE_BOOL8, &Renderer::saveScreenshot, NULL);
    //// TwAddVarRW(myBar, "Show Text", TW_TYPE_BOOL8, &(renderer.showText), NULL);

    // TwAddButton(myBar, "Show none", undisplayAllWays, NULL, NULL);
    // for (auto it = renderer.displayElementType.begin(); it != renderer.displayElementType.end(); ++it){
    //     PRINT(labelFromType(it->first));
    //     TwAddVarRW(myBar, labelFromType(it->first).c_str(), TW_TYPE_BOOL8, &(it->second), NULL);

    // }
}
