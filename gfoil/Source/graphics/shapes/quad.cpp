#include "quad.h"

#include "../../system/system.h"

// ---- color quad ----
bool gfoil::quad::color::visible_by_camera(generic_2d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec2 corrected_point = glm::vec2(vertex.position.x, vertex.position.y) - camera.position;
		if (math::vec2_in_rect(corrected_point, camera.mask[0], camera.mask[2]))
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
bool gfoil::quad::color::hit_by_ray(ray& ray, float* distance) {
	return math::ray_rect_intersect(
		ray, 
		vertices[0].position, 
		vertices[1].position, 
		vertices[2].position, 
		vertices[3].position, 
		distance
	);
}

// ---- texture quad ----
bool gfoil::quad::texture::visible_by_camera(generic_2d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec2 corrected_point = glm::vec2(vertex.position.x, vertex.position.y) - camera.position;
		if (math::vec2_in_rect(corrected_point, camera.mask[0], camera.mask[2]))
			return true;
	}
	return false;
}
bool gfoil::quad::texture::visible_by_camera(generic_3d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex.position - camera.position;
		if (math::vec3_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}
bool gfoil::quad::texture::hit_by_ray(ray& ray, float* distance) {
	return math::ray_rect_intersect(
		ray,
		vertices[0].position,
		vertices[1].position,
		vertices[2].position,
		vertices[3].position,
		distance
	);
}

// ---- tint quad ----
bool gfoil::quad::tint::visible_by_camera(generic_2d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec2 corrected_point = glm::vec2(vertex.position.x, vertex.position.y) - camera.position;
		if (math::vec2_in_rect(corrected_point, camera.mask[0], camera.mask[2]))
			return true;
	}
	return false;
}
bool gfoil::quad::tint::visible_by_camera(generic_3d_camera& camera) {
	for (auto& vertex : this->vertices) {
		glm::vec3 corrected_point = vertex.position - camera.position;
		if (math::vec3_in_front_of_plane(camera.forward_plane, corrected_point))
			return true;
	}
	return false;
}
bool gfoil::quad::tint::hit_by_ray(ray& ray, float* distance) {
	return math::ray_rect_intersect(
		ray,
		vertices[0].position,
		vertices[1].position,
		vertices[2].position,
		vertices[3].position,
		distance
	);
}

// ---- batch renderer ----
void gfoil::quad::batch_renderer::generate(unsigned int count, vertex::type target_vertex_type, unsigned int index_buffer_id) {
	this->vertex_type = target_vertex_type;
	this->renderer.generate(count * 4, primative_type::TRIANGLES, target_vertex_type, index_buffer_id, 4, 6);
}
void gfoil::quad::batch_renderer::destroy() {
	this->renderer.destroy();
}

void gfoil::quad::batch_renderer::flush() {
	this->renderer.flush();
}
void gfoil::quad::batch_renderer::draw() {
	this->renderer.draw();
}

void gfoil::quad::batch_renderer::buffer_data(std::vector<quad::color>& quads) {
	if (this->vertex_type != vertex::type::COLOR) { system::log::error("Attempting to buffer incorrect type to quad batch renderer"); };
	this->renderer.buffer_data(&quads[0].vertices[0], quads.size() * 4);
}
void gfoil::quad::batch_renderer::buffer_data(std::vector<quad::texture>& quads) {
	if (this->vertex_type != vertex::type::TEXTURE) { system::log::error("Attempting to buffer incorrect type to quad batch renderer"); };
	this->renderer.buffer_data(&quads[0].vertices[0], quads.size() * 4);
}
void gfoil::quad::batch_renderer::buffer_data(std::vector<quad::tint>& quads) {
	if (this->vertex_type != vertex::type::TINTED) { system::log::error("Attempting to buffer incorrect type to quad batch renderer"); };
	this->renderer.buffer_data(&quads[0].vertices[0], quads.size() * 4);
}