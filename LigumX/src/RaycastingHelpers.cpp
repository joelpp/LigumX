#include "RaycastingHelpers.h"

#include "LigumX.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "InputHandler.h"


glm::vec3 RaycastingHelpers::GetWorldSpaceRay(const glm::vec3& ndc, const glm::mat4& projectionMatrixInverse, const glm::mat4& viewMatrixInverse)
{
	glm::vec4 clipCoords = glm::vec4(glm::vec2(ndc), -1.0f, 1.0f);
	clipCoords.y = -clipCoords.y;

	glm::vec4 cameraRay = projectionMatrixInverse * clipCoords;

	cameraRay.z = -1;
	cameraRay.w = 0;

	glm::vec4 worldSpaceRay = viewMatrixInverse * cameraRay;

	return glm::normalize(glm::vec3(worldSpaceRay));
}

glm::vec3 RaycastingHelpers::IntersectRayWithPlane(const glm::vec3& worldSpaceRay, const glm::vec3& cameraPosition, const glm::vec3& planeNormal, const glm::vec3& pointOnPlane)
{
	float t = glm::dot(pointOnPlane - cameraPosition, planeNormal) / glm::dot(glm::vec3(worldSpaceRay), planeNormal);

	glm::vec3 worldPosition = cameraPosition + t * glm::vec3(worldSpaceRay);

	return worldPosition;
}

glm::vec3 RaycastingHelpers::IntersectRayWithFlatPlane(const glm::vec3& worldSpaceRay, const glm::vec3& cameraPosition)
{
	glm::vec3 planeNormal = glm::vec3(0, 0, 1);
	glm::vec3 pointOnPlane = glm::vec3(0, 0, 0);

	return IntersectRayWithPlane(worldSpaceRay, cameraPosition, planeNormal, pointOnPlane);
}

glm::vec3 RaycastingHelpers::GetAimingWorldSpacePosition(const glm::vec2& mousePosition)
{
	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	glm::ivec2 screenSize = renderer->m_Window->GetSize();

	glm::vec2 normalizedScreenPosition = glm::vec2(mousePosition / glm::vec2(screenSize));

	glm::vec3 ndc = glm::vec3(2.f * normalizedScreenPosition - glm::vec2(1.f, 1.f), 1.f);

	glm::vec3 wsRay = GetWorldSpaceRay(ndc, renderer->GetActiveCamera()->GetProjectionMatrixInverse(), renderer->GetActiveCamera()->GetViewMatrixInverse());

	glm::vec3 worldPosition = IntersectRayWithFlatPlane(wsRay, renderer->GetActiveCamera()->GetPosition());

	return worldPosition;
}

glm::vec3 RaycastingHelpers::GetAimingWorldSpacePosition()
{
	const glm::vec2& mousePosition = g_InputHandler->GetMousePosition();

	return GetAimingWorldSpacePosition(mousePosition);
}
