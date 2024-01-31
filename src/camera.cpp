#include "Camera.h"
#include "Inputhandler.h"
#include "vec\vec.h"
#include "vec\mat.h"
#include <algorithm>

using namespace linalg;

void Camera::MoveTo(const vec3f& position) noexcept
{
	m_position = position;
}

void Camera::Move(const vec3f& direction) noexcept
{
	m_position += direction;
}

void Camera::RotateWithMouse(const InputHandler& inputHandler, float sensitivity, float deltaTime) noexcept
{
	float deltaX = static_cast<float>(inputHandler.GetMouseDeltaX()) * sensitivity * deltaTime;
	float deltaY = static_cast<float>(inputHandler.GetMouseDeltaY()) * sensitivity * deltaTime;

    m_yaw += deltaX;
    m_pitch += deltaY;
}

mat4f Camera::WorldToViewMatrix() const noexcept
{
	// Assuming a camera's position and rotation is defined by matrices T(p) and R,
	// the View-to-World transform is T(p)*R (for a first-person style camera).
	//
	// World-to-View then is the inverse of T(p)*R;
	//		inverse(T(p)*R) = inverse(R)*inverse(T(p)) = transpose(R)*T(-p)
	// Since now there is no rotation, this matrix is simply T(-p)

	//return mat4f::translation(-m_position);

	// Combine translation and rotation to get the View-to-World matrix
	mat4f T = mat4f::translation(-m_position);
	mat4f R = mat4f::rotation(0.0f, m_yaw, m_pitch);

	// View-to-World transform is inverse of T(p)*R
	mat4f viewToWorldMatrix = T * R;

	mat4f worldToViewMatrix = transpose(R) * T;

	return worldToViewMatrix;
}

mat4f Camera::ProjectionMatrix() const noexcept
{
	return mat4f::projection(m_vertical_fov, m_aspect_ratio, m_near_plane, m_far_plane);
}