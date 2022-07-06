#include "generic_batch_renderer.h"

#include "../system/system.h"

void gfoil::generic_batch_renderer::generate(
	unsigned int count,
	gfoil::primative_type primative_type,
	gfoil::vertex_type vertex_type,
	unsigned int index_buffer_id,
	unsigned int index_group_size,
	unsigned int indices_per_group,
	bool allow_overflow
) {

	this->max_batch_size = count;
	this->current_batch_size = 0;
	this->primative_type = primative_type;
	this->vertex_type = vertex_type;
	this->index_buffer_id = index_buffer_id;
	this->vertex_group_size = vertex_group_size;
	this->indices_per_group = indices_per_group;
	this->allow_overflow = allow_overflow;

	this->bao.generate();
	
	if (this->index_buffer_id != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		if (this->vertex_group_size % this->max_batch_size)
			system::log::error("generic_batch_renderer: max_batch_size is not a multiple of index_group_size");
	}

	this->vertex_buffer.generate(buffer::types::VERTEX);

	switch (this->vertex_type) {
	case vertex_type::TEXTURE:
		this->data_texture.resize(this->max_batch_size);
		this->vertex_buffer.buffer_data(this->max_batch_size * sizeof(texture_vertex), nullptr, buffer::draw_types::DYNAMIC);
		this->vertex_buffer.enable_vertex_attributes({0, 1});
		this->vertex_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(texture_vertex), (const void*)offsetof(texture_vertex, position));
		this->vertex_buffer.set_vertex_attribute(1, 3, buffer::data_types::FLOAT, false, sizeof(texture_vertex), (const void*)offsetof(texture_vertex, texture_coords));
		break;
	case vertex_type::TINTED:
		this->vertex_buffer.buffer_data(this->max_batch_size * sizeof(tint_vertex), nullptr, buffer::draw_types::DYNAMIC);
		this->vertex_buffer.enable_vertex_attributes({ 0, 1, 2 });
		this->vertex_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(tint_vertex), (const void*)offsetof(tint_vertex, position));
		this->vertex_buffer.set_vertex_attribute(1, 3, buffer::data_types::FLOAT, false, sizeof(tint_vertex), (const void*)offsetof(tint_vertex, texture_coords));
		this->vertex_buffer.set_vertex_attribute(2, 3, buffer::data_types::FLOAT, false, sizeof(tint_vertex), (const void*)offsetof(tint_vertex, color));
		break;
	case vertex_type::COLOR:
		this->vertex_buffer.buffer_data(this->max_batch_size * sizeof(color_vertex), nullptr, buffer::draw_types::DYNAMIC);
		this->vertex_buffer.enable_vertex_attributes({ 0, 1 });
		this->vertex_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(color_vertex), (const void*)offsetof(color_vertex, position));
		this->vertex_buffer.set_vertex_attribute(1, 4, buffer::data_types::FLOAT, false, sizeof(color_vertex), (const void*)offsetof(color_vertex, color));
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
	case vertex_type::TEXTURE: this->vertex_buffer.buffer_sub_data(0, sizeof(texture_vertex) * this->current_batch_size, &data_texture[0]); break;
	case vertex_type::TINTED:  this->vertex_buffer.buffer_sub_data(0, sizeof(tint_vertex) * this->current_batch_size, &data_tint[0]); break;
	case vertex_type::COLOR:   this->vertex_buffer.buffer_sub_data(0, sizeof(color_vertex) * this->current_batch_size, &data_color[0]); break;
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
		glDrawElements((GLenum)this->primative_type, this->last_batch_size, GL_UNSIGNED_SHORT, nullptr);
	}

	this->bao.unbind();
}

void gfoil::generic_batch_renderer::buffer(std::vector<texture_vertex>& vertices) {
	if (vertices.size() == 0)
		return;
	if (this->vertex_type != vertex_type::TEXTURE)
		system::log::error("generic_batch_renderer: drawing a different vertex type");

	if (vertices.size() > this->max_batch_size)
		system::log::error("generic_batch_renderer: attempting to draw more vertices than max batch size");
	if ((this->data_texture.size() + vertices.size()) > this->max_batch_size) { 
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
void gfoil::generic_batch_renderer::buffer(std::vector<tint_vertex>& vertices) {
	if (vertices.size() == 0)
		return;
	if (this->vertex_type != vertex_type::TINTED)
		system::log::error("generic_batch_renderer: drawing a different vertex type");

	if (vertices.size() > this->max_batch_size)
		system::log::error("generic_batch_renderer: attempting to draw more vertices than max batch size");
	if ((this->data_tint.size() + vertices.size()) > this->max_batch_size) {
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
void gfoil::generic_batch_renderer::buffer(std::vector<color_vertex>& vertices) {
	if (vertices.size() == 0)
		return;
	if (this->vertex_type != vertex_type::COLOR)
		system::log::error("generic_batch_renderer: drawing a different vertex type");

	if (vertices.size() > this->max_batch_size)
		system::log::error("generic_batch_renderer: attempting to draw more vertices than max batch size");
	if ((this->data_color.size() + vertices.size()) > this->max_batch_size) {
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