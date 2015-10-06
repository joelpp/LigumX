#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"


// 2D orthographic camera, look in the -Z direction.
class Camera
{
//    enum class ViewMode {TOP2D, TOP3D, BIRD3D};
public:
    enum CameraType {AIRPLANE, AROUND_TARGET, CYLINDRICAL,
                     TOP_2D, TOP_3D};
    enum ControlType {QWEASDZXC_DRAG, QWEASDZXC_CONTINUOUS, QWEASDZXC_ARROWS};
public:
    glm::vec3 position;
    glm::mat4 mvpMat;

    // TOP2D ad TOP3D parameters
    float viewSize;
    float angle;

    // 3D parameters
    CameraType cameraType;
    ControlType controlType;
    bool mouseIsDragging;
    glm::vec2 mouseDragReferencePosition;
    glm::vec3 frontVec, upVec, rightVec;
    glm::vec3 frontVecReference, upVecReference, rightVecReference;
    glm::vec3 positionReference;
    float distanceReference;
    glm::mat4 tempExtraRotationMat;
    glm::vec3 lookAtTargetPos, cylindricalUpVec;
    float viewMovementSpeed, defaultViewMovementSpeed;
    float keyMovementSpeed, defaultKeyMovementSpeed, keyMovementSpeedIncreaseFactor;
    float totalViewAngleY;
    float aspectRatio;
    float near, far;


public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 lookAt, float viewSize);
    void translateBy(glm::vec3 delta);
    void translateTo(glm::vec3 position);
    void moveFromUserInput(GLFWwindow* pWindow);
    void updateMVPMatrix();
    void setViewSize(float inViewSize);
    void multViewSizeBy(float factor);
    void rotate(float _angle);

    void handlePresetMouseButton(
                GLFWwindow* pWindow, int button, int action, int mods);
    void handlePresetCursorPos(GLFWwindow* pWindow, double x, double y);
    void handlePresetKey(
                GLFWwindow* pWindow, int key, int scancode, int action, int mods);
    void handlePresetNewFrame(GLFWwindow* pWindow);

    void dragMousePresetButton(
            GLFWwindow* pWindow, int button, int action, int mods);
    void dragMousePresetCursorPos(
            GLFWwindow *pWindow, double x, double y);
    void continuousMousePresetCursorPos(
            GLFWwindow *pWindow, double x, double y);
    void qweasdzxcKeyPreset(
            GLFWwindow *pWindow, int key, int scancode, int action, int mods);
    void qweasdzxcKeyHoldPreset(GLFWwindow* pWindow);
    void viewArrowsPreset(GLFWwindow* pWindow);

};

#endif // CAMERA_H
