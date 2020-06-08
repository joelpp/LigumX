
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Settings.h"
#include "RenderDataManager.h"

#pragma region  CLASS_SOURCE Camera

#include "Camera.h"
#include "serializer.h"
const ClassPropertyData Camera::g_Properties[] = 
{
{ "Position", PIDX_Position, offsetof(Camera, m_Position), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_Adder, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "FrontVector", PIDX_FrontVector, offsetof(Camera, m_FrontVector), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "RightVector", PIDX_RightVector, offsetof(Camera, m_RightVector), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "UpVector", PIDX_UpVector, offsetof(Camera, m_UpVector), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "ViewMatrix", PIDX_ViewMatrix, offsetof(Camera, m_ViewMatrix), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "ViewMatrixNoTranslation", PIDX_ViewMatrixNoTranslation, offsetof(Camera, m_ViewMatrixNoTranslation), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "ProjectionMatrix", PIDX_ProjectionMatrix, offsetof(Camera, m_ProjectionMatrix), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "ViewProjectionMatrix", PIDX_ViewProjectionMatrix, offsetof(Camera, m_ViewProjectionMatrix), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "ViewMatrixInverse", PIDX_ViewMatrixInverse, offsetof(Camera, m_ViewMatrixInverse), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "ProjectionMatrixInverse", PIDX_ProjectionMatrixInverse, offsetof(Camera, m_ProjectionMatrixInverse), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "ViewProjectionMatrixInverse", PIDX_ViewProjectionMatrixInverse, offsetof(Camera, m_ViewProjectionMatrixInverse), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "NearPlane", PIDX_NearPlane, offsetof(Camera, m_NearPlane), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "FarPlane", PIDX_FarPlane, offsetof(Camera, m_FarPlane), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "ProjectionType", PIDX_ProjectionType, offsetof(Camera, m_ProjectionType), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "OrthoBorders", PIDX_OrthoBorders, offsetof(Camera, m_OrthoBorders), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "ViewSize", PIDX_ViewSize, offsetof(Camera, m_ViewSize), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MovementSpeed", PIDX_MovementSpeed, offsetof(Camera, m_MovementSpeed), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0.f, 30.f, 0,}, 
{ "KeyMovementSpeedIncreaseFactor", PIDX_KeyMovementSpeedIncreaseFactor, offsetof(Camera, m_KeyMovementSpeedIncreaseFactor), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "FOVY", PIDX_FOVY, offsetof(Camera, m_FOVY), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "FPSCamera", PIDX_FPSCamera, offsetof(Camera, m_FPSCamera), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
};
void Camera::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeVec3(g_Properties[PIDX_Position], m_Position);
	serializer.SerializeVec3(g_Properties[PIDX_FrontVector], m_FrontVector);
	serializer.SerializeVec3(g_Properties[PIDX_RightVector], m_RightVector);
	serializer.SerializeVec3(g_Properties[PIDX_UpVector], m_UpVector);
	serializer.SerializeFloat(g_Properties[PIDX_NearPlane], m_NearPlane);
	serializer.SerializeFloat(g_Properties[PIDX_FarPlane], m_FarPlane);
	serializer.SerializeInt(g_Properties[PIDX_ProjectionType], m_ProjectionType);
	serializer.SerializeFloat(g_Properties[PIDX_OrthoBorders], m_OrthoBorders);
	serializer.SerializeFloat(g_Properties[PIDX_ViewSize], m_ViewSize);
	serializer.SerializeFloat(g_Properties[PIDX_MovementSpeed], m_MovementSpeed);
	serializer.SerializeFloat(g_Properties[PIDX_KeyMovementSpeedIncreaseFactor], m_KeyMovementSpeedIncreaseFactor);
	serializer.SerializeFloat(g_Properties[PIDX_FOVY], m_FOVY);
}
bool Camera::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool Camera::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Position], &m_Position , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_FrontVector], &m_FrontVector , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_RightVector], &m_RightVector , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UpVector], &m_UpVector , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_NearPlane], &m_NearPlane , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_FarPlane], &m_FarPlane , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ProjectionType], &m_ProjectionType , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_OrthoBorders], &m_OrthoBorders , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ViewSize], &m_ViewSize , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MovementSpeed], &m_MovementSpeed , 0.f, 30.f );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_KeyMovementSpeedIncreaseFactor], &m_KeyMovementSpeedIncreaseFactor , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_FOVY], &m_FOVY , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_FPSCamera], &m_FPSCamera  );
	return true;
}
void Camera::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Camera* other = (Camera*) otherObj;
	other->SetPosition(m_Position);
	other->SetFrontVector(m_FrontVector);
	other->SetRightVector(m_RightVector);
	other->SetUpVector(m_UpVector);
	other->SetViewMatrix(m_ViewMatrix);
	other->SetViewMatrixNoTranslation(m_ViewMatrixNoTranslation);
	other->SetProjectionMatrix(m_ProjectionMatrix);
	other->SetViewProjectionMatrix(m_ViewProjectionMatrix);
	other->SetViewMatrixInverse(m_ViewMatrixInverse);
	other->SetProjectionMatrixInverse(m_ProjectionMatrixInverse);
	other->SetViewProjectionMatrixInverse(m_ViewProjectionMatrixInverse);
	other->SetNearPlane(m_NearPlane);
	other->SetFarPlane(m_FarPlane);
	other->SetProjectionType(m_ProjectionType);
	other->SetOrthoBorders(m_OrthoBorders);
	other->SetViewSize(m_ViewSize);
	other->SetMovementSpeed(m_MovementSpeed);
	other->SetKeyMovementSpeedIncreaseFactor(m_KeyMovementSpeedIncreaseFactor);
	other->SetFOVY(m_FOVY);
	other->SetFPSCamera(m_FPSCamera);
}
const char* Camera::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Camera
#define PI (3.14159265359)

using namespace glm;

Camera::Camera()
{
	m_OrthoBorders = 10.f;
    m_ViewProjectionMatrix = mat4(1);
    m_ViewSize = 1;
    angle = 0;
    aspectRatio = 1; // TODO: change to window's aspect ratio.
    m_NearPlane = 0.01f;
    m_FarPlane = 1000.f;

    cameraType = CameraType::CYLINDRICAL;
    controlType = ControlType::QWEASDZXC_DRAG;
    m_UpVector = normalize(vec3(0,0,1));
    m_FrontVector = normalize(vec3(0,-1,0));
    m_RightVector = vec3(1,0,0);
    cylindricalUpVec = vec3(0,0,1);
    upVecReference = m_UpVector;
    frontVecReference = m_FrontVector;
    rightVecReference = m_RightVector;
//    translateTo(vec3(45.47500,-73.65500,10000));
//    scaleTo(vec3(1,1,1));
//    lookAtTargetPos = vec3(45.47500,-73.65500,0);
    lookAtTargetPos = m_Position + m_FrontVector;
    defaultViewMovementSpeed = 0.1f;
    viewMovementSpeed = defaultViewMovementSpeed;
    defaultKeyMovementSpeed = m_MovementSpeed * 0.05f;

    keyMovementSpeed = defaultKeyMovementSpeed;
    
    mouseIsDragging = false;
}

void Camera::PostSerialization(bool writing, bool success)
{
	if (!writing)
	{
		UpdateVPMatrix();
	}
}

void Camera::translateBy(vec3 delta)
{
    m_Position += delta;
}

void Camera::translateTo(vec3 inPosition)
{
    //TODO: handle this better
	glm::vec2 coordinateShifting = glm::vec2(2, 0);


	m_Position = inPosition;

	m_Position.x += coordinateShifting.x;
	m_Position.y += coordinateShifting.y;
}

void Camera::rotate(float _angle){
    angle += _angle;
}

void Camera::moveFromUserInput(GLFWwindow *pWindow)
{
    static const float deltaFactor = 0.05f;

    if( glfwGetKey(pWindow, GLFW_KEY_S)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_DOWN)==GLFW_PRESS ) {
        translateBy(deltaFactor * m_ViewSize * vec3(0,-1,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_W)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_UP)==GLFW_PRESS ) {
        translateBy(deltaFactor * m_ViewSize * vec3(0,1,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_A)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_LEFT)==GLFW_PRESS ) {
        translateBy(deltaFactor * m_ViewSize * vec3(-1,0,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_D)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_RIGHT)==GLFW_PRESS ) {
        translateBy(deltaFactor * m_ViewSize * vec3(1,0,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_R)==GLFW_PRESS ) {
        rotate(0.01f);
    }
    if( glfwGetKey(pWindow, GLFW_KEY_T)==GLFW_PRESS ) {
        rotate(-0.01f);
    }
}

void Camera::UpdateVPMatrix()
{

    m_ViewProjectionMatrix = column(mat4(1), 0, vec4(m_RightVector, 0));
    m_ViewProjectionMatrix = column(m_ViewProjectionMatrix, 1, vec4(m_UpVector, 0));
    m_ViewProjectionMatrix = column(m_ViewProjectionMatrix, 2, vec4(m_FrontVector, 0));
    m_ViewProjectionMatrix = column(m_ViewProjectionMatrix, 3, vec4(0, 0, 0, 1));
    m_ViewProjectionMatrix = translate(m_Position) * m_ViewProjectionMatrix;
    m_ViewProjectionMatrix = inverse(m_ViewProjectionMatrix);

	m_ViewMatrix = m_ViewProjectionMatrix;


	// todo : with 2 types it might not worth it to subclass
	// but also it might do
	if (m_ProjectionType == ProjectionType_Perspective)
	{
		m_ViewProjectionMatrix = perspective(m_FOVY, aspectRatio, GetNearPlane(), m_FarPlane) * m_ViewProjectionMatrix;
		m_ProjectionMatrix = perspective(m_FOVY, aspectRatio, GetNearPlane(), m_FarPlane);
	}
	else
	{
		float borders = m_OrthoBorders;
		m_ViewProjectionMatrix = ortho(-borders, borders, -borders, borders, GetNearPlane(), m_FarPlane) * m_ViewProjectionMatrix;
		m_ProjectionMatrix = ortho(-borders, borders, -borders, borders, GetNearPlane(), m_FarPlane);
	}

	m_ViewMatrixInverse = glm::inverse(m_ViewMatrix);
	m_ProjectionMatrixInverse = glm::inverse(m_ProjectionMatrix);
	m_ViewProjectionMatrixInverse = glm::inverse(m_ViewProjectionMatrix);

	m_ViewMatrixNoTranslation = m_ViewMatrix;
	m_ViewMatrixNoTranslation[3] = glm::vec4(0, 0, 0, 1);

	lxAssert(! (glm::isnan(m_ViewProjectionMatrix[0][0])));
}








void Camera::handlePresetMouseButton(
        GLFWwindow *pWindow, int button, int action, int mods)
{
//    if(mbUsePresetMouseButtonBehavior) {
        switch(controlType) {
        case ControlType::QWEASDZXC_DRAG:
            dragMousePresetButton(pWindow, button, action, mods);
        default:
            break;
        }
//    }
}


void Camera::dragMousePresetButton(GLFWwindow* pWindow, int button, int action, int mods)
{
    (void)mods;
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(action == GLFW_PRESS) {
            positionReference = m_Position;
            mouseIsDragging = true;
            double xx, yy;
            glfwGetCursorPos(pWindow, &xx, &yy);
            mouseDragReferencePosition = vec2(xx, yy);
            distanceReference = distance(m_Position, lookAtTargetPos);
            frontVecReference = m_FrontVector;
            upVecReference = m_UpVector;
            rightVecReference = m_RightVector;
//            mbRotationMatrixIsDirty = true;
        } else if(action == GLFW_RELEASE) {
           mouseIsDragging = false;
            if(cameraType == CameraType::TOP_3D) {
				m_FrontVector = vec3(0,0,1);
				m_UpVector = vec3(0,1,0);
				m_RightVector = vec3(1,0,0);
            } else {
				m_FrontVector = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
                if(cameraType == CameraType::CYLINDRICAL) {
					m_RightVector = normalize(cross(cylindricalUpVec, m_FrontVector));
					m_UpVector = cross(m_FrontVector, m_RightVector);
                } else {
					m_UpVector = vec3(tempExtraRotationMat * vec4(upVecReference,1));
					m_RightVector = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
                }
                if(cameraType == CameraType::AROUND_TARGET) {
					m_Position = lookAtTargetPos + distanceReference*m_FrontVector;
                }
//                mbTranslationMatrixIsDirty = true;
//                mbRotationMatrixIsDirty = true;
            }
        }
    }
}


void Camera::handlePresetCursorPos(GLFWwindow *pWindow, double x, double y)
{

    switch(controlType) 
	{
		case ControlType::QWEASDZXC_DRAG:
			dragMousePresetCursorPos(pWindow, x, y);
			break;
		case ControlType::QWEASDZXC_CONTINUOUS:
			//cursorPos = pWindow->getCursorPos();
			//continuousMousePresetCursorPos(pWindow, cursorPos.x, cursorPos.y);
			continuousMousePresetCursorPos(pWindow, x, y);
			qweasdzxcKeyHoldPreset(pWindow);
			break;
		default:
			break;
    }
}

void Camera::dragMousePresetCursorPos(GLFWwindow* /*pWindow*/, double x, double y)
{
    if(mouseIsDragging) {
        vec2 deltaPos = vec2(x, y) - mouseDragReferencePosition;
        if(cameraType == CameraType::AROUND_TARGET) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        upVecReference);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
			m_FrontVector = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
			m_UpVector = vec3(tempExtraRotationMat * vec4(upVecReference,1));
			m_RightVector = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
			m_Position = lookAtTargetPos + distanceReference*m_FrontVector;
//            mbRotationMatrixIsDirty = true;
//            mbTranslationMatrixIsDirty = true;
        } else if(cameraType == CameraType::CYLINDRICAL) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        cylindricalUpVec);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
			m_FrontVector = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
			m_RightVector = normalize(cross(cylindricalUpVec, m_FrontVector));
			m_UpVector = cross(m_FrontVector, m_RightVector);
//            mbRotationMatrixIsDirty = true;
        } else if(cameraType == CameraType::AIRPLANE) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        upVecReference);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
			m_FrontVector = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
			m_UpVector = vec3(tempExtraRotationMat * vec4(upVecReference,1));
			m_RightVector = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
//            mbRotationMatrixIsDirty = true;
        } else if(cameraType == CameraType::TOP_3D) {
			m_FrontVector = vec3(0,0,1);
			m_UpVector = vec3(0,1,0);
			m_RightVector = vec3(1,0,0);
			m_Position = positionReference - viewMovementSpeed*vec3(deltaPos.x, -deltaPos.y, 0);
        }
    }
}



void Camera::handlePresetKey(
        GLFWwindow *pWindow, int key, int scancode, int action, int mods)
{
//    if(mbUsePresetKeyBehavior) {
        switch(controlType) {
        case ControlType::QWEASDZXC_DRAG:
        case ControlType::QWEASDZXC_CONTINUOUS:
        case ControlType::QWEASDZXC_ARROWS:
            qweasdzxcKeyPreset(pWindow, key, scancode, action, mods);
            break;
        default:
            break;
        }
//    }
}

void Camera::qweasdzxcKeyPreset(
        GLFWwindow *pWindow, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    (void)pWindow;

	bool isPress = (action == GLFW_PRESS);

	if (action == GLFW_REPEAT)
	{
		return;
	}

    switch(key) 
	{
		case GLFW_KEY_LEFT_SHIFT:
			m_KeyMovementSpeedIncreaseFactor *= (isPress ? 2.f : 0.5f);
			break;
		case GLFW_KEY_X:
			m_KeyMovementSpeedIncreaseFactor = isPress ? 0.05f : m_KeyMovementSpeedIncreaseFactor;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			m_KeyMovementSpeedIncreaseFactor /= (isPress ? 2.f : 0.5f);
			break;
		case GLFW_KEY_KP_ADD:
			if (isPress)
			{
				m_MovementSpeed *= 5.f;
				g_RenderDataManager->AddTimedMessage(StringUtils::Format("'+' : 5x camera speed (%f)", m_MovementSpeed));
			}

			break; 
		case GLFW_KEY_KP_SUBTRACT:
			if (isPress)
			{
				m_MovementSpeed /= 5.f;
				g_RenderDataManager->AddTimedMessage(StringUtils::Format("'-' : 0.2x camera speed (%f)", m_MovementSpeed));
			}

			break;
    }

}

void Camera::NormalizeVectors()
{
	m_FrontVector = normalize(m_FrontVector);
	m_UpVector = normalize(m_UpVector);
	m_RightVector = normalize(m_RightVector);
}



void Camera::handlePresetNewFrame(GLFWwindow* pWindow)
{
	// TODO : if imgui hadnled setcallback this wouldn't be needed
	//NormalizeVectors();

	if (!mouseIsDragging)
	{
		return;
	}

    switch(controlType) 
	{
		case ControlType::QWEASDZXC_DRAG:
		case ControlType::QWEASDZXC_CONTINUOUS:
			qweasdzxcKeyHoldPreset(pWindow);
			break;
		case ControlType::QWEASDZXC_ARROWS:
			qweasdzxcKeyHoldPreset(pWindow);
			viewArrowsPreset(pWindow);
			break;
		default:
			break;
    }

}


void Camera::viewArrowsPreset(GLFWwindow* pWindow)
{
    vec2 deltaPos = vec2(0,0);
    if(glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) 
	{
        deltaPos = deltaPos - vec2(0,1);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
        deltaPos = deltaPos + vec2(0,1);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) 
	{
        deltaPos = deltaPos - vec2(1,0);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) 
	{
        deltaPos = deltaPos + vec2(1,0);
    }
    if(cameraType == CameraType::AROUND_TARGET) 
	{
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    m_UpVector);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, m_RightVector);
        m_FrontVector = vec3(tempExtraRotationMat * vec4(m_FrontVector,1));
		m_UpVector = vec3(tempExtraRotationMat * vec4(m_UpVector,1));
		m_RightVector = vec3(tempExtraRotationMat * vec4(m_RightVector,1));
		m_Position = lookAtTargetPos + (distance(lookAtTargetPos, m_Position))*m_FrontVector;
//        mbRotationMatrixIsDirty = true;
//        mbTranslationMatrixIsDirty = true;
    } 
	else if(cameraType == CameraType::CYLINDRICAL) 
	{
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    cylindricalUpVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, m_RightVector);
        m_FrontVector = vec3(tempExtraRotationMat * vec4(m_FrontVector,1));
		m_RightVector = normalize(cross(cylindricalUpVec, m_FrontVector));
		m_UpVector = cross(m_FrontVector, m_RightVector);
//        mbRotationMatrixIsDirty = true;
    } 
	else if(cameraType == CameraType::AIRPLANE) 
	{
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    m_UpVector);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, m_RightVector);
        m_FrontVector = vec3(tempExtraRotationMat * vec4(m_FrontVector,1));
		m_UpVector = vec3(tempExtraRotationMat * vec4(m_UpVector, 1));
		m_RightVector = vec3(tempExtraRotationMat * vec4(m_RightVector, 1));
//        mbRotationMatrixIsDirty = true;
    } 
	else if(cameraType == CameraType::TOP_3D) 
	{
        m_FrontVector = vec3(0,0,1);
		m_UpVector = vec3(0,1,0);
		m_RightVector = vec3(1,0,0);
		m_Position += viewMovementSpeed*vec3(deltaPos.x, -deltaPos.y, 0);
    }
}


void Camera::continuousMousePresetCursorPos(
        GLFWwindow *pWindow, double x, double y)
{
    //vec2 deltaPos = vec2(x, y) - vec2(pWindow->getCenter());
    int windowWidth, windowHeight;
    glfwGetWindowSize(pWindow, &windowWidth, &windowHeight);
    vec2 deltaPos = vec2(x, y) - 0.5f*vec2(float(windowWidth), float(windowHeight));
    if(cameraType == CameraType::AROUND_TARGET) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
			m_UpVector);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, m_RightVector);
        m_FrontVector = vec3(tempExtraRotationMat * vec4(m_FrontVector,1));
		m_UpVector = vec3(tempExtraRotationMat * vec4(m_UpVector,1));
		m_RightVector = vec3(tempExtraRotationMat * vec4(m_RightVector,1));
		m_Position = lookAtTargetPos + (distance(lookAtTargetPos, m_Position))*m_FrontVector;
//        mbRotationMatrixIsDirty = true;
//        mbTranslationMatrixIsDirty = true;
    } else if(cameraType == CameraType::CYLINDRICAL) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    cylindricalUpVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, m_RightVector);
        m_FrontVector = vec3(tempExtraRotationMat * vec4(m_FrontVector,1));
		m_RightVector = normalize(cross(cylindricalUpVec, m_FrontVector));
		m_UpVector = cross(m_FrontVector, m_RightVector);
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::AIRPLANE) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    m_UpVector);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, m_RightVector);
        m_FrontVector = vec3(tempExtraRotationMat * vec4(m_FrontVector,1));
		m_UpVector = vec3(tempExtraRotationMat * vec4(m_UpVector,1));
		m_RightVector = vec3(tempExtraRotationMat * vec4(m_RightVector,1));
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::TOP_3D) {
        m_FrontVector = vec3(0,0,1);
		m_UpVector = vec3(0,1,0);
		m_RightVector = vec3(1,0,0);
		m_Position += viewMovementSpeed*vec3(deltaPos.x, -deltaPos.y, 0);
    }
//    vec2 centerPos = vec2(pWindow->getCenter());
    glfwSetCursorPos(pWindow, float(windowWidth)/2.f, float(windowHeight)/2.f);
}

void Camera::qweasdzxcKeyHoldPreset(GLFWwindow *pWindow)
{
	keyMovementSpeed = m_MovementSpeed * m_KeyMovementSpeedIncreaseFactor;
    if(cameraType == CameraType::AROUND_TARGET) 
	{
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) 
		{
			m_Position -= keyMovementSpeed * m_FrontVector;
            distanceReference = max(distanceReference - keyMovementSpeed, 0.0f);
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) 
		{
			m_Position += keyMovementSpeed * m_FrontVector;
            distanceReference = max(distanceReference + keyMovementSpeed, 0.0f);
        }
    } 
	else if(cameraType == CameraType::TOP_3D) 
	{
        m_FrontVector = vec3(0,0,1);
		m_UpVector = vec3(0,1,0);
		m_RightVector = vec3(1,0,0);
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) 
		{
			m_Position += keyMovementSpeed * m_UpVector;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) 
		{
			m_Position -= keyMovementSpeed * m_UpVector;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) 
		{
			m_Position -= keyMovementSpeed * m_RightVector;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) 
		{
			m_Position += keyMovementSpeed * m_RightVector;
        }
    } 
	else 
	{
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) 
		{
			m_Position -= keyMovementSpeed * m_FrontVector;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) 
		{
			m_Position += keyMovementSpeed * m_FrontVector;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) 
		{
			m_Position -= keyMovementSpeed * m_RightVector;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) 
		{
			m_Position += keyMovementSpeed * m_RightVector;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS) 
		{
            if(cameraType == CameraType::CYLINDRICAL) 
			{
				m_Position -= keyMovementSpeed * cylindricalUpVec;
            } 
			else 
			{
				m_Position -= keyMovementSpeed * m_UpVector;
            }
        }
        if(glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS) 
		{
            if(cameraType == CameraType::CYLINDRICAL) 
			{
				m_Position += keyMovementSpeed * cylindricalUpVec;
            } 
			else 
			{
				m_Position += keyMovementSpeed * m_UpVector;
            }
        }
    }
}

float Camera::GetNearPlane_Callback()
{
	return max(m_NearPlane, 0.01f);
}
