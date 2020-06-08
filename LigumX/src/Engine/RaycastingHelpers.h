#pragma once

#include "glm/glm.hpp"

namespace RaycastingHelpers
{
	glm::vec3 GetWorldSpaceRay(const glm::vec3& ndc, const glm::mat4& projectionMatrixInverse, const glm::mat4& viewMatrixInverse);

	glm::vec3 IntersectRayWithPlane(const glm::vec3& worldSpaceRay, const glm::vec3& cameraPosition, const glm::vec3& planeNormal, const glm::vec3& pointOnPlane);

	glm::vec3 IntersectRayWithFlatPlane(const glm::vec3& worldSpaceRay, const glm::vec3& cameraPosition);

	glm::vec3 GetAimingWorldSpacePosition(const glm::vec2& mousePosition);

	glm::vec3 GetAimingWorldSpacePosition();


};
