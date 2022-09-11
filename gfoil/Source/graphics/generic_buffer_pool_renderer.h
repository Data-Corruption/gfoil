#pragma once

#include <array>

#include <glad/glad.h> 
#include <glm/glm.hpp>

#include "buffer_array_object.h"
#include "buffer.h"

#include "small_types/vertex.h"

namespace gfoil {
	class generic_buffer_pool_renderer {
	public:

		struct pool_buffer {
			buffer buffer;
			unsigned int last_commit_index = -1;
			bool modified = false;
			bool queued = false;
		};

		// ----==== Members ====----

		primative_type primative_type;
		vertex::type vertex_type;

		std::vector<pool_buffer> buffer_pool;
		std::vector<unsigned int> last_commit;
		unsigned int current_commit_count = 0;

		// max verticies per draw
		unsigned int max_verticies_per_draw;
		unsigned int max_verticies_per_buffer;
		unsigned int vertex_group_size;
		unsigned int indices_per_group;
		unsigned int size_of_vertex;

		// ----==== Methods ====----

		/// If using an index buffer it must be a multiple of vertex group size.
		/// Index stuff is ignored if index_buffer_id is 0.
		/// If you buffer data to a buffer out of range it will generate new buffers to meet that pool size.
		/// If you commit a buffer out of range it will throw an error.
		void generate(
			unsigned int max_verticies_per_draw,
			unsigned int max_verticies_per_buffer, 
			unsigned int starting_buffer_count, 
			enum primative_type primative,
			enum vertex::type vertex,
			unsigned int index_buffer_id,
			unsigned int vertex_group_size,
			unsigned int indices_per_group
		);
		void destroy();
		
		void buffer_data(unsigned int& buffer_index, vertex::color* start_of_vertices, unsigned int& number_of_vertices);
		void buffer_data(unsigned int& buffer_index, vertex::texture* start_of_vertices, unsigned int& number_of_vertices);
		void buffer_data(unsigned int& buffer_index, vertex::tint* start_of_vertices, unsigned int& number_of_vertices);

		// resets current draw
		void resize_pool(unsigned int& new_size);
		void commit(unsigned int& buffer_index) { buffer_pool[buffer_index].queued = true; current_commit_count++; };
		void draw();

	private:

		buffer_array_object bao;
		buffer draw_buffer;
		unsigned int index_buffer_id;

		void gen_buff(unsigned int& max_vertices, buffer& target_buffer);
		void copy_buff(buffer& source, buffer& destination, int read_offset, int write_offset, int size);

	};
}