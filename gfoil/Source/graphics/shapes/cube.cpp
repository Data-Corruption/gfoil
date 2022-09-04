#include "cube.h"

#include <limits>

#include "../../system/system.h"
#include "../../config.h"
#include "../generic_index_buffers.h"

constexpr int face_indices[6][4] = {
	{ 2, 3, 7, 6 }, // up
	{ 0, 2, 6, 4 }, // front
	{ 4, 6, 7, 5 }, // left
	{ 5, 7, 3, 1 }, // back 
	{ 1, 3, 2, 0 }, // right
	{ 0, 4, 5, 1 }  // down
};

// ---- color ----
bool gfoil::cube::color::visible_by_camera(generic_3d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex.position - camera.position;
		if (math::vec3_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}
bool gfoil::cube::color::hit_by_ray(ray& ray, int* face_hit, float* distance) {
	*distance = std::numeric_limits<float>::max();
	float current_hit_distance = 0.0f;
	bool hit = false;

	// check each face for intersection
	for (int i = 0; i < 6; i++) {
		if (math::ray_rect_intersect(
			ray,
			this->vertices[face_indices[i][0]].position,
			this->vertices[face_indices[i][1]].position,
			this->vertices[face_indices[i][2]].position,
			this->vertices[face_indices[i][3]].position,
			&current_hit_distance))
		{
			if (current_hit_distance < *distance) {
				*distance = current_hit_distance;
				*face_hit = i;
				hit = true;
			}
		}
	}

	return hit;
}

// ---- texture ----
bool gfoil::cube::texture::visible_by_camera(generic_3d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex.position - camera.position;
		if (math::vec3_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}
bool gfoil::cube::texture::hit_by_ray(ray& ray, int* face_hit, float* distance) {
	*distance = std::numeric_limits<float>::max();
	float current_hit_distance = 0.0f;
	bool hit = false;

	// check each face for intersection
	for (int i = 0; i < 6; i++) {
		if (math::ray_rect_intersect(
			ray,
			this->vertices[face_indices[i][0]].position,
			this->vertices[face_indices[i][1]].position,
			this->vertices[face_indices[i][2]].position,
			this->vertices[face_indices[i][3]].position,
			&current_hit_distance))
		{
			if (current_hit_distance < *distance) {
				*distance = current_hit_distance;
				*face_hit = i;
				hit = true;
			}
		}
	}

	return hit;
}

// ---- tint ----
bool gfoil::cube::tint::visible_by_camera(generic_3d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex.position - camera.position;
		if (math::vec3_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}
bool gfoil::cube::tint::hit_by_ray(ray& ray, int* face_hit, float* distance) {
	*distance = std::numeric_limits<float>::max();
	float current_hit_distance = 0.0f;
	bool hit = false;

	// check each face for intersection
	for (int i = 0; i < 6; i++) {
		if (math::ray_rect_intersect(
			ray,
			this->vertices[face_indices[i][0]].position,
			this->vertices[face_indices[i][1]].position,
			this->vertices[face_indices[i][2]].position,
			this->vertices[face_indices[i][3]].position,
			&current_hit_distance))
		{
			if (current_hit_distance < *distance) {
				*distance = current_hit_distance;
				*face_hit = i;
				hit = true;
			}
		}
	}

	return hit;
}

// ---- batch renderer ----
void gfoil::cube::batch_renderer::generate(vertex::type target_vertex_type) {
	this->vertex_type = target_vertex_type;
	this->max_cubes_per_batch = 1800 * 8;
	this->renderer.generate(this->max_cubes_per_batch, primative_type::TRIANGLES, target_vertex_type, generic_index_buffers::cube.id, 8, 36);
}
void gfoil::cube::batch_renderer::generate(unsigned int max_cubes_per_batch, vertex::type target_vertex_type, unsigned int index_buffer_id) {
	this->vertex_type = target_vertex_type;

	this->max_cubes_per_batch = max_cubes_per_batch * 8;

	this->renderer.generate(this->max_cubes_per_batch, primative_type::TRIANGLES, target_vertex_type, index_buffer_id, 8, 36);
}

void gfoil::cube::batch_renderer::buffer_data(std::vector<cube::color>& cubes) {
	if (this->vertex_type != vertex::type::COLOR) { system::log::error("Attempting to buffer incorrect type to cube batch renderer"); };
	this->renderer.buffer_data(&cubes[0].vertices[0], (unsigned int)cubes.size() * 8);
}
void gfoil::cube::batch_renderer::buffer_data(std::vector<cube::texture>& cubes) {
	if (this->vertex_type != vertex::type::COLOR) { system::log::error("Attempting to buffer incorrect type to cube batch renderer"); };
	this->renderer.buffer_data(&cubes[0].vertices[0], (unsigned int)cubes.size() * 8);
}
void gfoil::cube::batch_renderer::buffer_data(std::vector<cube::tint>& cubes) {
	if (this->vertex_type != vertex::type::COLOR) { system::log::error("Attempting to buffer incorrect type to cube batch renderer"); };
	this->renderer.buffer_data(&cubes[0].vertices[0], (unsigned int)cubes.size() * 8);
}