#include "generic_batch_renderer.h"

#include "../system/system.h"

void gfoil::generic_batch_renderer::generate(
	unsigned int count,
	enum primative_type primative,
	enum vertex::type vertex,
	unsigned int index_buffer_id,
	unsigned int vertex_group_size,
	unsigned int indices_per_group,
	bool allow_overflow
) {

	this->max_batch_size = count;
	this->current_batch_size = 0;
	this->primative_type = primative;
	this->vertex_type = vertex;
	this->index_buffer_id = index_buffer_id;
	this->vertex_group_size = vertex_group_size;
	this->indices_per_group = indices_per_group;
	this->allow_overflow = allow_overflow;

	this->bao.generate();
	
	if (this->index_buffer_id != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		if (this->max_batch_size % this->vertex_group_size)
			system::log::error("generic_batch_renderer: max_batch_size is not a multiple of index_group_size");
	}

	this->vertex_buffer.generate(buffer::types::VERTEX);

	switch (this->vertex_type) {
	case vertex::type::TEXTURE:
		this->data_texture.resize(this->max_batch_size);
		this->vertex_buffer.buffer_data(this->max_batch_size * sizeof(vertex::texture), nullptr, buffer::draw_types::DYNAMIC);
		this->vertex_buffer.enable_vertex_attributes({0, 1});
		this->vertex_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(vertex::texture), (const void*)offsetof(vertex::texture, position));
		this->vertex_buffer.set_vertex_attribute(1, 3, buffer::data_types::FLOAT, false, sizeof(vertex::texture), (const void*)offsetof(vertex::texture, texture_coords));
		break;
	case vertex::type::TINTED:
		this->data_tint.resize(this->max_batch_size);
		this->vertex_buffer.buffer_data(this->max_batch_size * sizeof(vertex::tint), nullptr, buffer::draw_types::DYNAMIC);
		this->vertex_buffer.enable_vertex_attributes({ 0, 1, 2 });
		this->vertex_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(vertex::tint), (const void*)offsetof(vertex::tint, position));
		this->vertex_buffer.set_vertex_attribute(1, 3, buffer::data_types::FLOAT, false, sizeof(vertex::tint), (const void*)offsetof(vertex::tint, texture_coords));
		this->vertex_buffer.set_vertex_attribute(2, 3, buffer::data_types::FLOAT, false, sizeof(vertex::tint), (const void*)offsetof(vertex::tint, color));
		break;
	case vertex::type::COLOR:
		this->data_color.resize(this->max_batch_size);
		this->vertex_buffer.buffer_data(this->max_batch_size * sizeof(vertex::color), nullptr, buffer::draw_types::DYNAMIC);
		this->vertex_buffer.enable_vertex_attributes({ 0, 1 });
		this->vertex_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(vertex::color), (const void*)offsetof(vertex::color, position));
		this->vertex_buffer.set_vertex_attribute(1, 4, buffer::data_types::FLOAT, false, sizeof(vertex::color), (const void*)offsetof(vertex::color, color));
		break;
	}

}
void gfoil::generic_batch_renderer::destroy() {
	this->vertex_buffer.destroy();
	this->bao.destroy();
}

void gfoil::generic_batch_renderer::flush() {
	if (this->current_batch_size == 0)
		return;

	this->vertex_buffer.bind();

	switch (this->vertex_type) {
	case vertex::type::TEXTURE: this->vertex_buffer.buffer_sub_data(0, sizeof(vertex::texture) * this->current_batch_size, &data_texture[0]); break;
	case vertex::type::TINTED:  this->vertex_buffer.buffer_sub_data(0, sizeof(vertex::tint) * this->current_batch_size, &data_tint[0]); break;
	case vertex::type::COLOR:   this->vertex_buffer.buffer_sub_data(0, sizeof(vertex::color) * this->current_batch_size, &data_color[0]); break;
	}

	this->last_batch_size = current_batch_size;
	this->current_batch_size = 0;
}

void gfoil::generic_batch_renderer::draw() {
	if (this->last_batch_size == 0)
		return;

	this->bao.bind();

	if (this->index_buffer_id == 0) {
		glDrawArrays((GLenum)this->primative_type, 0, this->last_batch_size);
	} else {
		if (this->last_batch_size % this->vertex_group_size)
			system::log::error("generic_batch_renderer: max_batch_size is not a multiple of index_group_size");
		glDrawElements((GLenum)this->primative_type, (this->last_batch_size / this->vertex_group_size) * this->indices_per_group, GL_UNSIGNED_SHORT, nullptr);
	}

	this->bao.unbind();
}

void gfoil::generic_batch_renderer::buffer_data(std::vector<vertex::texture>& vertices) {
	if (vertices.size() == 0) {
		system::log::warn("buffering nothing to: " + std::to_string(vertex_buffer.id));
		return;
	}
	if (this->vertex_type != vertex::type::TEXTURE)
		system::log::error("generic_batch_renderer: drawing a different vertex type");

	if (vertices.size() > this->max_batch_size)
		system::log::error("generic_batch_renderer: attempting to draw more vertices than max batch size");
	if ((this->current_batch_size + vertices.size()) > this->max_batch_size) {
		if (this->allow_overflow) {
			this->flush();
			this->draw();
		} else {
			system::log::error("generic_batch_renderer: buffer overflow!!!");
		}
	}

	std::copy(vertices.begin(), vertices.end(), this->data_texture.begin() + this->current_batch_size);
	this->current_batch_size += (unsigned int)vertices.size();
}
void gfoil::generic_batch_renderer::buffer_data(std::vector<vertex::tint>& vertices) {
	if (vertices.size() == 0) { 
		system::log::warn("buffering nothing to: " + std::to_string(vertex_buffer.id));
		return;
	}
	if (this->vertex_type != vertex::type::TINTED)
		system::log::error("generic_batch_renderer: drawing a different vertex type");

	if (vertices.size() > this->max_batch_size)
		system::log::error("generic_batch_renderer: attempting to draw more vertices than max batch size");
	if ((this->current_batch_size + vertices.size()) > this->max_batch_size) {
		if (this->allow_overflow) {
			this->flush();
			this->draw();
		} else {
			system::log::error("generic_batch_renderer: buffer overflow!!!");
		}
	}

	std::copy(vertices.begin(), vertices.end(), this->data_tint.begin() + this->current_batch_size);
	this->current_batch_size += (unsigned int)vertices.size();
}
void gfoil::generic_batch_renderer::buffer_data(std::vector<vertex::color>& vertices) {
	if (vertices.size() == 0) {
		system::log::warn("buffering nothing to: " + std::to_string(vertex_buffer.id));
		return;
	}
	if (this->vertex_type != vertex::type::COLOR)
		system::log::error("generic_batch_renderer: drawing a different vertex type");

	if (vertices.size() > this->max_batch_size)
		system::log::error("generic_batch_renderer: attempting to draw more vertices than max batch size");
	if ((this->current_batch_size + vertices.size()) > this->max_batch_size) {
		if (this->allow_overflow) {
			this->flush();
			this->draw();
		} else {
			system::log::error("generic_batch_renderer: buffer overflow!!!");
		}
	}

	std::copy(vertices.begin(), vertices.end(), this->data_color.begin() + this->current_batch_size);
	this->current_batch_size += (unsigned int)vertices.size();
}