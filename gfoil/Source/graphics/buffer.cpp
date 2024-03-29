#include "buffer.h"

#include "../system.hpp"

// ----==== Members ====----

unsigned int gfoil::buffer::bound_buffer_ID = 0;

// ----==== Methods ====----

void gfoil::buffer::generate(enum types type) {
	if (this->id != 0) {
		system::log::error("Attempting to generate a buffer that has already generated!");
	} else {
		this->type = type;
		glGenBuffers(1, &this->id);
		this->bind();
	}
}
void gfoil::buffer::destroy() {
	if (this->id == 0) {
		system::log::error("Attempting to destroy a buffer that is not generated!");
	} else {
		glDeleteBuffers(1, &this->id);
		this->id = 0;
	}
}

void gfoil::buffer::bind() {
	if (this->id == 0)
		system::log::error("Attempting to bind a buffer that is not generated!");

	if (bound_buffer_ID != this->id) {
		glBindBuffer((GLenum)this->type, this->id);
		bound_buffer_ID = this->id;
	}
}

void gfoil::buffer::buffer_data(unsigned int size_of_data_in_bytes, void* start_of_data, draw_types draw_type) {
	if (bound_buffer_ID != this->id)
		system::log::error("Attempting to buffer_data() for an unbound buffer!");
	glBufferData((GLenum)this->type, size_of_data_in_bytes, start_of_data, (GLenum)draw_type);
	this->last_tail_in_bytes = size_of_data_in_bytes;
}
void gfoil::buffer::buffer_sub_data(unsigned int offset_into_buffer, unsigned int size_of_data_in_bytes, void* start_of_data) {
	if (bound_buffer_ID != this->id)
		system::log::error("Attempting to buffer_sub_data() for an unbound buffer!");
	glBufferSubData((GLenum)this->type, offset_into_buffer, size_of_data_in_bytes, start_of_data);
	this->last_tail_in_bytes = offset_into_buffer + size_of_data_in_bytes;
}

void gfoil::buffer::enable_vertex_attributes(std::vector<unsigned int> positions) {
	if (bound_buffer_ID != this->id)
		system::log::error("Attempting to enable vertex attributes for an unbound buffer!");
	for (int i = 0; i < positions.size(); i++)
		glEnableVertexAttribArray(positions[i]);
}
void gfoil::buffer::disable_vertex_attributes(std::vector<unsigned int> positions) {
	if (bound_buffer_ID != this->id)
		system::log::error("Attempting to disable vertex attributes for an unbound buffer!");
	for (int i = 0; i < positions.size(); i++)
		glDisableVertexAttribArray(positions[i]);
}

void gfoil::buffer::set_vertex_attribute(unsigned int position, unsigned int size, data_types type, bool normalize, int stride, const void* component_offset) {
	if (bound_buffer_ID != this->id)
		system::log::error("Attempting to set a vertex attribute for an unbound buffer!");
	glVertexAttribPointer(position, size, (GLenum)type, normalize, stride, component_offset);
}
void gfoil::buffer::set_int_vertex_attribute(unsigned int position, unsigned int size, data_types type, int stride, const void* component_offset) {
	if (bound_buffer_ID != this->id)
		system::log::error("Attempting to set a vertex attribute for an unbound buffer!");
	glVertexAttribIPointer(position, size, (GLenum)type, stride, component_offset);
}