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
	vec3f forward = normalize(vec3f{ sin(m_yaw), -sin(m_pitch), -cos(m_yaw) * cos(m_pitch) });
	vec3f right = normalize(vec3f{ -cos(m_yaw), 0.0f, -sin(m_yaw) });

	// Calculate the up direction manually
	vec3f up;
	up.x = forward.y * right.z - forward.z * right.y;
	up.y = forward.z * right.x - forward.x * right.z;
	up.z = forward.x * right.y - forward.y * right.x;
	up = normalize(up);

	// Calculate left, back, and down vectors
	vec3f left = -right;
	vec3f back = -forward;
	vec3f down = -up;

	// Update position components individually
	m_position.x += direction.x * right.x + direction.y * up.x + direction.z * forward.x;
	m_position.y += direction.x * right.y + direction.y * up.y + direction.z * forward.y;
	m_position.z += direction.x * right.z + direction.y * up.z + direction.z * forward.z;
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

	mat4f T = mat4f::translation(-m_position);
	mat4f R = mat4f::rotation(0.0f, m_yaw, m_pitch);

	mat4f viewToWorldMatrix = T * R;

	mat4f worldToViewMatrix = transpose(R) * T;

	return worldToViewMatrix;
}

mat4f Camera::ProjectionMatrix() const noexcept
{
	return mat4f::projection(m_vertical_fov, m_aspect_ratio, m_near_plane, m_far_plane);
}