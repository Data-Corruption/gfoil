#include "gmath.h"

glm::mat4 gfoil::math::ortho(
	const float& left,
	const float& right,
	const float& bottom,
	const float& top,
	const float& zNear,
	const float& zFar) 
{
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = float(2) / (right - left);
	result[1][1] = float(2) / (top - bottom);
	result[2][2] = -float(2) / (zFar - zNear);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[3][2] = -(zFar + zNear) / (zFar - zNear);
	return result;
}

bool gfoil::math::ray_triangle_intersect(ray& ray, glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::dvec3* result) {
	constexpr auto EPSILON = 0.000001;

	glm::vec3 edge_1, edge_2, t_vec3, p_vec3, q_vec3;
	double determinant, inv_determinant;

	// calculate edges
	edge_1 = v1 - v0;
	edge_2 = v2 - v0;

	// start determinate, also used to get u
	p_vec3 = glm::cross(ray.direction, edge_2);
	determinant = glm::dot(edge_1, p_vec3);

	// if ray does not pass through triangle plane then return 0
	if (determinant < EPSILON)
		return false;

	t_vec3 = ray.position - v0;

	result->y = glm::dot(t_vec3, p_vec3);
	if (result->y < 0.0 || result->y > determinant)
		return false;

	// calculate v and test bounds
	q_vec3 = glm::cross(t_vec3, edge_1);
	result->z = glm::dot(ray.direction, q_vec3);
	if (result->z < 0.0 || result->y + result->z > determinant)
		return false;

	// ray intersects triangle, calculate t aka x
	result->x = glm::dot(edge_2, q_vec3);
	inv_determinant = 1.0 / determinant;
	result->x *= inv_determinant;
	result->y *= inv_determinant;
	result->z *= inv_determinant;

	if (result->x < 0.0)
		return false;
	return true;
}