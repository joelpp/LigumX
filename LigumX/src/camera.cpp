#include "stdafx.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Settings.h"

#pragma region  CLASS_SOURCE Camera
#include "Camera.h"
#include <cstddef>
const ClassPropertyData Camera::g_CameraProperties[] = 
{
{ "Position", offsetof(Camera, m_Position), 0, LXType_glmvec3, false,  }, 
{ "someBullshitVec3", offsetof(Camera, m_someBullshitVec3), 0, LXType_glmvec3, false,  }, 
{ "ViewMatrix", offsetof(Camera, m_ViewMatrix), 0, LXType_glmmat4, false,  }, 
{ "ProjectionMatrix", offsetof(Camera, m_ProjectionMatrix), 0, LXType_glmmat4, false,  }, 
{ "ViewProjectionMatrix", offsetof(Camera, m_ViewProjectionMatrix), 0, LXType_glmmat4, false,  }, 
{ "NearPlane", offsetof(Camera, m_NearPlane), 0, LXType_float, false,  }, 
{ "FarPlane", offsetof(Camera, m_FarPlane), 0, LXType_float, false,  }, 
{ "ProjectionType", offsetof(Camera, m_ProjectionType), 0, LXType_ProjectionType, false,  }, 
{ "OrthoBorders", offsetof(Camera, m_OrthoBorders), 0, LXType_float, false,  }, 
};

#pragma endregion  CLASS_SOURCE Camera
#define PI (3.14159265359)

using namespace glm;

Camera::Camera()
{
	m_OrthoBorders = 10.f;
    m_ViewProjectionMatrix = mat4(1);
    viewSize = 1;
    angle = 0;
    totalViewAngleY = 45;
    aspectRatio = 1; // TODO: change to window's aspect ratio.
    m_NearPlane = 0.001f;
    m_FarPlane = 1000.f;

    cameraType = CameraType::CYLINDRICAL;
    controlType = ControlType::QWEASDZXC_DRAG;
    upVec = normalize(vec3(0,0,1));
    frontVec = normalize(vec3(0,-1,0));
    rightVec = vec3(1,0,0);
    cylindricalUpVec = vec3(0,0,1);
    upVecReference = upVec;
    frontVecReference = frontVec;
    rightVecReference = rightVec;
//    translateTo(vec3(45.47500,-73.65500,10000));
//    scaleTo(vec3(1,1,1));
//    lookAtTargetPos = vec3(45.47500,-73.65500,0);
    lookAtTargetPos = m_Position + frontVec;
    defaultViewMovementSpeed = 0.1f;
    viewMovementSpeed = defaultViewMovementSpeed;
    defaultKeyMovementSpeed = 0.05f;
	minimumSpeed = 0.01f;
	maximumSpeed = 0.15f;

    keyMovementSpeed = defaultKeyMovementSpeed;
    keyMovementSpeedIncreaseFactor = 1.5f;
    mouseIsDragging = false;
    updateVPMatrix();
}


void Camera::translateBy(vec3 delta)
{
    m_Position += delta;
    updateVPMatrix();
}

void Camera::translateTo(vec3 inPosition)
{
    //TODO: handle this better
	glm::vec2 coordinateShifting = glm::vec2(2, 0);


	m_Position = inPosition;

	m_Position.x += coordinateShifting.x;
	m_Position.y += coordinateShifting.y;

    updateVPMatrix();
}

void Camera::setViewSize(float inViewSize)
{
    viewSize = inViewSize;
    updateVPMatrix();
}

void Camera::multViewSizeBy(float factor)
{
    viewSize *= factor;
    updateVPMatrix();
}

void Camera::rotate(float _angle){
    angle += _angle;
    updateVPMatrix();
}

void Camera::moveFromUserInput(GLFWwindow *pWindow)
{
    static const float deltaFactor = 0.05f;

    if( glfwGetKey(pWindow, GLFW_KEY_S)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_DOWN)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(0,-1,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_W)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_UP)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(0,1,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_A)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_LEFT)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(-1,0,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_D)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_RIGHT)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(1,0,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_R)==GLFW_PRESS ) {
        rotate(0.01);
    }
    if( glfwGetKey(pWindow, GLFW_KEY_T)==GLFW_PRESS ) {
        rotate(-0.01);
    }
}

void Camera::updateVPMatrix()
{

    m_ViewProjectionMatrix = column(mat4(1), 0, vec4(rightVec, 0));
    m_ViewProjectionMatrix = column(m_ViewProjectionMatrix, 1, vec4(upVec, 0));
    m_ViewProjectionMatrix = column(m_ViewProjectionMatrix, 2, vec4(frontVec, 0));
    m_ViewProjectionMatrix = column(m_ViewProjectionMatrix, 3, vec4(0, 0, 0, 1));
    m_ViewProjectionMatrix = translate(m_Position) * m_ViewProjectionMatrix;
    m_ViewProjectionMatrix = inverse(m_ViewProjectionMatrix);

	m_ViewMatrix = m_ViewProjectionMatrix;


	// todo : with 2 types it might not worth it to subclass
	// but also it might do
	if (m_ProjectionType == ProjectionType_Perspective)
	{
		m_ViewProjectionMatrix = perspective(totalViewAngleY, aspectRatio, m_NearPlane, m_FarPlane) * m_ViewProjectionMatrix;
		m_ProjectionMatrix = perspective(totalViewAngleY, aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		float borders = 20.f;
		m_ViewProjectionMatrix = ortho(-borders, borders, -borders, borders, m_NearPlane, m_FarPlane) * m_ViewProjectionMatrix;
		m_ProjectionMatrix = ortho(-borders, borders, -borders, borders, m_NearPlane, m_FarPlane);
	}
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
            frontVecReference = frontVec;
            upVecReference = upVec;
            rightVecReference = rightVec;
//            mbRotationMatrixIsDirty = true;
            updateVPMatrix();
        } else if(action == GLFW_RELEASE) {
           mouseIsDragging = false;
            if(cameraType == CameraType::TOP_3D) {
                frontVec = vec3(0,0,1);
                upVec = vec3(0,1,0);
                rightVec = vec3(1,0,0);
            } else {
                frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
                if(cameraType == CameraType::CYLINDRICAL) {
                    rightVec = normalize(cross(cylindricalUpVec, frontVec));
                    upVec = cross(frontVec, rightVec);
                } else {
                    upVec = vec3(tempExtraRotationMat * vec4(upVecReference,1));
                    rightVec = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
                }
                if(cameraType == CameraType::AROUND_TARGET) {
					m_Position = lookAtTargetPos + distanceReference*frontVec;
                }
//                mbTranslationMatrixIsDirty = true;
//                mbRotationMatrixIsDirty = true;
            }
            updateVPMatrix();
        }
    }
}


void Camera::handlePresetCursorPos(GLFWwindow *pWindow, double x, double y)
{
//    if(mbUsePresetCursorPosBehavior) {
        //vec2 cursorPos;
        switch(controlType) {
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
//    }
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
            frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
            upVec = vec3(tempExtraRotationMat * vec4(upVecReference,1));
            rightVec = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
			m_Position = lookAtTargetPos + distanceReference*frontVec;
//            mbRotationMatrixIsDirty = true;
//            mbTranslationMatrixIsDirty = true;
        } else if(cameraType == CameraType::CYLINDRICAL) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        cylindricalUpVec);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
            frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
            rightVec = normalize(cross(cylindricalUpVec, frontVec));
            upVec = cross(frontVec, rightVec);
//            mbRotationMatrixIsDirty = true;
        } else if(cameraType == CameraType::AIRPLANE) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        upVecReference);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
            frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
            upVec = vec3(tempExtraRotationMat * vec4(upVecReference,1));
            rightVec = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
//            mbRotationMatrixIsDirty = true;
        } else if(cameraType == CameraType::TOP_3D) {
            frontVec = vec3(0,0,1);
            upVec = vec3(0,1,0);
            rightVec = vec3(1,0,0);
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

    if (action == GLFW_PRESS) 
	{
        switch(key) 
		{
        case GLFW_KEY_LEFT_SHIFT:
            keyMovementSpeed /= keyMovementSpeedIncreaseFactor;
            break;
        case GLFW_KEY_X:
            keyMovementSpeed = defaultKeyMovementSpeed;
            break;
        case GLFW_KEY_LEFT_CONTROL:
            keyMovementSpeed *= keyMovementSpeedIncreaseFactor;
            break;
        }
    }

	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT_SHIFT:
			keyMovementSpeed *= keyMovementSpeedIncreaseFactor;
			break;

		case GLFW_KEY_LEFT_CONTROL:
			keyMovementSpeed /= keyMovementSpeedIncreaseFactor;
			break;
		}
	}
}


void Camera::handlePresetNewFrame(GLFWwindow* pWindow)
{
//    if(mbUsePresetNewFrameBehavior) {
        //vec2 cursorPos;
        switch(controlType) {
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
//    }
}


void Camera::viewArrowsPreset(GLFWwindow* pWindow)
{
    vec2 deltaPos = vec2(0,0);
    if(glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) {
        deltaPos = deltaPos - vec2(0,1);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
        deltaPos = deltaPos + vec2(0,1);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
        deltaPos = deltaPos - vec2(1,0);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        deltaPos = deltaPos + vec2(1,0);
    }
    if(cameraType == CameraType::AROUND_TARGET) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
		m_Position = lookAtTargetPos + (distance(lookAtTargetPos, m_Position))*frontVec;
//        mbRotationMatrixIsDirty = true;
//        mbTranslationMatrixIsDirty = true;
    } else if(cameraType == CameraType::CYLINDRICAL) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    cylindricalUpVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        rightVec = normalize(cross(cylindricalUpVec, frontVec));
        upVec = cross(frontVec, rightVec);
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::AIRPLANE) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::TOP_3D) {
        frontVec = vec3(0,0,1);
        upVec = vec3(0,1,0);
        rightVec = vec3(1,0,0);
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
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
		m_Position = lookAtTargetPos + (distance(lookAtTargetPos, m_Position))*frontVec;
//        mbRotationMatrixIsDirty = true;
//        mbTranslationMatrixIsDirty = true;
    } else if(cameraType == CameraType::CYLINDRICAL) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    cylindricalUpVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        rightVec = normalize(cross(cylindricalUpVec, frontVec));
        upVec = cross(frontVec, rightVec);
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::AIRPLANE) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::TOP_3D) {
        frontVec = vec3(0,0,1);
        upVec = vec3(0,1,0);
        rightVec = vec3(1,0,0);
		m_Position += viewMovementSpeed*vec3(deltaPos.x, -deltaPos.y, 0);
    }
//    vec2 centerPos = vec2(pWindow->getCenter());
    glfwSetCursorPos(pWindow, float(windowWidth)/2.f, float(windowHeight)/2.f);
}

void Camera::qweasdzxcKeyHoldPreset(GLFWwindow *pWindow)
{
    if(cameraType == CameraType::AROUND_TARGET) {
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
			m_Position -= keyMovementSpeed * frontVec;
            distanceReference = max(distanceReference - keyMovementSpeed, 0.0f);
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
			m_Position += keyMovementSpeed * frontVec;
            distanceReference = max(distanceReference + keyMovementSpeed, 0.0f);
        }
    } else if(cameraType == CameraType::TOP_3D) {
        frontVec = vec3(0,0,1);
        upVec = vec3(0,1,0);
        rightVec = vec3(1,0,0);
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
			m_Position += keyMovementSpeed * upVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
			m_Position -= keyMovementSpeed * upVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
			m_Position -= keyMovementSpeed * rightVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
			m_Position += keyMovementSpeed * rightVec;
        }
    } else {
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
			m_Position -= keyMovementSpeed * frontVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
			m_Position += keyMovementSpeed * frontVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
			m_Position -= keyMovementSpeed * rightVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
			m_Position += keyMovementSpeed * rightVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS) {
            if(cameraType == CameraType::CYLINDRICAL) {
				m_Position -= keyMovementSpeed * cylindricalUpVec;
            } else {
				m_Position -= keyMovementSpeed * upVec;
            }
        }
        if(glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS) {
            if(cameraType == CameraType::CYLINDRICAL) {
				m_Position += keyMovementSpeed * cylindricalUpVec;
            } else {
				m_Position += keyMovementSpeed * upVec;
            }
        }
    }
    updateVPMatrix();
}