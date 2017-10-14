#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "Logging.h"

#pragma region  FORWARD_DECLARATIONS Camera
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS Camera
// 2D orthographic camera, look in the -Z direction.3

enum ProjectionType
{
	ProjectionType_Perspective,
	ProjectionType_Orthographic
};

class Camera
{
public:
public:
public:
#pragma region  HEADER Camera
public:
static const int ClassID = 3880549230;
const glm::vec3& GetPosition() { return m_Position; }; 
void SetPosition(glm::vec3 value) { m_Position = value; }; 
const glm::vec3& GetsomeBullshitVec3() { return m_someBullshitVec3; }; 
void SetsomeBullshitVec3(glm::vec3 value) { m_someBullshitVec3 = value; }; 
const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }; 
void SetViewMatrix(glm::mat4 value) { m_ViewMatrix = value; }; 
const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }; 
void SetProjectionMatrix(glm::mat4 value) { m_ProjectionMatrix = value; }; 
const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }; 
void SetViewProjectionMatrix(glm::mat4 value) { m_ViewProjectionMatrix = value; }; 
const float& GetNearPlane() { return m_NearPlane; }; 
void SetNearPlane(float value) { m_NearPlane = value; }; 
const float& GetFarPlane() { return m_FarPlane; }; 
void SetFarPlane(float value) { m_FarPlane = value; }; 
const ProjectionType& GetProjectionType() { return m_ProjectionType; }; 
void SetProjectionType(ProjectionType value) { m_ProjectionType = value; }; 
const float& GetOrthoBorders() { return m_OrthoBorders; }; 
void SetOrthoBorders(float value) { m_OrthoBorders = value; }; 
private:
glm::vec3 m_Position;
glm::vec3 m_someBullshitVec3;
glm::mat4 m_ViewMatrix;
glm::mat4 m_ProjectionMatrix;
glm::mat4 m_ViewProjectionMatrix;
float m_NearPlane;
float m_FarPlane;
ProjectionType m_ProjectionType;
float m_OrthoBorders;
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CameraPIDX
{
PIDX_Position,
PIDX_someBullshitVec3,
PIDX_ViewMatrix,
PIDX_ProjectionMatrix,
PIDX_ViewProjectionMatrix,
PIDX_NearPlane,
PIDX_FarPlane,
PIDX_ProjectionType,
PIDX_OrthoBorders,
};
void Serialize(bool writing);

#pragma endregion  HEADER Camera

//    enum class ViewMode {TOP2D, TOP3D, BIRD3D};
public:
    enum CameraType {AIRPLANE, AROUND_TARGET, CYLINDRICAL,
                     TOP_2D, TOP_3D};
    enum ControlType {QWEASDZXC_DRAG, QWEASDZXC_CONTINUOUS, QWEASDZXC_ARROWS};
public:
    //glm::vec3 position;
	glm::mat4 vpMat;
	//glm::mat4 m_ViewMatrix;
	//glm::mat4 m_ProjectionMatrix;

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

	float minimumSpeed;
	float maximumSpeed;

public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 lookAt, float viewSize);
    void translateBy(glm::vec3 delta);
    void translateTo(glm::vec3 position);
    void moveFromUserInput(GLFWwindow* pWindow);
    void updateVPMatrix();
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
private:
    REGISTERCLASS(Camera);
};

#endif // CAMERA_H
