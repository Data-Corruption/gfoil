#include "cube.h"

constexpr int face_indices[6][4] = {
	{ 2, 3, 7, 6 }, // up
	{ 0, 2, 6, 4 }, // front
	{ 4, 6, 7, 5 }, // left
	{ 5, 7, 3, 1 }, // back 
	{ 1, 3, 2, 0 }, // right
	{ 0, 4, 5, 1 }  // down
};

void gfoil::cube::generate(glm::ivec3 position, glm::ivec3 size) {
	vertices[0] = glm::vec3((float)(position.x + 0)     , (float)(position.y + 0)     , (float)(position.z + 0)     );
	vertices[1] = glm::vec3((float)(position.x + 0)     , (float)(position.y + 0)     , (float)(position.z + size.z));
	vertices[2] = glm::vec3((float)(position.x + 0)     , (float)(position.y + size.y), (float)(position.z + 0)     );
	vertices[3] = glm::vec3((float)(position.x + 0)     , (float)(position.y + size.y), (float)(position.z + size.z));
	vertices[4] = glm::vec3((float)(position.x + size.x), (float)(position.y + 0)     , (float)(position.z + 0)     );
	vertices[5] = glm::vec3((float)(position.x + size.x), (float)(position.y + 0)     , (float)(position.z + size.z));
	vertices[6] = glm::vec3((float)(position.x + size.x), (float)(position.y + size.y), (float)(position.z + 0)     );
	vertices[7] = glm::vec3((float)(position.x + size.x), (float)(position.y + size.y), (float)(position.z + size.z));
}

// ---- plane stuff ----
bool gfoil::cube::in_front_of_camera(generic_3d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex - camera.position;
		if (vertex_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}

bool gfoil::cube::vertex_in_front_of_plane(plane& plane, glm::vec3& vertex) {
	// if half space >= 0.0
	if (((plane.normal.x * vertex.x) + (plane.normal.y * vertex.y) + (plane.normal.z * vertex.z) + plane.distance) >= 0.0)
		return true;
	return false;
}

// ---- ray stuff ----
bool gfoil::cube::hit_by_ray(ray& ray, int* face_hit, float* distance) {
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
			*face_hit = i;
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
			*face_hit = i;
			hit = true;
			continue;
		}
	}
	return hit;
}