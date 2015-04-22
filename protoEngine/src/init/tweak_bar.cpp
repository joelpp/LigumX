#include "../game.h"

void TW_CALL undisplayAllWays(void *clientData){
    for( auto it = game->displayElementType.begin(); it != game->displayElementType.end(); ++it){
        it->second = false;
    }
}

void Game::init_tweakBar()
{
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(windowWidth, windowHeight);

    myBar = TwNewBar("NameOfMyTweakBar");
    TwAddVarRW(myBar, "Fancy Display Mode", TW_TYPE_BOOLCPP, &fancyDisplayMode, NULL);
    TwAddVarRW(myBar, "Draw Building Sides", TW_TYPE_BOOLCPP, &drawBuildingSides, NULL);

    TwEnumVal CameraTypeEV[] = { {Camera::CameraType::AIRPLANE, "Airplane"}, {Camera::CameraType::AROUND_TARGET, "Around Target"}, {Camera::CameraType::CYLINDRICAL, "Cynlindrical"}, {Camera::CameraType::TOP_2D, "Top 2D"}, {Camera::CameraType::TOP_3D, "Top 3D"} };
    TwType CameraTwType;
    CameraTwType = TwDefineEnum("CameraType", CameraTypeEV, 5);
    TwAddVarRW(myBar, "Camera Type", CameraTwType, &(camera->cameraType), NULL);

    TwEnumVal ControlTypeEV[] = { {Camera::ControlType::QWEASDZXC_DRAG, "QWEASDZXC Drag"}, {Camera::ControlType::QWEASDZXC_CONTINUOUS, "QWEASDZXC Continuous"}, {Camera::ControlType::QWEASDZXC_ARROWS, "QWEASDZXC Arrows"} };
    TwType ControlTwType;
    ControlTwType = TwDefineEnum("ControlType", ControlTypeEV, 3);
    TwAddVarRW(myBar, "Control Type", ControlTwType, &(camera->controlType), NULL);

    TwAddVarRW(myBar, "Camera Speed", TW_TYPE_FLOAT, &camera->keyMovementSpeed, NULL);
    TwAddVarRW(myBar, "Building Height", TW_TYPE_FLOAT, &buildingHeight, NULL);
    TwAddVarRW(myBar, "Building Side Scale", TW_TYPE_FLOAT, &buildingSideScaleFactor, NULL);
    TwAddVarRW(myBar, "Sun Direction", TW_TYPE_FLOAT, &sunOrientation, "step=0.001");
    TwAddVarRW(myBar, "Sun Time", TW_TYPE_FLOAT, &sunTime, "step=0.001");
    TwAddVarRW(myBar, "Sun Speed", TW_TYPE_FLOAT, &sunSpeed, "step=0.001");
    TwAddVarRW(myBar, "Move Sun Automatically", TW_TYPE_BOOL8, &sunMoveAuto, NULL);
    TwAddVarRW(myBar, "Save Screenshot", TW_TYPE_BOOL8, &saveScreenshot, NULL);

    TwAddButton(myBar, "Show none", undisplayAllWays, NULL, NULL);
    for (auto it = displayElementType.begin(); it != displayElementType.end(); ++it){
        PRINT(it->first);
        TwAddVarRW(myBar, labelFromType(it->first).c_str(), TW_TYPE_BOOL8, &(it->second), NULL);

    }
}
