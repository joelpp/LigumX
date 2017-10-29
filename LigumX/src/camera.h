#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "Logging.h"

#pragma region  FORWARD_DECLARATIONS Camera
#include "property.h"

class Camera;


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
static const LXType Type = LXType_Camera;
static constexpr const char* ClassName = "Camera";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const glm::vec3& GetPosition() { return m_Position; }; 
void SetPosition(glm::vec3 value) { m_Position = value; }; 
const glm::vec3& GetFrontVector() { return m_FrontVector; }; 
void SetFrontVector(glm::vec3 value) { m_FrontVector = value; }; 
const glm::vec3& GetRightVector() { return m_RightVector; }; 
void SetRightVector(glm::vec3 value) { m_RightVector = value; }; 
const glm::vec3& GetUpVector() { return m_UpVector; }; 
void SetUpVector(glm::vec3 value) { m_UpVector = value; }; 
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
const int& GetProjectionType() { return m_ProjectionType; }; 
void SetProjectionType(int value) { m_ProjectionType = value; }; 
const float& GetOrthoBorders() { return m_OrthoBorders; }; 
void SetOrthoBorders(float value) { m_OrthoBorders = value; }; 
const float& GetViewSize() { return m_ViewSize; }; 
void SetViewSize(float value) { m_ViewSize = value; }; 
private:
int m_ObjectID;
std::string m_Name;
glm::vec3 m_Position;
glm::vec3 m_FrontVector;
glm::vec3 m_RightVector;
glm::vec3 m_UpVector;
glm::mat4 m_ViewMatrix;
glm::mat4 m_ProjectionMatrix;
glm::mat4 m_ViewProjectionMatrix;
float m_NearPlane;
float m_FarPlane;
int m_ProjectionType;
float m_OrthoBorders;
float m_ViewSize;
public:
static const int g_PropertyCount = 14;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CameraPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Position,
PIDX_FrontVector,
PIDX_RightVector,
PIDX_UpVector,
PIDX_ViewMatrix,
PIDX_ProjectionMatrix,
PIDX_ViewProjectionMatrix,
PIDX_NearPlane,
PIDX_FarPlane,
PIDX_ProjectionType,
PIDX_OrthoBorders,
PIDX_ViewSize,
};
bool Serialize(bool writing);
void PostSerialization(bool writing);

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
    float angle;

    // 3D parameters
    CameraType cameraType;
    ControlType controlType;
    bool mouseIsDragging;
    glm::vec2 mouseDragReferencePosition;
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
    void translateBy(glm::vec3 delta);
    void translateTo(glm::vec3 position);
    void moveFromUserInput(GLFWwindow* pWindow);
    void updateVPMatrix();
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
