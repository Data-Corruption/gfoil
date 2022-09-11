#include "generic_buffer_pool_renderer.h"

#include<algorithm>

#include "../system/system.h"

void gfoil::generic_buffer_pool_renderer::generate(
	unsigned int max_verticies_per_draw,
	unsigned int max_verticies_per_buffer,
	unsigned int starting_buffer_count,
	enum primative_type primative,
	enum vertex::type vertex,
	unsigned int index_buffer_id,
	unsigned int vertex_group_size,
	unsigned int indices_per_group
) {
	if (this->bao.ID != 0)
		system::log::error("Attempting to generate already generated generic batch renderer");

	this->max_verticies_per_draw = max_verticies_per_draw;
	this->max_verticies_per_buffer = max_verticies_per_buffer;
	this->primative_type = primative;
	this->vertex_type = vertex;
	this->index_buffer_id = index_buffer_id;
	this->vertex_group_size = vertex_group_size;
	this->indices_per_group = indices_per_group;

	this->bao.generate();

	if (this->index_buffer_id != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		if (this->max_verticies_per_draw % this->vertex_group_size)
			system::log::error("generic_batch_renderer: max_batch_size is not a multiple of index_group_size");
	}

	this->gen_buff(this->max_verticies_per_draw, this->draw_buffer);

	for (int x = 0; x < starting_buffer_count; x++) {
		this->buffer_pool.emplace_back();
		this->gen_buff(this->max_verticies_per_buffer, this->buffer_pool.back().buffer);
	}

	switch (this->vertex_type) {
	case vertex::type::COLOR: this->size_of_vertex = sizeof(vertex::color); break;
	case vertex::type::TEXTURE: this->size_of_vertex = sizeof(vertex::texture); break;
	case vertex::type::TINTED: this->size_of_vertex = sizeof(vertex::tint); break;
	}

}
void gfoil::generic_buffer_pool_renderer::destroy() {
	this->draw_buffer.destroy();
	this->bao.destroy();

	this->last_commit.clear();
	this->last_commit.shrink_to_fit();

	for (auto& buff : this->buffer_pool)
		buff.buffer.destroy();

	this->buffer_pool.clear();
	this->buffer_pool.shrink_to_fit();

	this->index_buffer_id = 0;
}

void gfoil::generic_buffer_pool_renderer::buffer_data(unsigned int& buffer_index, vertex::color* start_of_vertices, unsigned int& number_of_vertices) {
	if (this->buffer_pool.size() < buffer_index)
		system::log::error("buffer pool: index out of range");
	if (this->max_verticies_per_buffer < number_of_vertices)
		system::log::error("buffer pool: buffering more vertices than max buffer size");
	this->buffer_pool[buffer_index].buffer.buffer_sub_data(0, number_of_vertices * sizeof(vertex::color), start_of_vertices);
	this->buffer_pool[buffer_index].modified = true;
}
void gfoil::generic_buffer_pool_renderer::buffer_data(unsigned int& buffer_index, vertex::texture* start_of_vertices, unsigned int& number_of_vertices) {
	if (this->buffer_pool.size() < buffer_index)
		system::log::error("buffer pool index out of range");
	if (this->max_verticies_per_buffer < number_of_vertices)
		system::log::error("buffer pool: buffering more vertices than max buffer size");
	this->buffer_pool[buffer_index].buffer.buffer_sub_data(0, number_of_vertices * sizeof(vertex::texture), start_of_vertices);
	this->buffer_pool[buffer_index].modified = true;
}
void gfoil::generic_buffer_pool_renderer::buffer_data(unsigned int& buffer_index, vertex::tint* start_of_vertices, unsigned int& number_of_vertices) {
	if (this->buffer_pool.size() < buffer_index)
		system::log::error("buffer pool index out of range");
	if (this->max_verticies_per_buffer < number_of_vertices)
		system::log::error("buffer pool: buffering more vertices than max buffer size");
	this->buffer_pool[buffer_index].buffer.buffer_sub_data(0, number_of_vertices * sizeof(vertex::tint), start_of_vertices);
	this->buffer_pool[buffer_index].modified = true;
}

void gfoil::generic_buffer_pool_renderer::resize_pool(unsigned int& new_size) {
	if (this->buffer_pool.size() == new_size)
		return;

	this->last_commit.clear();
	this->last_commit.shrink_to_fit();

	if (this->buffer_pool.size() > new_size) {
		for (int x = new_size; x < this->buffer_pool.size(); x++)
			this->buffer_pool[x].buffer.destroy();
		this->buffer_pool.resize(new_size);
		this->buffer_pool.shrink_to_fit();
	} else {
		int number_of_new_buffers = new_size - this->buffer_pool.size();
		for (int x = 0; x < number_of_new_buffers; x++) {
			this->buffer_pool.emplace_back();
			this->gen_buff(this->max_verticies_per_buffer, this->buffer_pool.back().buffer);
		}
	}
}
void gfoil::generic_buffer_pool_renderer::draw() {
	if (current_commit_count == 0)
		return;
	current_commit_count = 0;


	std::vector<unsigned int> new_commit;
	unsigned int tail_index = 0;
	unsigned int tail_byte_offset = 0;

	// get tail index / byte offset
	for (int i = 0; i < this->last_commit.size(); i++) {
		if ( (buffer_pool[last_commit[i]].modified) || (!buffer_pool[last_commit[i]].queued) )
			break;
		tail_byte_offset += buffer_pool[last_commit[i]].buffer.last_tail_in_bytes;
		buffer_pool[last_commit[i]].queued = false;
		new_commit.push_back(last_commit[i]);
		tail_index++;
	}

	for (auto& buffer : this->buffer_pool)
		if (buffer.last_commit_index >= tail_index)
			buffer.last_commit_index = -1;

	// copy queued buffers that are unedited and were in the last draw
	for (int i = 0; i < this->buffer_pool.size(); i++) {
		if (buffer_pool[i].queued && !buffer_pool[i].modified) {
			new_commit.push_back(i);
			buffer_pool[i].queued = false;
			buffer_pool[i].last_commit_index = tail_index;
			tail_index++;

			this->copy_buff(buffer_pool[i].buffer, this->draw_buffer, 0, tail_byte_offset, buffer_pool[i].buffer.last_tail_in_bytes);
			tail_byte_offset += buffer_pool[i].buffer.last_tail_in_bytes;
		}
	}

	// copy remaining queued buffers
	for (int i = 0; i < this->buffer_pool.size(); i++) {
		if (buffer_pool[i].queued) {
			new_commit.push_back(i);
			buffer_pool[i].queued = false;
			buffer_pool[i].modified = false;
			buffer_pool[i].last_commit_index = tail_index;
			tail_index++;

			this->copy_buff(buffer_pool[i].buffer, this->draw_buffer, 0, tail_byte_offset, buffer_pool[i].buffer.last_tail_in_bytes);
			tail_byte_offset += buffer_pool[i].buffer.last_tail_in_bytes;
		}
	}

	this->last_commit = new_commit;

	// bind vao and draw
	this->bao.bind();

	unsigned int last_batch_size = tail_byte_offset / this->size_of_vertex;

	if (this->index_buffer_id == 0) {
		glDrawArrays((GLenum)this->primative_type, 0, last_batch_size);
	} else {
		if (last_batch_size % this->vertex_group_size)
			system::log::error("generic_batch_renderer: max_batch_size is not a multiple of index_group_size");
		glDrawElements((GLenum)this->primative_type, (last_batch_size / this->vertex_group_size) * this->indices_per_group, GL_UNSIGNED_SHORT, nullptr);
	}

	this->bao.unbind();

}

void gfoil::generic_buffer_pool_renderer::gen_buff(unsigned int& max_vertices, buffer& target_buffer) {
	target_buffer.generate(buffer::types::VERTEX);

	switch (this->vertex_type) {
	case vertex::type::COLOR:
		target_buffer.buffer_data(max_vertices * sizeof(vertex::color), nullptr, buffer::draw_types::DYNAMIC);
		target_buffer.enable_vertex_attributes({ 0, 1 });
		target_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(vertex::color), (const void*)offsetof(vertex::color, position));
		target_buffer.set_vertex_attribute(1, 4, buffer::data_types::FLOAT, false, sizeof(vertex::color), (const void*)offsetof(vertex::color, color));
		break;
	case vertex::type::TEXTURE:
		target_buffer.buffer_data(max_vertices * sizeof(vertex::texture), nullptr, buffer::draw_types::DYNAMIC);
		target_buffer.enable_vertex_attributes({ 0, 1 });
		target_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(vertex::texture), (const void*)offsetof(vertex::texture, position));
		target_buffer.set_vertex_attribute(1, 3, buffer::data_types::FLOAT, false, sizeof(vertex::texture), (const void*)offsetof(vertex::texture, texture_coords));
		break;
	case vertex::type::TINTED:
		target_buffer.buffer_data(max_vertices * sizeof(vertex::tint), nullptr, buffer::draw_types::DYNAMIC);
		target_buffer.enable_vertex_attributes({ 0, 1, 2 });
		target_buffer.set_vertex_attribute(0, 3, buffer::data_types::FLOAT, false, sizeof(vertex::tint), (const void*)offsetof(vertex::tint, position));
		target_buffer.set_vertex_attribute(1, 3, buffer::data_types::FLOAT, false, sizeof(vertex::tint), (const void*)offsetof(vertex::tint, texture_coords));
		target_buffer.set_vertex_attribute(2, 3, buffer::data_types::FLOAT, false, sizeof(vertex::tint), (const void*)offsetof(vertex::tint, color));
		break;
	}
}

void gfoil::generic_buffer_pool_renderer::copy_buff(buffer& source, buffer& destination, int read_offset, int write_offset, int size) {

	buffer::bound_buffer_ID = 0;

	glBindBuffer(GL_COPY_READ_BUFFER, source.id);
	glBindBuffer(GL_COPY_WRITE_BUFFER, destination.id);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, read_offset, write_offset, size);

}