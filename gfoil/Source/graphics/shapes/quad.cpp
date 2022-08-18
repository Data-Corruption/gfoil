#include "quad.h"

// ---- plane stuff ----
bool gfoil::quad::color::visible_by_camera(generic_2d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex - camera.position;
		if (vertex_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}
bool gfoil::quad::color::visible_by_camera(generic_3d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex.position - camera.position;
		if (math::vec3_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}

bool gfoil::quad::vertex_in_front_of_plane(plane& plane, glm::vec3& vertex) {
	// if half space >= 0.0
	if (((plane.normal.x * vertex.x) + (plane.normal.y * vertex.y) + (plane.normal.z * vertex.z) + plane.distance) >= 0.0)
		return true;
	return false;
}

// ---- ray stuff ----
bool gfoil::quad::hit_by_ray(ray& ray, int* side_hit, float* distance) {
	glm::dvec3 intersection;
	bool hit = false;

	// cheack each face for intersection
	for (int i = 0; i < 6; i++) {
		if (gfoil::math::ray_triangle_intersect(
			ray,
			vertices[face_indices[i][0]],
			vertices[face_indices[i][1]],
			vertices[face_indices[i][2]],
			&intersection))
		{
			*distance = (float)intersection.x;
			*side_hit = i;
			hit = true;
			continue;
		}
		if (gfoil::math::ray_triangle_intersect(
			ray,
			vertices[face_indices[i][2]],
			vertices[face_indices[i][3]],
			vertices[face_indices[i][0]],
			&intersection))
		{
			*distance = (float)intersection.x;
			*side_hit = i;
			hit = true;
			continue;
		}
	}
	return hit;
}