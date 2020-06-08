#ifndef CAMERA_H
#define CAMERA_H

#pragma region  FORWARD_DECLARATIONS Camera
#include "LXSystem.h"
class Serializer2;

class Camera;


#pragma endregion  FORWARD_DECLARATIONS Camera
// 2D orthographic camera, look in the -Z direction.3

enum ProjectionType
{
	ProjectionType_Perspective,
	ProjectionType_Orthographic
};

#pragma region  HEADER Camera
class Camera : public LXObject
{
public:
static const int ClassID = 1181213486;
static const LXType Type = LXType_Camera;
static constexpr const char* ClassName = "Camera";
virtual LXType GetLXType() { return LXType_Camera; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

glm::vec3& GetPosition() { return m_Position; }; 
void SetPosition(const glm::vec3& value) { m_Position = value; }; 
void AddTo_Position(glm::vec3 value) { m_Position += value; };
glm::vec3& GetFrontVector() { return m_FrontVector; }; 
void SetFrontVector(const glm::vec3& value) { m_FrontVector = value; }; 
glm::vec3& GetRightVector() { return m_RightVector; }; 
void SetRightVector(const glm::vec3& value) { m_RightVector = value; }; 
glm::vec3& GetUpVector() { return m_UpVector; }; 
void SetUpVector(const glm::vec3& value) { m_UpVector = value; }; 
glm::mat4& GetViewMatrix() { return m_ViewMatrix; }; 
void SetViewMatrix(const glm::mat4& value) { m_ViewMatrix = value; }; 
glm::mat4& GetViewMatrixNoTranslation() { return m_ViewMatrixNoTranslation; }; 
void SetViewMatrixNoTranslation(const glm::mat4& value) { m_ViewMatrixNoTranslation = value; }; 
glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }; 
void SetProjectionMatrix(const glm::mat4& value) { m_ProjectionMatrix = value; }; 
glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }; 
void SetViewProjectionMatrix(const glm::mat4& value) { m_ViewProjectionMatrix = value; }; 
glm::mat4& GetViewMatrixInverse() { return m_ViewMatrixInverse; }; 
void SetViewMatrixInverse(const glm::mat4& value) { m_ViewMatrixInverse = value; }; 
glm::mat4& GetProjectionMatrixInverse() { return m_ProjectionMatrixInverse; }; 
void SetProjectionMatrixInverse(const glm::mat4& value) { m_ProjectionMatrixInverse = value; }; 
glm::mat4& GetViewProjectionMatrixInverse() { return m_ViewProjectionMatrixInverse; }; 
void SetViewProjectionMatrixInverse(const glm::mat4& value) { m_ViewProjectionMatrixInverse = value; }; 
float GetNearPlane() { return GetNearPlane_Callback(); }
float GetNearPlane_Callback();
void SetNearPlane(float value) { m_NearPlane = value; }; 
float GetFarPlane() { return m_FarPlane; }; 
void SetFarPlane(float value) { m_FarPlane = value; }; 
int GetProjectionType() { return m_ProjectionType; }; 
void SetProjectionType(int value) { m_ProjectionType = value; }; 
float GetOrthoBorders() { return m_OrthoBorders; }; 
void SetOrthoBorders(float value) { m_OrthoBorders = value; }; 
float GetViewSize() { return m_ViewSize; }; 
void SetViewSize(float value) { m_ViewSize = value; }; 
float GetMovementSpeed() { return m_MovementSpeed; }; 
void SetMovementSpeed(float value) { m_MovementSpeed = value; }; 
float GetKeyMovementSpeedIncreaseFactor() { return m_KeyMovementSpeedIncreaseFactor; }; 
void SetKeyMovementSpeedIncreaseFactor(float value) { m_KeyMovementSpeedIncreaseFactor = value; }; 
float GetFOVY() { return m_FOVY; }; 
void SetFOVY(float value) { m_FOVY = value; }; 
bool GetFPSCamera() { return m_FPSCamera; }; 
void SetFPSCamera(bool value) { m_FPSCamera = value; }; 
private:
glm::vec3 m_Position = glm::vec3(0, 0, 0);
glm::vec3 m_FrontVector = glm::vec3(0, 0, 0);
glm::vec3 m_RightVector = glm::vec3(0, 0, 0);
glm::vec3 m_UpVector = glm::vec3(0, 0, 0);
glm::mat4 m_ViewMatrix;
glm::mat4 m_ViewMatrixNoTranslation;
glm::mat4 m_ProjectionMatrix;
glm::mat4 m_ViewProjectionMatrix;
glm::mat4 m_ViewMatrixInverse;
glm::mat4 m_ProjectionMatrixInverse;
glm::mat4 m_ViewProjectionMatrixInverse;
float m_NearPlane = 0.f;
float m_FarPlane = 0.f;
int m_ProjectionType = 0;
float m_OrthoBorders = 0.f;
float m_ViewSize = 0.f;
float m_MovementSpeed = 0.f;
float m_KeyMovementSpeedIncreaseFactor = 1.5f;
float m_FOVY = 45.f;
bool m_FPSCamera = false;
public:
static const int g_PropertyCount = 20;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CameraPIDX
{
PIDX_Position,
PIDX_FrontVector,
PIDX_RightVector,
PIDX_UpVector,
PIDX_ViewMatrix,
PIDX_ViewMatrixNoTranslation,
PIDX_ProjectionMatrix,
PIDX_ViewProjectionMatrix,
PIDX_ViewMatrixInverse,
PIDX_ProjectionMatrixInverse,
PIDX_ViewProjectionMatrixInverse,
PIDX_NearPlane,
PIDX_FarPlane,
PIDX_ProjectionType,
PIDX_OrthoBorders,
PIDX_ViewSize,
PIDX_MovementSpeed,
PIDX_KeyMovementSpeedIncreaseFactor,
PIDX_FOVY,
PIDX_FPSCamera,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
void PostSerialization(bool writing, bool success);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER Camera

//    enum class ViewMode {TOP2D, TOP3D, BIRD3D};
public:
    enum CameraType {AIRPLANE, AROUND_TARGET, CYLINDRICAL,
                     TOP_2D, TOP_3D};
    enum ControlType {QWEASDZXC_DRAG, QWEASDZXC_CONTINUOUS, QWEASDZXC_ARROWS};
public:
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
    float keyMovementSpeed, defaultKeyMovementSpeed;
    float aspectRatio;

	float minimumSpeed;
	float maximumSpeed;

public:
    Camera();
    void translateBy(glm::vec3 delta);
    void translateTo(glm::vec3 position);
    void moveFromUserInput(GLFWwindow* pWindow);
    void UpdateVPMatrix();
    void rotate(float _angle);

	void NormalizeVectors();


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
